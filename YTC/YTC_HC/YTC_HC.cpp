// CAM_HC.cpp : 定义 DLL 应用程序的导出函数。
//

#include <windows.h>

#include "HCNetSDK.h"
#pragma comment(lib, "HCNetSDK.lib")

#include "plaympeg4.h"
#pragma comment(lib, "PlayCtrl.lib")

extern "C"
{
#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
#include "stdint.h"
#endif


#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
};

#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"swscale.lib")

#include "jpeglib.h"
#include "jerror.h"

#pragma comment(lib,"jpeg.lib")

const int SWS_FAST_FLAGS = SWS_CPU_CAPS_MMX | SWS_CPU_CAPS_MMX2 | SWS_CPU_CAPS_3DNOW | SWS_CPU_CAPS_ALTIVEC | SWS_CPU_CAPS_BFIN;


#include <map>
#include <string>
#include "Video.h"
#include "PTZ.h"
#include "ThreadMutex.h"
#include <assert.h>

#if defined(_DEBUG)
#if defined(_MEM_LEAK_DETECT) // 内存泄漏检测支持。
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <malloc.h> // 解决 malloc.h 与 crtdbg.h 顺序导致的 Debug Assertion Failed, "Corrupted pointer passed to _freea" 。
#include <crtdbg.h>
#undef DEBUG_NEW
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif
#endif // _DEBUG

#define DEVICE_PIX_FMT ZW_PIX_FMT_YV12

struct internal_handle
{
	ZWVideoParam param;

	VideoStreamCallback OnVideoStreamReady;/*实时视频流回调*/
	RealTimeImageCallback OnRealImgReady;/*实时图像输出回调*/
	RealTimeImageCallback OnRealFixFmtImgReady;/*实时转码图像输出回调*/
	int nZwRealFixFmt;
	TriggerImageCallback OnTriggerImgReady;  /*外部触发抓拍图片回调*/
	TriggerImageCallback OnTriggerFixFmtImgReady;/*外部触发抓拍转码图片回调*/
	int nZwTriggerFixFmt;
	void* pUserData;

	volatile long  nRefCount;
	std::string ipcode;

	SwsContext* pRealFixFmtCtx;
	char* pRealFixFmtImgBuffer;
	SwsContext* pTriggerFixFmtCtx;
	char* pTriggerFixFmtImgSwsBuffer;
	char* pTriggerFixFmtImgBuffer;

	long lUserHandle; //用户句柄 
	long lChannel;
	long lRealPlayHandle;//实时预览句柄
	long lPort;
	long lAlarmHandle;//报警通道句柄

	ThreadMutex mtx;

	internal_handle()
		:ipcode()
	{
		param = { 0 };

		OnVideoStreamReady = NULL;
		OnRealImgReady = NULL;
		OnRealFixFmtImgReady = NULL;
		nZwRealFixFmt = ZW_PIX_FMT_NONE;
		OnTriggerImgReady = NULL;
		OnTriggerFixFmtImgReady = NULL;
		nZwTriggerFixFmt = ZW_PIX_FMT_NONE;
		pUserData = NULL;

		nRefCount = 0;

		pRealFixFmtCtx = NULL;
		pRealFixFmtImgBuffer = NULL;

		pTriggerFixFmtCtx = NULL;
		pTriggerFixFmtImgSwsBuffer = NULL;
		pTriggerFixFmtImgBuffer = NULL;

		lUserHandle = -1;
		lChannel = -1;
		lRealPlayHandle = -1;
		lPort = -1;
		lAlarmHandle = -1;
	}
};

ThreadMutex  g_mtx;
std::map<std::string, internal_handle*> g_handle_map;
volatile long g_nInternal_UserNum = 0;//设备接入SDK初始化计数


void WriteLog(char* pMsg)
{
#if defined(_DEBUG) 
	try
	{
		SYSTEMTIME oT;
		::GetLocalTime(&oT);
		FILE* pLog = fopen("d:\\YTCHC.log.txt", "a");
		fprintf(pLog, "%02d/%02d/%04d, %02d:%02d:%02d\n    %s\n", oT.wMonth, oT.wDay, oT.wYear, oT.wHour, oT.wMinute, oT.wSecond, pMsg);
		fclose(pLog);
	}
	catch (...) {}
#endif
}

#include <setjmp.h>

struct jpg_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */
	jmp_buf setjmp_buffer;		/* for return to caller */
};
typedef jpg_error_mgr *jpg_error_ptr;


////////////////////////////////////////////////////////////////////////////////
// Here's the routine that will replace the standard error_exit method:
////////////////////////////////////////////////////////////////////////////////
static void
ima_jpeg_error_exit (j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	jpg_error_ptr myerr = (jpg_error_ptr) cinfo->err;

	char pTemp[121];
	sprintf(pTemp, "JPG解码错误 %d", myerr->pub.msg_code);
	WriteLog(pTemp);

	/* Send it to stderr, adding a newline */
	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

int MapPixelFormat(enum AVPixelFormat fmt)
{
	int pixfmt = ZW_PIX_FMT_NONE;
	switch (fmt)
	{
	case AV_PIX_FMT_RGB24:
		pixfmt = ZW_PIX_FMT_RGB24;
		break;
	case AV_PIX_FMT_BGR24:
		pixfmt = ZW_PIX_FMT_BGR24;
		break;
	case AV_PIX_FMT_YUV420P:
		pixfmt = ZW_PIX_FMT_YUV420P;
		break;
		/*ZW_PIX_FMT_YV12 ffmpeg不支持*/
	case AV_PIX_FMT_NV12:
		pixfmt = ZW_PIX_FMT_NV12;
		break;
	case AV_PIX_FMT_NV21:
		pixfmt = ZW_PIX_FMT_NV21;
		break;
	case AV_PIX_FMT_YUV422P:
		pixfmt = ZW_PIX_FMT_YUV422P;
		break;
		/*ZW_PIX_FMT_YV16 ffmpeg不支持*/
	case AV_PIX_FMT_NV16:
		pixfmt = ZW_PIX_FMT_NV16;
		break;
		/*ZW_PIX_FMT_NV61 ffmpeg不支持*/
	case AV_PIX_FMT_YUYV422:
		pixfmt = ZW_PIX_FMT_YUYV422;
		break;
		/*ZW_PIX_FMT_YVYU422 ffmpeg不支持*/
	case AV_PIX_FMT_UYVY422:
		pixfmt = ZW_PIX_FMT_UYVY422;
		break;
		/*ZW_PIX_FMT_VYUY422 ffmpeg不支持*/
	case AV_PIX_FMT_YUV444P:
		pixfmt = ZW_PIX_FMT_YUV444P;
		break;
		/*ZW_PIX_FMT_YVU444P ffmpeg不支持*/
	default:
		pixfmt = ZW_PIX_FMT_NONE;
		break;
	}

	return pixfmt;
}

enum AVPixelFormat MapPixelFormat2(int fmt, int *swapCbCr)
{
	int swap = 0;
	int swap2 = *swapCbCr;
	enum AVPixelFormat pixfmt = AV_PIX_FMT_NONE;
	switch (fmt)
	{
	case ZW_PIX_FMT_YUV420P:
		pixfmt = AV_PIX_FMT_YUV420P;
		break;
	case ZW_PIX_FMT_YV12:
		swap = 1;
		pixfmt = AV_PIX_FMT_YUV420P; /*ffmpeg不支持*/
		break;
	case  ZW_PIX_FMT_YUV422P:
		pixfmt = AV_PIX_FMT_YUV422P;
		break;
	case  ZW_PIX_FMT_YV16:
		swap = 1;
		pixfmt = AV_PIX_FMT_YUV422P; /*ffmpeg不支持*/
		break;
	case ZW_PIX_FMT_YUV444P:
		pixfmt = AV_PIX_FMT_YUV444P;
		break;
	case ZW_PIX_FMT_YVU444P:
		swap = 1;
		pixfmt = AV_PIX_FMT_YUV444P; /*ffmpeg不支持*/
		break;


	case ZW_PIX_FMT_RGB24:
		swap = 1;
		pixfmt = swap2==1? AV_PIX_FMT_BGR24 : AV_PIX_FMT_RGB24;
		break;
	case ZW_PIX_FMT_BGR24:
		swap = 1;
		pixfmt = swap2 == 1 ? AV_PIX_FMT_RGB24 : AV_PIX_FMT_BGR24;
		break;
	case ZW_PIX_FMT_NV12 :
		swap = 1;
		pixfmt = swap2 == 1 ? AV_PIX_FMT_NV21 : AV_PIX_FMT_NV12;
		break;
	case ZW_PIX_FMT_NV21:
		swap = 1;
		pixfmt = swap2 == 1 ? AV_PIX_FMT_NV12 : AV_PIX_FMT_NV21;
		break;


	case ZW_PIX_FMT_NV16:
		pixfmt = AV_PIX_FMT_NV16;
		break;
	case ZW_PIX_FMT_NV61:
		swap = 1;
		pixfmt = AV_PIX_FMT_NV16; /*ffmpeg不支持*/
		break;
	case ZW_PIX_FMT_YUYV422 :
		pixfmt = AV_PIX_FMT_YUYV422;
		break;
	case ZW_PIX_FMT_YVYU422:
		swap = 1;
		pixfmt = AV_PIX_FMT_YUYV422; /*ffmpeg不支持*/
		break;
	case ZW_PIX_FMT_UYVY422:
		pixfmt = AV_PIX_FMT_UYVY422;
		break;
	case ZW_PIX_FMT_VYUY422:
		swap = 1;
		pixfmt = AV_PIX_FMT_UYVY422; /*ffmpeg不支持*/
		break;

	default:
		pixfmt = AV_PIX_FMT_NONE;
		break;
	}

	*swapCbCr = swap;

	return pixfmt;
}


void GetPixelFormat(int inFmt, int outFmt, enum AVPixelFormat *srcFormat, enum AVPixelFormat *dstFormat, int *swapCbCr )
{
	assert((inFmt > 50 && inFmt < 61) || (inFmt > 0 && inFmt < 7));
	assert((outFmt > 50 && outFmt < 55) || (outFmt > 0 && outFmt < 7));

	int inSwap = 0;
	int outSwap = 0;

	*srcFormat = MapPixelFormat2(inFmt,&inSwap);
	outSwap = inSwap;
	*dstFormat = MapPixelFormat2(outFmt, &outSwap);

	*swapCbCr = inSwap ^ outSwap;
}
									   
/*实时解码RGB回调*/
void CALLBACK g_RealRGBReadyCB(long nPort, char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1, long nReserved2)
{
	internal_handle *pih = (internal_handle*)nReserved1;

	if (pFrameInfo->nWidth == 0)
	{
		return;
	}
	int swap;
	enum AVPixelFormat srcFormat;
	enum AVPixelFormat dstFormat;

	GetPixelFormat(DEVICE_PIX_FMT, pih->nZwRealFixFmt, &srcFormat, &dstFormat, &swap);

	AVPicture avSrc;
	avpicture_fill(&avSrc, (uint8_t*)pBuf, srcFormat, pFrameInfo->nWidth, pFrameInfo->nHeight);

	ZWImageData imgData = { 0 };
	imgData.Width = (int)(pFrameInfo->nWidth);
	imgData.Height = (int)(pFrameInfo->nHeight);
	imgData.PixFmt = DEVICE_PIX_FMT;
	imgData.PlaneData[0] = avSrc.data[0];
	imgData.LineSize[0] = avSrc.linesize[0];
    imgData.PlaneData[1] = avSrc.data[1];
	imgData.LineSize[1] = avSrc.linesize[1];
	imgData.PlaneData[2] = avSrc.data[2];
	imgData.LineSize[2] = avSrc.linesize[2];
	//imgData.PlaneData[0] = pBuf;
	//imgData.LineSize[0] = (int)(pFrameInfo->nWidth);
	//imgData.PlaneData[1] = (pBuf+ pFrameInfo->nWidth*pFrameInfo->nHeight);
	//imgData.LineSize[1] = (int)(pFrameInfo->nWidth/2);
	//imgData.PlaneData[2] = ((char*)imgData.PlaneData[1]+ pFrameInfo->nWidth*pFrameInfo->nHeight/4);
	//imgData.LineSize[2] = imgData.LineSize[1];

	pih->mtx.Lock();
	if (pih->OnRealImgReady != NULL)
	{
		pih->OnRealImgReady(pih->pUserData, &imgData);
	}

	if (pih->OnRealFixFmtImgReady != NULL && pih->nZwRealFixFmt == DEVICE_PIX_FMT)
	{
		pih->OnRealFixFmtImgReady(pih->pUserData, &imgData);
	}
	pih->mtx.Unlock();

	pih->mtx.Lock();
	if (pih->OnRealFixFmtImgReady != NULL && pih->nZwRealFixFmt != DEVICE_PIX_FMT)
	{
		if (pih->pRealFixFmtCtx == NULL)
		{
			pih->pRealFixFmtCtx = sws_getContext(pFrameInfo->nWidth,
				pFrameInfo->nHeight,
				srcFormat,
				pFrameInfo->nWidth,
				pFrameInfo->nHeight,
				dstFormat,
				SWS_BICUBIC | SWS_FAST_FLAGS,
				NULL, NULL, NULL);
		}

		if (pih->pRealFixFmtCtx != NULL)
		{
			if (pih->pRealFixFmtImgBuffer == NULL)
			{
				int numBytes = avpicture_get_size(dstFormat, pFrameInfo->nWidth, pFrameInfo->nHeight);
				pih->pRealFixFmtImgBuffer = (char*)malloc(numBytes);
			}

			if (pih->pRealFixFmtImgBuffer != NULL)
			{
				AVPicture avDst;
				avpicture_fill(&avDst, (uint8_t*)pih->pRealFixFmtImgBuffer, dstFormat, pFrameInfo->nWidth, pFrameInfo->nHeight);

				if (sws_scale(pih->pRealFixFmtCtx, avSrc.data, avSrc.linesize, 0, pFrameInfo->nHeight, avDst.data, avDst.linesize) > 0)
				{
					ZWImageData imgData = { 0 };
					imgData.Width = (int)(pFrameInfo->nWidth);
					imgData.Height = (int)(pFrameInfo->nHeight);
					imgData.PixFmt = pih->nZwRealFixFmt;
					imgData.PlaneData[0] = avDst.data[0];
					imgData.LineSize[0] = avDst.linesize[0];

					if (swap == 1) 
					{
						imgData.PlaneData[1] = avDst.data[1];
						imgData.LineSize[1] = avDst.linesize[1];
						imgData.PlaneData[2] = avDst.data[2];
						imgData.LineSize[2] = avDst.linesize[2];
					}
					else 
					{
						imgData.PlaneData[1] = avDst.data[2];
						imgData.LineSize[1] = avDst.linesize[2];
						imgData.PlaneData[2] = avDst.data[1];
						imgData.LineSize[2] = avDst.linesize[1];
					}

					pih->OnRealFixFmtImgReady(pih->pUserData, &imgData);
				}
			}
		}
	}
	pih->mtx.Unlock();
}

 /*实时视频流回调*/
void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser)
{
	internal_handle *pih = (internal_handle*)dwUser;

	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD: //系统头
		if (pih->OnRealImgReady == NULL && pih->OnRealFixFmtImgReady == NULL)
			break;

		if (pih->lPort >= 0) 
		{
			PlayM4_Stop(pih->lPort);
			PlayM4_CloseStream(pih->lPort);
			pih->lPort = -1;
		}

		if (!PlayM4_GetPort(&(pih->lPort)))  //获取播放库未使用的通道号
		{
			break;
		}
		//第一次回调的是系统头，将获取的播放库port号赋值给全局port，下次回调数据时即使用此port号播放
		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(pih->lPort, STREAME_REALTIME))  //设置实时流播放模式
			{
				break;
			}

			if (!PlayM4_OpenStream(pih->lPort, pBuffer, dwBufSize, 1920 * 1080)) //打开流接口
			{
				break;
			}

			PlayM4_SetDecCallBackMend(pih->lPort, g_RealRGBReadyCB, (long)pih);

			if (!PlayM4_Play(pih->lPort, NULL)) //播放开始
			{
				break;
			}
		}
		break;
	case NET_DVR_STREAMDATA:  
		if (dwBufSize > 0)
		{
			pih->mtx.Lock();
			if (pih->OnVideoStreamReady != NULL)
			{
				pih->OnVideoStreamReady(pih->pUserData, (void*)pBuffer, dwBufSize, 0);
			}
			pih->mtx.Unlock();
		}

	    if (pih->OnRealImgReady == NULL && pih->OnRealFixFmtImgReady == NULL)
			break;

	    if (pih->lPort >= 0)
	    {
			if (!PlayM4_InputData(pih->lPort, pBuffer, dwBufSize))
			{
				break;
			}
		}
		break;
	}
}

BOOL CALLBACK g_MSGCallBack_EX(LONG lCommand, LONG lUserID, char *pBuf, DWORD dwBufLen)
{
	if (lCommand == COMM_ITS_PLATE_RESULT)
	{
		internal_handle *pih = NULL;
		g_mtx.Lock();
		std::map<std::string, internal_handle*>::iterator pos = g_handle_map.begin();
		while (pos != g_handle_map.end())
		{
			internal_handle *pih2 = pos->second;
			if (pih2->lUserHandle == lUserID)
			{
				pih = pih2;
				break;
			}

			pos++;
		}
		g_mtx.Unlock();

		if (pih != NULL)
		{
			ZWVehicleInfo vi;
			NET_ITS_PLATE_RESULT struITSPlateResult = { 0 };
			memcpy(&struITSPlateResult, pBuf, sizeof(struITSPlateResult));
			//车牌
			memcpy(vi.License, struITSPlateResult.struPlateInfo.sLicense + 2, 16);

			WriteLog("收到相机触发");

			//车牌颜色
			switch (struITSPlateResult.struPlateInfo.byColor)
			{
			case VCA_BLUE_PLATE:
				vi.PlateColor = ZW_COLOR_BLUE;
				break;
			case VCA_YELLOW_PLATE:
				vi.PlateColor = ZW_COLOR_YELLOW;
				break;
			case VCA_WHITE_PLATE:
				vi.PlateColor = ZW_COLOR_WHITE;
				break;
			case VCA_BLACK_PLATE:
				vi.PlateColor = ZW_COLOR_BLACK;
				break;
			case VCA_GREEN_PLATE:
				vi.PlateColor = ZW_COLOR_GREEN;
				break;
			case VCA_BKAIR_PLATE:
				vi.PlateColor = ZW_COLOR_BLACK;
				break;
			default:
				vi.PlateColor = ZW_COLOR_UNKNOWN;
				break;
			}

			//车身颜色
			switch (struITSPlateResult.struVehicleInfo.byColor)
			{
			case VCR_CLR_WHITE:
				vi.VehicleColor = ZW_COLOR_WHITE;
				break;
			case VCR_CLR_SILVER:
				vi.VehicleColor = ZW_COLOR_SILVER;
				break;
			case VCR_CLR_GRAY:
				vi.VehicleColor = ZW_COLOR_GRAY;
				break;
			case VCR_CLR_BLACK:
				vi.VehicleColor = ZW_COLOR_BLACK;
				break;
			case VCR_CLR_RED:
				vi.VehicleColor = ZW_COLOR_RED;
				break;
			case VCR_CLR_DARKBLUE:
				vi.VehicleColor = ZW_COLOR_DARKBLUE;
				break;
			case VCR_CLR_BLUE:
				vi.VehicleColor = ZW_COLOR_BLUE;
				break;
			case VCR_CLR_YELLOW:
				vi.VehicleColor = ZW_COLOR_YELLOW;
				break;
			case VCR_CLR_GREEN:
				vi.VehicleColor = ZW_COLOR_GREEN;
				break;
			case VCR_CLR_BROWN:
				vi.VehicleColor = ZW_COLOR_BROWN;
				break;
			case VCR_CLR_PINK:
				vi.VehicleColor = ZW_COLOR_PINK;
				break;
			case VCR_CLR_PURPLE:
				vi.VehicleColor = ZW_COLOR_PURPLE;
				break;
			case VCR_CLR_DARKGRAY:
				vi.VehicleColor = ZW_COLOR_DARKGRAY;
				break;
			case VCR_CLR_CYAN:
				vi.VehicleColor = ZW_COLOR_CYAN;
				break;
			default:
				vi.VehicleColor = ZW_COLOR_UNKNOWN;
				break;
			}

			//车身尺寸
			if (struITSPlateResult.byVehicleType == 1
				|| struITSPlateResult.byVehicleType == 2
				|| struITSPlateResult.byVehicleType == 10)
				vi.VehicleSize = ZW_VEHICLE_LARGE;
			else if (struITSPlateResult.byVehicleType == 3
				|| struITSPlateResult.byVehicleType == 4
				|| struITSPlateResult.byVehicleType == 5
				|| struITSPlateResult.byVehicleType == 9)
				vi.VehicleSize = ZW_VEHICLE_SMALL;
			else
				vi.VehicleSize = ZW_VEHICLE_UNKNOWN;

			void *pImage = NULL;
			DWORD   imageSize = 0;

			for (DWORD i = 0;i<struITSPlateResult.dwPicNum;i++)
			{
				//保存场景图
				if ((struITSPlateResult.struPicInfo[i].dwDataLen != 0) && (struITSPlateResult.struPicInfo[i].byType == 1) || (struITSPlateResult.struPicInfo[i].byType == 2))
				{
					imageSize = struITSPlateResult.struPicInfo[i].dwDataLen;
					pImage = struITSPlateResult.struPicInfo[i].pBuffer;
					vi.PlateRect.X = struITSPlateResult.struPicInfo[i].struPlateRect.fX;
					vi.PlateRect.Y = struITSPlateResult.struPicInfo[i].struPlateRect.fY;
					vi.PlateRect.Width = struITSPlateResult.struPicInfo[i].struPlateRect.fWidth;
					vi.PlateRect.Height = struITSPlateResult.struPicInfo[i].struPlateRect.fHeight;
					break;
				}
			}

			pih->mtx.Lock();
			if (pih->OnTriggerImgReady != NULL)
			{
				ZWImageData imgData = { 0 };
				imgData.Width = 0;
				imgData.Height = 0;
				imgData.PixFmt = ZW_PIX_FMT_JPEG;
				imgData.PlaneData[0] = pImage;
				imgData.LineSize[0] = imageSize;
				pih->OnTriggerImgReady(pih->pUserData, &imgData, &vi, ZW_PF_VEHICLE_IDENTIFY);
			}
			pih->mtx.Unlock();

			pih->mtx.Lock();
			if (pih->OnTriggerFixFmtImgReady != NULL && pih->nZwTriggerFixFmt != ZW_PIX_FMT_NONE)
			{
				WriteLog("进入JPG解码过程");
				/* This struct contains the JPEG decompression parameters and pointers to
				* working space (which is allocated as needed by the JPEG library).
				*/
				struct jpeg_decompress_struct cinfo;
				/* We use our private extension JPEG error handler. <CSC> */
				struct jpg_error_mgr jerr;
				/* Step 1: allocate and initialize JPEG decompression object */
				/* We set up the normal JPEG error routines, then override error_exit. */
				cinfo.err = jpeg_std_error(&jerr.pub);
				jerr.pub.error_exit = ima_jpeg_error_exit;

				/* Establish the setjmp return context for my_error_exit to use. */
				if (setjmp(jerr.setjmp_buffer)) {
					/* If we get here, the JPEG code has signaled an error.
					* We need to clean up the JPEG object, close the input file, and return.
					*/
					jpeg_destroy_decompress(&cinfo);

					WriteLog("JPG解码失败结束");
				}
				else {
					/* Now we can initialize the JPEG decompression object. */
					jpeg_create_decompress(&cinfo);
					/* Step 2: specify data source (eg, a file) */
					jpeg_mem_src(&cinfo, (unsigned char*)pImage, imageSize);

					/* Step 3: read file parameters with jpeg_read_header() */
					(void)jpeg_read_header(&cinfo, TRUE);

					/* Step 4 <chupeev> handle decoder options*/
					cinfo.jpeg_color_space = JCS_RGB;
					cinfo.scale_num = 1;
					cinfo.scale_denom = 1;

					/* Step 5: Start decompressor */
					jpeg_start_decompress(&cinfo);

					int row_stride = cinfo.output_width * cinfo.output_components;
					if (pih->pTriggerFixFmtImgBuffer == NULL) {
						pih->pTriggerFixFmtImgBuffer = (char*)malloc(row_stride*cinfo.output_height);
						if (pih->pTriggerFixFmtImgBuffer == NULL)
							longjmp(jerr.setjmp_buffer, 1);
					}

					JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)
						((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

					char *pTriggerBuffer = pih->pTriggerFixFmtImgBuffer;

					/* Step 6: while (scan lines remain to be read) */
					/*           jpeg_read_scanlines(...); */
					/* Here we use the library's state variable cinfo.output_scanline as the
					* loop counter, so that we don't have to keep track ourselves.
					*/
					while (cinfo.output_scanline < cinfo.output_height) {
						(void)jpeg_read_scanlines(&cinfo, buffer, 1);
						memcpy_s(pTriggerBuffer, row_stride, buffer, row_stride);
						pTriggerBuffer += row_stride;
					}

					/* Step 7: Finish decompression */
					(void)jpeg_finish_decompress(&cinfo);

					/* Step 8: Release JPEG decompression object */
					/* This is an important step since it will release a good deal of memory. */
					jpeg_destroy_decompress(&cinfo);

					WriteLog("JPG解码成功结束");

					if (pih->nZwTriggerFixFmt == ZW_PIX_FMT_RGB24)
					{
						WriteLog("输出触发图片");

						ZWImageData imgData = { 0 };
						imgData.Width = cinfo.output_width;
						imgData.Height = cinfo.output_height;
						imgData.PixFmt = pih->nZwTriggerFixFmt;
						imgData.PlaneData[0] = pih->pTriggerFixFmtImgBuffer;
						imgData.LineSize[0] = row_stride;
						pih->OnTriggerFixFmtImgReady(pih->pUserData, &imgData, &vi, ZW_PF_VEHICLE_IDENTIFY);
					}
					else 
					{
						WriteLog("转换JPG解码格式");
						int swap;
						enum AVPixelFormat srcFormat;
						enum AVPixelFormat dstFormat;

						GetPixelFormat(ZW_PIX_FMT_RGB24, pih->nZwTriggerFixFmt, &srcFormat, &dstFormat, &swap);

						if (pih->pTriggerFixFmtCtx == NULL && pih->nZwTriggerFixFmt != ZW_PIX_FMT_RGB24)
						{
							pih->pTriggerFixFmtCtx = sws_getContext(cinfo.output_width,
								cinfo.output_height,
								srcFormat,
								cinfo.output_width,
								cinfo.output_height,
								dstFormat,
								SWS_BICUBIC | SWS_FAST_FLAGS,
								NULL, NULL, NULL);
						}

						if (pih->pTriggerFixFmtCtx != NULL)
						{
							AVPicture avSrc = { 0 };
							AVPicture avDst = { 0 };
							avSrc.data[0] = (uint8_t*)(pih->pTriggerFixFmtImgBuffer);
							avSrc.linesize[0] = row_stride;

							if (pih->pTriggerFixFmtImgSwsBuffer == NULL)
							{
								int numBytes = avpicture_get_size(dstFormat, cinfo.output_width, cinfo.output_height);
								pih->pTriggerFixFmtImgSwsBuffer = (char*)malloc(numBytes);
							}

							if (pih->pTriggerFixFmtImgSwsBuffer != NULL)
							{
								avpicture_fill(&avDst, (uint8_t*)(pih->pTriggerFixFmtImgSwsBuffer), dstFormat, cinfo.output_width, cinfo.output_height);
								if (sws_scale(pih->pTriggerFixFmtCtx, avSrc.data, avSrc.linesize, 0, cinfo.output_height, avDst.data, avDst.linesize) > 0)
								{
									ZWImageData imgData = { 0 };
									imgData.Width = cinfo.output_width;
									imgData.Height = cinfo.output_height;
									imgData.PixFmt = pih->nZwTriggerFixFmt;
									imgData.PlaneData[0] = avDst.data[0];
									imgData.LineSize[0] = avDst.linesize[0];

									if (swap == 1)
									{
										imgData.PlaneData[1] = avDst.data[1];
										imgData.LineSize[1] = avDst.linesize[1];
										imgData.PlaneData[2] = avDst.data[2];
										imgData.LineSize[2] = avDst.linesize[2];
									}
									else
									{
										imgData.PlaneData[1] = avDst.data[2];
										imgData.LineSize[1] = avDst.linesize[2];
										imgData.PlaneData[2] = avDst.data[1];
										imgData.LineSize[2] = avDst.linesize[1];
									}
									WriteLog("输出触发图片");
									pih->OnTriggerFixFmtImgReady(pih->pUserData, &imgData, &vi, ZW_PF_VEHICLE_IDENTIFY);
								}
								else
								{
									WriteLog("转换JPG解码结果失败");
								}
							}
							else
							{
								WriteLog("申请解码内存失败");
							}
						}
						else
						{
							WriteLog("创建JPG解码器失败");
						}

					}//指定的触发输出图片格式不是RGB
				}//jpg解码成功
			}//需要输出指定格式的触发图片
			pih->mtx.Unlock();
		}
	}

	return TRUE;
}

internal_handle* __cdecl  InternalInitial(ZWVideoParam *param, unsigned long addr)
{
	int nRet = 1;
	internal_handle *pih = new internal_handle();

	long count = InterlockedIncrement(&g_nInternal_UserNum);

	//初始化服务
	if (1 == count)
	{
		nRet = NET_DVR_Init();
	}

	char ipaddr[16][16] = { 0 };
	DWORD num;
	BOOL  bind;
	DWORD  i = 0;
	NET_DVR_GetLocalIP(ipaddr, &num, &bind);

	for (i = 0; i < num; i++)
	{
		if (strcmp(ipaddr[i], param->LocalIp) == 0)
		{
			NET_DVR_SetValidIP(i, TRUE);
			break;
		}
	}

	if (0 != nRet)
	{
		//用户登录
		NET_DVR_DEVICEINFO_V30 struDeviceInfo;
		memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));
		pih->lUserHandle = NET_DVR_Login_V30(param->CameraIp, param->CameraPort, param->UserName, param->Password, &struDeviceInfo);
		if (pih->lUserHandle >= 0) 
		{
			memcpy(&(pih->param), &param, sizeof(param));
			return pih;
		}
	}

	count = InterlockedDecrement(&g_nInternal_UserNum);
	if (0 == count)
	{
		NET_DVR_Cleanup();
	}

	delete pih;
	return NULL;
}


void   __cdecl  InternalFinialize(internal_handle* pih)
{
	StopVideoSource(pih);

	if(pih->lUserHandle >= 0)
		NET_DVR_Logout_V30(pih->lUserHandle);

	long count = InterlockedDecrement(&g_nInternal_UserNum);
	if (0 == count)
	{
		NET_DVR_Cleanup();
	}

	if (pih->pRealFixFmtCtx != NULL)
		sws_freeContext(pih->pRealFixFmtCtx);

	if (pih->pRealFixFmtImgBuffer != NULL)
		free(pih->pRealFixFmtImgBuffer);

	if (pih->pTriggerFixFmtCtx != NULL)
		sws_freeContext(pih->pTriggerFixFmtCtx);

	if (pih->pTriggerFixFmtImgBuffer != NULL)
		free(pih->pTriggerFixFmtImgBuffer);

	if (pih->pTriggerFixFmtImgSwsBuffer != NULL)
		free(pih->pTriggerFixFmtImgSwsBuffer);

	delete pih;
}

/*初始化视频源，传入回调用于获取码流和外部触发抓拍的图片*/
ZW_VIDEO_API handle __cdecl  InitialVideoSource(ZWVideoParam *param,
	                                         VideoStreamCallback OnVideoStreamReady,/*实时视频流回调*/
	                                         RealTimeImageCallback OnRealImgReady,/*实时图像输出回调*/
	                                         RealTimeImageCallback OnRealFixFmtImgReady,/*实时转码图像输出回调*/
	                                         int realFixFmt,/*实时转码图像的格式*/
	                                         TriggerImageCallback OnTriggerImgReady,/*外部触发抓拍图片回调*/
	                                         TriggerImageCallback OnTriggerFixFmtImgReady,/*外部触发抓拍转码图片回调*/
	                                         int triggerFixFmt,/*外部触发抓拍转码图片的格式*/
											 void *pUserData)
{
	assert(realFixFmt < 55);
	assert(triggerFixFmt < 55);
	internal_handle *pih = NULL;
	unsigned long addr = inet_addr(param->CameraIp);
	char szipcode[20] = { 0 };
	sprintf_s(szipcode, 20, "%08X%02d", addr,param->AddressNum);
	std::string ipcode(szipcode);

	g_mtx.Lock();
	//g_handle_map必须互斥访问
	std::map<std::string, internal_handle*>::iterator pos = g_handle_map.find(ipcode);
	if (pos != g_handle_map.end())
	{
		pih = pos->second;
	}

	if (pih == NULL)
	{
		//这里加锁的意义是为了方式第一个初始化调用还未完成，也就是还未出这个锁区域
		//第二个初始化调用跳过这个区域直接执行后面的代码
		pih = InternalInitial(param, addr);
		if (pih != NULL) 
		{
			pih->ipcode = ipcode;
			g_handle_map.insert(std::make_pair(ipcode, pih));
		}
	}
	g_mtx.Unlock();

	if (pih != NULL)
	{
		InterlockedIncrement(&(pih->nRefCount));
		pih->mtx.Lock();
		pih->OnVideoStreamReady = OnVideoStreamReady;
		pih->OnRealImgReady = OnRealImgReady;
		pih->OnRealFixFmtImgReady = OnRealFixFmtImgReady;
		pih->nZwRealFixFmt = realFixFmt;
		pih->OnTriggerImgReady = OnTriggerImgReady;
		pih->OnTriggerFixFmtImgReady = OnTriggerFixFmtImgReady;
		pih->nZwTriggerFixFmt = triggerFixFmt;
		pih->pUserData = pUserData;
		pih->mtx.Unlock();
	}

	return (handle)pih;
}


/*获取视频源编码，返回ZW_CODEC_ID_? */
ZW_VIDEO_API int   __cdecl  GetVideoCodec(handle hVS)
{
	 return ZW_CODEC_ID_NONE;
}

/*返回设备是否能自动重连。支持的话返回1，否则返回0*/
ZW_VIDEO_API int    __cdecl  CanReConnect(handle hVS)
{
	return 1;
}

/*触发功能*/
ZW_VIDEO_API int    __cdecl  TriggerPeripheral(handle hVS, int func)
{
	internal_handle *pih = (internal_handle*)hVS;
	NET_DVR_BARRIERGATE_CFG gateCfg = {0};
	gateCfg.dwSize = sizeof(NET_DVR_BARRIERGATE_CFG);
	gateCfg.dwChannel = pih->lChannel;
	gateCfg.byLaneNo = 1;
	switch (func) 
	{
	case ZW_PF_OPEN_GATE:
		gateCfg.byBarrierGateCtrl = 1;
		NET_DVR_RemoteControl(pih->lUserHandle, NET_DVR_BARRIERGATE_CTRL, &gateCfg, gateCfg.dwSize);
		break;
	case ZW_PF_CLOSE_GATE:
		gateCfg.byBarrierGateCtrl = 0;
		NET_DVR_RemoteControl(pih->lUserHandle, NET_DVR_BARRIERGATE_CTRL, &gateCfg, gateCfg.dwSize);
		break;
	}

	return 0;
}

/*开启视频源，返回0失败，1成功*/
ZW_VIDEO_API int    __cdecl  StartVideoSource(handle hVS, handle hPlayWnd)
{
	internal_handle *pih = (internal_handle*)hVS;

	if (pih->lRealPlayHandle >= 0)
		return 0;

	NET_DVR_PREVIEWINFO PreviewInfo = { 0 };
	PreviewInfo.lChannel = pih->lChannel;
	PreviewInfo.dwStreamType = 0;
	PreviewInfo.dwLinkMode = 0;
	PreviewInfo.hPlayWnd = (HWND)hPlayWnd;
	PreviewInfo.bBlocked = 0;
	PreviewInfo.bPassbackRecord = 0;
	PreviewInfo.byPreviewMode = 0;
	PreviewInfo.byProtoType = 0;
	PreviewInfo.dwDisplayBufNum = 0;

	pih->lRealPlayHandle = NET_DVR_RealPlay_V40(pih->lUserHandle, &PreviewInfo, NULL, NULL);

	if (pih->lRealPlayHandle >= 0)
	{
		NET_DVR_SetRealDataCallBack(pih->lRealPlayHandle, g_RealDataCallBack_V30, (DWORD)pih);

		NET_DVR_SetDVRMessCallBack_EX(g_MSGCallBack_EX);

		//启用布防
		NET_DVR_SETUPALARM_PARAM struSetupParam = { 0 };
		struSetupParam.dwSize = sizeof(NET_DVR_SETUPALARM_PARAM);

		//上传报警信息类型: 0- 老报警信息(NET_DVR_PLATE_RESULT), 1- 新报警信息(NET_ITS_PLATE_RESULT)
		struSetupParam.byAlarmInfoType = 1;
		//布防等级:二级布防，针对智能交通设备
		struSetupParam.byLevel = 1;
		struSetupParam.bySupport = 0;
		struSetupParam.byBrokenNetHttp = 0;

		pih->lAlarmHandle = NET_DVR_SetupAlarmChan_V41(pih->lUserHandle, &struSetupParam);

		if (pih->lAlarmHandle >= 0)
		{
			return 1;
		}
		else
		{
			NET_DVR_StopRealPlay(pih->lRealPlayHandle);
			pih->lRealPlayHandle = -1;
		}
	}

	return 0;
}

/*暂停视频源，调用该函数后仍然可以调用StartVideoSource开启*/
ZW_VIDEO_API void   __cdecl  StopVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;
	if(pih->lPort >= 0)
	{
		PlayM4_Stop(pih->lPort);
		PlayM4_CloseStream(pih->lPort);
		pih->lPort = -1;
	}

	if (pih->lAlarmHandle >= 0)
	{
		NET_DVR_CloseAlarmChan_V30(pih->lAlarmHandle);
		pih->lAlarmHandle = -1;
	}

	if (pih->lRealPlayHandle >= 0)
	{
		NET_DVR_StopRealPlay(pih->lRealPlayHandle);
		pih->lRealPlayHandle = -1;
	}
}

/*终止视频源模块*/
ZW_VIDEO_API void   __cdecl  FinializeVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;
	long count = InterlockedDecrement(&(pih->nRefCount));
	if (count != 0)
		return;

	g_mtx.Lock();
	g_handle_map.erase(pih->ipcode);
	g_mtx.Unlock();

	InternalFinialize(pih);
}



/*返回用于PTZ控制的句柄*/
ZW_PTZ_API handle __cdecl InitialPTZ(ZWPTZParam* ptzParam, ZWVideoParam* videoParam)
{
	ZWVideoParam vparam;
	if (videoParam != NULL)
	{
		memcpy(&vparam, videoParam, sizeof(ZWVideoParam));
	}
	else if (ptzParam != NULL)
	{
		vparam.CameraPort = ptzParam->DevicePort;
		memcpy(vparam.CameraIp, ptzParam->DeviceIp, ZW_LENGTH_IP_ADDR);
		memcpy(vparam.LocalIp, ptzParam->LocalIp, ZW_LENGTH_IP_ADDR);
		vparam.AddressNum = ptzParam->Address;
		memcpy(vparam.UserName, ptzParam->UserName, ZW_LENGTH_USER_NAME);
		memcpy(vparam.Password, ptzParam->Password, ZW_LENGTH_PASSWORD);
	}
	else
	{
		return NULL;
	}

	return InitialVideoSource(&vparam, NULL, NULL, NULL, ZW_PIX_FMT_NONE, NULL, NULL, ZW_PIX_FMT_NONE, NULL);
}


/*执行PTZ指令。param1-pan speed和其它操作的speed，param2-tilt speed
**pan和tilt的speed为1-64，其它操作的speed为1-8
**其它操作如果不支持speed直接忽略
**当cmd为VE_ZOOM_STEP时，param1为负值表示从当前位置VE_ZOOM_OUT多少级别，正值表示VE_ZOOM_IN多少级别
**
**
**返回 1-表示执行成功 0-表示执行失败 -1-表示不支持的操作
*/
ZW_PTZ_API int   __cdecl ExecPTZ(handle hPTZ, int cmd, int param1, int param2)
{
	internal_handle *pih = (internal_handle*)hPTZ;
	int speed = (param1 + 8) / 9;
	int ret = 1;

	switch (cmd)
	{
	case ZW_TILT_UP:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, TILT_UP, 0, speed);
		break;
	case ZW_TILT_DOWN:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, TILT_DOWN, 0, speed);
		break;
	case ZW_PAN_LEFT:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, PAN_LEFT, 0, speed);
		break;
	case ZW_PAN_RIGHT:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, PAN_RIGHT, 0, speed);
		break;
	case ZW_LEFTUP:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, UP_LEFT, 0, speed);
		break;
	case ZW_LEFTDOWN:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, DOWN_LEFT, 0, speed);
		break;
	case ZW_RIGHTUP:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, UP_RIGHT, 0, speed);
		break;
	case ZW_RIGHTDOWN:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, DOWN_RIGHT, 0, speed);
		break;
	case ZW_ZOOM_OUT:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, ZOOM_OUT, 0, speed);
		break;
	case ZW_ZOOM_IN:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, ZOOM_IN, 0, speed);
		break;
	case ZW_ZOOM_STEP:
		ret = -1;
		break;
	case ZW_FOCUS_NEAR:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, FOCUS_NEAR, 0, speed);
		break;
	case ZW_FOCUS_FAR:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, FOCUS_FAR, 0, speed);
		break;
	case ZW_IRIS_OPEN:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, IRIS_OPEN, 0, speed);
		break;
	case ZW_IRIS_CLOSE:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, IRIS_CLOSE, 0, speed);
		break;
	case ZW_SET_PRESET:
		ret = NET_DVR_PTZPreset_Other(pih->lUserHandle, pih->lChannel, SET_PRESET, param1);
		break;
	case ZW_USE_PRESET:
		ret = NET_DVR_PTZPreset_Other(pih->lUserHandle, pih->lChannel, GOTO_PRESET, param1);
		break;
	case ZW_DEL_PRESET:
		ret = NET_DVR_PTZPreset_Other(pih->lUserHandle, pih->lChannel, CLE_PRESET, param1);
		break;
	case ZW_STOP_PAN:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, PAN_RIGHT, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, PAN_LEFT, 1, speed);
		break;
	case ZW_STOP_TILT:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, TILT_UP, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, TILT_DOWN, 1, speed);
		break;
	case ZW_STOP_ZOOM:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, ZOOM_IN, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, ZOOM_OUT, 1, speed);
		break;
	case ZW_STOP_FOCUS:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, FOCUS_NEAR, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, FOCUS_FAR, 1, speed);
		break;
	case ZW_STOP_IRIS:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, FOCUS_NEAR, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, FOCUS_FAR, 1, speed);
		break;
	case ZW_STOP_ALL:
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, PAN_RIGHT, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, PAN_LEFT, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, TILT_UP, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, TILT_DOWN, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, ZOOM_IN, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, ZOOM_OUT, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, FOCUS_NEAR, 1, speed);
		ret = NET_DVR_PTZControlWithSpeed_Other(pih->lUserHandle, pih->lChannel, FOCUS_FAR, 1, speed);
		break;
	case ZW_LIGHT_ON:
		ret = -1;
		break;
	case ZW_LIGHT_OFF:
		ret = -1;
		break;
	case ZW_WIPER_ON:
		ret = -1;
		break;
	case ZW_WIPER_OFF:
		ret = -1;
		break;
	default:
		ret = -1;
		break;
	}

	return ret;
}

/*返回设备是否支持3D定位。支持的话返回1，否则返回0*/
ZW_PTZ_API int    __cdecl CanDirectTo(handle hPTZ)
{
	return 1;
}



/*调用3D定位功能。
**zoom 表示3D定位的时候是否执行ZOOM操作，0表示不执行，1表示执行
**vw、vh 视频播放区域的宽度、高度
**rx、ry、rw、rh   3D定位功能框选框的x坐标、y坐标、宽度、高度
**3D定位功能框选框的坐标系的定定义如下：
**坐标原点在视频播放区域的左上角，y坐标向下，x坐标向右
**
**
**返回 1-表示执行成功 0-表示执行失败
*/
ZW_PTZ_API int   __cdecl DirectTo(handle hPTZ, int zoom, int vw, int vh, int rx, int ry, int rw, int rh)
{
	/*控制云台设备完成3D定位*/
	internal_handle *pih = (internal_handle*)hPTZ;

	NET_DVR_POINT_FRAME ptf;
	ptf.xTop = rx * 255 / vw;
	ptf.yTop = ry * 255 / vh;
	ptf.xBottom = (rx + rw) * 255 / vw;
	ptf.yBottom = (ry + rh) * 255 / vh;

	int vw2 = vw / 2;
	int vh2 = vh / 2;
	int rx2 = rx + rw / 2;
	int ry2 = ry + rh / 2;

	if (rx2 > vw2)
	{
		if (ry2 > vh2)
			ptf.bCounter = 4;
		else
			ptf.bCounter = 2;
	}
	else
	{
		if (ry2 > vh2)
			ptf.bCounter = 3;
		else
			ptf.bCounter = 1;
	}


	int nRet = NET_DVR_PTZSelZoomIn_EX(pih->lUserHandle, pih->lChannel, &ptf);

	return nRet;
}

/*销毁用于PTZ控制的句柄*/
ZW_PTZ_API void   __cdecl FinializePTZ(handle hPTZ)
{
	FinializeVideoSource(hPTZ);
}


