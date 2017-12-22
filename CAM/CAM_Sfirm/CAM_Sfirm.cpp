#include <windows.h>

#include <map>
#include <string>
#include "Video.h"
#include "PTZ.h"
#include "ThreadMutex.h"
#include <assert.h>


#include "Net_Interface.h"
#include "Net_Setup.h"
#pragma comment(lib, "NetSDK.lib")

struct internal_handle
{
	ZWVideoParam param;

	VideoStreamCallback OnVideoStreamReady;/*实时视频流回调*/
	RealTimeImageCallback OnRealImgReady;/*实时图像输出回调*/
	RealTimeImageCallback OnRealFixFmtImgReady;/*实时转码图像输出回调*/
	int nRealFixFmt;/*实时转码图像的格式*/
	TriggerImageCallback OnTriggerImgReady;  /*外部触发抓拍图片回调*/
	TriggerImageCallback OnTriggerFixFmtImgReady;/*外部触发抓拍转码图片回调*/
	int nTriggerFixFmt;/*外部触发抓拍转码图片的格式*/
	void* pUserData;

	DCHANDLE hCam;

	volatile long nRefCount;
	std::string ipcode;

	bool started;
	ThreadMutex mtx;

	internal_handle()
	{
		OnVideoStreamReady = NULL;
		OnRealImgReady = NULL;
		OnRealFixFmtImgReady = NULL;
		nRealFixFmt = ZW_PIX_FMT_NONE;
		OnTriggerImgReady = NULL;
		OnTriggerFixFmtImgReady = NULL;
		nTriggerFixFmt = ZW_PIX_FMT_NONE;
		pUserData = NULL;

		nRefCount = 0;
		hCam = -1;
		started = false;
	}
};

ThreadMutex  g_mtx;
std::map<std::string, internal_handle*> g_handle_map;
volatile long g_nInternal_UserNum = 0;//设备接入SDK初始化计数


internal_handle* __cdecl  InternalInitial(ZWVideoParam *param, unsigned long addr)
{
	internal_handle *pih = new internal_handle();

	memcpy(&(pih->param), param, sizeof(ZWVideoParam));

	long count = InterlockedIncrement(&g_nInternal_UserNum);

	//初始化服务
	if (1 == count)
	{
		Net_Init();
	}
	DCHANDLE hCam = Net_AddCamera(param->CameraIp);
	if (hCam > -1) 
	{
		int ret = Net_ConnCamera(hCam, param->CameraPort, 15);
		if (ret == 0) 
		{
			pih->hCam = hCam;
			Net_RegOffLineClient(pih->hCam);
			return pih;
		}

		Net_DelCamera(hCam);
	}

	count = InterlockedDecrement(&g_nInternal_UserNum);
	if (0 == count)
	{
		Net_UNinit();
	}
	delete pih;
	return NULL;

	return pih;
}

void   __cdecl  InternalFinialize(internal_handle* pih)
{
	StopVideoSource(pih);

	Net_DisConnCamera(pih->hCam);
	Net_DelCamera(pih->hCam);

	long count = InterlockedDecrement(&g_nInternal_UserNum);
	if (0 == count)
	{
		Net_UNinit();
	}

	delete pih;
}

int CALLBACK  RecImageCallBack(DCHANDLE tHandle, unsigned int  uiImageId, T_ImageUserInfo *ptImageInfo, T_PicInfo *ptPicInfo, void *pUser) 
{
	//if (uiImageId == 0) 
	{
		internal_handle *pih = (internal_handle*)pUser;

		ZWVehicleInfo vi = { 0 };
		memcpy(vi.License, ptImageInfo->szLprResult, 16);
		switch (ptImageInfo->ucPlateColor) {
		case E_ColorType::COLOR_TYPE_BLACK:
			vi.PlateColor = ZW_COLOR_BLACK;
			break;
		case E_ColorType::COLOR_TYPE_BLUE:
			vi.PlateColor = ZW_COLOR_BLUE;
			break;
		case E_ColorType::COLOR_TYPE_WHITE:
			vi.PlateColor = ZW_COLOR_WHITE;
			break;
		case E_ColorType::COLOR_TYPE_YELLO:
			vi.PlateColor = ZW_COLOR_YELLOW;
			break;
		default:
			vi.PlateColor = ZW_COLOR_UNKNOWN;
			break;
		}

		switch (ptImageInfo->ucVehicleColor) {
		case E_ColorType::COLOR_TYPE_BLACK:
			vi.VehicleColor = ZW_COLOR_BLACK;
			break;
		case E_ColorType::COLOR_TYPE_BLUE:
			vi.VehicleColor = ZW_COLOR_BLUE;
			break;
		case E_ColorType::COLOR_TYPE_WHITE:
			vi.VehicleColor = ZW_COLOR_WHITE;
			break;
		case E_ColorType::COLOR_TYPE_YELLO:
			vi.VehicleColor = ZW_COLOR_YELLOW;
			break;
		default:
			vi.VehicleColor = ZW_COLOR_UNKNOWN;
			break;
		}

		switch (ptImageInfo->ucVehicleSize)
		{
		case ITS_Tb_Vt::TB_VEHICLE_TYPE_LARGE:
			vi.VehicleSize = ZW_VEHICLE_LARGE;
			break;
		case ITS_Tb_Vt::TB_VEHICLE_TYPE_MEDIUM:
			vi.VehicleSize = ZW_VEHICLE_MEDIUM;
			break;
		case ITS_Tb_Vt::TB_VEHICLE_TYPE_SMALL:
			vi.VehicleSize = ZW_VEHICLE_SMALL;
			break;
		default:
			vi.VehicleSize = ZW_VEHICLE_UNKNOWN;
			break;
		}

		vi.PlateRect.X = ((float)(ptImageInfo->usLpBox[0]) / (float)(ptImageInfo->usWidth));
		vi.PlateRect.Y = ((float)(ptImageInfo->usLpBox[1]) / (float)(ptImageInfo->usHeight));
		vi.PlateRect.Width = abs((float)(ptImageInfo->usLpBox[2]-ptImageInfo->usLpBox[0]) / (float)(ptImageInfo->usWidth));
		vi.PlateRect.Width = abs((float)(ptImageInfo->usLpBox[3] - ptImageInfo->usLpBox[1]) / (float)(ptImageInfo->usHeight));

		ZWImageData imgData = { 0 };
		imgData.Width = ptImageInfo->usWidth;
		imgData.Height = ptImageInfo->usHeight;
		imgData.PixFmt = ZW_PIX_FMT_JPEG;
		imgData.LineSize[0] = ptPicInfo->uiPanoramaPicLen;
		imgData.PlaneData[0] = ptPicInfo->ptPanoramaPicBuff;

		pih->mtx.Lock();
		if (pih->OnTriggerImgReady != NULL)
		{
			pih->OnTriggerImgReady(pih->pUserData, &imgData, &vi, ZW_PF_VEHICLE_IDENTIFY);
		}
		pih->mtx.Unlock();
	}

	return 0;
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
	assert(realFixFmt < 100);
	assert(triggerFixFmt < 100);
	internal_handle *pih = NULL;
	unsigned long addr = inet_addr(param->CameraIp);
	char szipcode[10] = { 0 };
	sprintf_s(szipcode, 10, "%08X", addr);
	std::string ipcode(szipcode);

	g_mtx.Lock();

	std::map<std::string, internal_handle*>::iterator pos = g_handle_map.find(ipcode);
	if (pos != g_handle_map.end())
	{
		pih = pos->second;
	}

	if (pih == NULL)
	{
		pih = InternalInitial(param, addr);
		if (pih != NULL)
		{
			pih->ipcode = ipcode;
			g_handle_map.insert(std::make_pair(ipcode, pih));
		}
	}

	if (pih != NULL)
	{
		InterlockedIncrement(&(pih->nRefCount));
		//对以下变量的修改必须同步
		//若不同步，回调方法可能正在执行，但是这里的指针确被修改了
		//尤其是nRealFixFmt、nTriggerFixFmt、pUserData使用中被修改会导致无法预料的问题
		pih->mtx.Lock();
		pih->OnVideoStreamReady = OnVideoStreamReady;
		pih->OnRealImgReady = OnRealImgReady;
		pih->OnRealFixFmtImgReady = OnRealFixFmtImgReady;
		pih->nRealFixFmt = realFixFmt;
		pih->OnTriggerImgReady = OnTriggerImgReady;
		pih->OnTriggerFixFmtImgReady = OnTriggerFixFmtImgReady;
		pih->nTriggerFixFmt = triggerFixFmt;
		pih->pUserData = pUserData;
		pih->mtx.Unlock();
	}

	g_mtx.Unlock();

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

/*开启视频源，返回0失败，1成功*/
ZW_VIDEO_API int    __cdecl  StartVideoSource(handle hVS, handle hPlayWnd)
{
	internal_handle *pih = (internal_handle*)hVS;
	if (pih->started)
		return 0;

	//DCHANDLE hCam = Net_AddCamera(pih->param.CameraIp);
	//if (hCam > -1)
	//{
	//	int ret = Net_ConnCamera(hCam, (unsigned short)(pih->param.CameraPort), 10);
	//	if (ret == 0)
	//	{
	//		ret = Net_StartVideo(pih->hCam, 0, (HWND)hPlayWnd);
	//		if (ret == 0)
	//		{
	//			Net_RegOffLineClient(pih->hCam);
	//			Net_RegImageRecvEx(pih->hCam, RecImageCallBack, pih);
	//			
	//			pih->started = true;
	//			pih->hCam = hCam;
	//			return 1;
	//		}
	//		Net_DisConnCamera(hCam);
	//	}

	//	Net_DelCamera(hCam);
	//}
	int ret = Net_StartVideo(pih->hCam, 0, (HWND)hPlayWnd);
	if (ret == 0) 
	{
		Net_RegImageRecvEx(pih->hCam, RecImageCallBack, pih);
		pih->started = true;
		return 1;
	}

	return 0;
}

/*触发功能*/
ZW_VIDEO_API int    __cdecl  TriggerPeripheral(handle hVS, int func)
{
	internal_handle *pih = (internal_handle*)hVS;

	if (ZW_PF_TAKE_PICTURE == func && pih->OnTriggerImgReady != NULL)
	{
		unsigned char* jpgBuffer = NULL;
		unsigned long  jpgSize = 0;
		Net_GetJpgBuffer(pih->hCam, &jpgBuffer, &jpgSize);

		if (jpgBuffer != NULL) 
		{
			ZWImageData img = { 0 };
			img.PixFmt = ZW_PIX_FMT_JPEG;
			img.PlaneData[0] = jpgBuffer;
			img.LineSize[0] = jpgSize;
			pih->OnTriggerImgReady(pih->pUserData, &img, jpgBuffer, func);
			Net_FreeBuffer(jpgBuffer);
		}
	}

	return 0;
}

/*暂停视频源，调用该函数后仍然可以调用StartVideoSource开启*/
ZW_VIDEO_API void   __cdecl  StopVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;
	if (pih->started)
	{
		Net_StopVideo(pih->hCam);
		pih->started = false;
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




