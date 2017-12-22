extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
};


#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"swscale.lib")


const int SWS_FAST_FLAGS = SWS_CPU_CAPS_MMX | SWS_CPU_CAPS_MMX2 | SWS_CPU_CAPS_3DNOW | SWS_CPU_CAPS_ALTIVEC | SWS_CPU_CAPS_BFIN ;

#include "export_jsnp.h"
#include <windows.h>
#include <winsock2.h>
#include <map>
#include <string>
#include "Video.h"
#include "PTZ.h"
#include "ThreadMutex.h"

#pragma comment(lib,"JSNetProtocol.lib")

struct internal_handle
{
	StreamCallback OnStreamReady;/*实时视频流回调*/
	PixBufCallback OnPixBufReady;/*解码数据输出回调*/
	RealTimeRGBCallback OnRealRGBReady;  /*实时解码图片回调*/
	TriggerImageCallback OnTriggerRGBReady;  /*外部触发抓拍图片回调*/
	bool  bRaisCallback;
	void* pUserData;
	int   nRefCount;
	std::string ipcode;

	SwsContext* pCtx;
	char* pRGBBuffer;
	long lUserHandle; //用户句柄

	internal_handle()
	{
		OnStreamReady = NULL;
		OnPixBufReady = NULL;
		OnRealRGBReady = NULL;
		OnTriggerRGBReady = NULL;
		pUserData = NULL;
		bRaisCallback = false;
		nRefCount = 0;

		pCtx = NULL;
		pRGBBuffer = NULL;
		lUserHandle = -1;
	}
};

ThreadMutex  g_mtx;
std::map<std::string,internal_handle*> g_handle_map;
int g_nInternal_UserNum = 0; //用户数量

/*实时视频流回调*/
long CALLBACK iveInternal_StreamReadyCB( long lHandle, const char * pBuf, long  nSize, RAW_INFO * pDataInfo, long lParam  )
{
	internal_handle *pih = (internal_handle*)lParam;

	if(pih == NULL || !pih->bRaisCallback)
	{
		return -1;
	}
	//1			CODEC_ID_H264				H264视频编码数据
	//2			CODEC_ID_MPEG4				MPEG4视频编码数据
	//3			CODEC_ID_MJPEG				MJPEG视频编码数据
	if(pDataInfo->nType == 1
		|| pDataInfo->nType == 2
		|| pDataInfo->nType == 3)
	{
		int nFrametype = 3;
		if (0 == pDataInfo->nVideoIFrame) //p
		{
			nFrametype = 3;
		}
		else //I
		{
			nFrametype = 1;
		}
		if(pih->OnStreamReady != NULL)
		{
			pih->OnStreamReady(pih->pUserData,(void*)pBuf,nSize,nFrametype);
		}
	}

	return 0;
}

/*实时解码RGB回调*/
long CALLBACK iveInternal_RealRGBReadyCB( long hHandle, const char * pBuf, long  nSize, FRAME_INFO * pFrameInfo, long lParam  )
{
	internal_handle *pih = (internal_handle*)lParam;

	if(!pih->bRaisCallback)
	{
		return 0;
	}

	//2	RGB32		视频。		每像素4字节，排列方式与位图相似，"BGR0",第一个像素位于图像左下角
	//3	YV12		视频，		yv12格式。排列顺序"Y0Y1……"、"V0V1……"、"U0-U1……"
	//4	UYVY		视频，		uyvy格式。排列顺序"U0Y0V0Y1U2Y2V2Y3 … …"，第一个像素位于图像左上角
	//5	YUV420		视频，		YUV420格式。排列顺序"Y0Y1……"、"U0-U1……"、"V0V1……"
	//6	yuy2、yuyv	视频，		yuy2或yuyv格式。排列顺序"Y0 U0 Y1 V0 Y2 U2 Y3 V2… …"，第一个像素位于图像左上角。
	switch(pFrameInfo->nType)
	{
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:/*注释来自厂家文档，文档有误。5实际上是yv12*/
		if(pih->OnPixBufReady != NULL)
		{
			pih->OnPixBufReady(pih->pUserData,(void*)pBuf,ZW_PIX_FMT_YUV420P,pFrameInfo->nWidth,0,pFrameInfo->nHeight);
		}
		if(pih->OnRealRGBReady != NULL)
		{
			if(pih->pCtx == NULL)
			{
				pih->pCtx = sws_getContext(pFrameInfo->nWidth,
					pFrameInfo->nHeight,
					AV_PIX_FMT_YUV420P,
					pFrameInfo->nWidth,
					pFrameInfo->nHeight,
					AV_PIX_FMT_BGR24,
					SWS_BICUBIC | SWS_FAST_FLAGS,
					NULL,NULL,NULL);
			}
			if(pih->pCtx != NULL)
			{
				if(pih->pRGBBuffer == NULL)
					pih->pRGBBuffer = new char[3*pFrameInfo->nWidth*pFrameInfo->nHeight];			

				AVPicture avSrc;
				avpicture_fill(&avSrc,(uint8_t*)pBuf,AV_PIX_FMT_YUV420P,pFrameInfo->nWidth,pFrameInfo->nHeight);

				AVPicture avDst;
				avpicture_fill(&avDst,(uint8_t*)pih->pRGBBuffer,AV_PIX_FMT_RGB24,pFrameInfo->nWidth,pFrameInfo->nHeight);

				if(sws_scale(  pih->pCtx,avSrc.data,avSrc.linesize,0,pFrameInfo->nHeight,avDst.data,avDst.linesize)>0)
					pih->OnRealRGBReady(pih->pUserData,pih->pRGBBuffer,pFrameInfo->nWidth,pFrameInfo->nWidth*3,pFrameInfo->nHeight);
			}
		}
		break;
	case 6:
		break;
	}

	return 0;
}

internal_handle* __cdecl  InternalInitial(VideoParam *param,unsigned long addr,std::string& ipcode)
{
	internal_handle *pih = new internal_handle();
	int nRet = 0;

	//初始化服务
	if (0 == g_nInternal_UserNum)
	{
		nRet = NP_Initial();
		if (0 != nRet)
		{
			delete pih;
			return NULL;
		}
	}

	if(strcmp(param->AddressName,"") == 0)
		memcpy(param->AddressName,"1/1080p",8);

	//用户登录
	nRet = NP_StartPreview(param->CameraIp, param->AddressName, param->CameraPort, 1, param->UserName, param->Password, (long)0,(long)0, &(pih->lUserHandle));
	if (0 != nRet)
	{
		delete pih;
		return NULL;
	}

	//音视频解码数据回调设置
	nRet = NP_SetDecodeCallBack(iveInternal_RealRGBReadyCB,(long)pih, pih->lUserHandle);
	//音视频原始码流回调设置
	nRet |= NP_SetRawDataCallBack(iveInternal_StreamReadyCB,(long)pih, pih->lUserHandle);
	if (0 != nRet)
	{
		delete pih;
		return NULL;
	}

	g_nInternal_UserNum++;
	
	pih->ipcode = ipcode;
	g_mtx.Lock();
	g_handle_map.insert(std::make_pair(ipcode,pih));
	g_mtx.Unlock();
	return pih;
}

void   __cdecl  InternalFinialize(internal_handle* pih)
{
	pih->nRefCount--;
	if(pih->nRefCount != 0)
		return;

	NP_StopPrevew(pih->lUserHandle);
	if (--g_nInternal_UserNum == 0) //连接用户个数为0才释放服务
	{
		NP_Free();
	}

	if(pih->pCtx != NULL)
	{
		sws_freeContext(pih->pCtx);
	}
	if(pih->pRGBBuffer != NULL)
	{
		delete [] pih->pRGBBuffer;
	}

	g_mtx.Lock();
	g_handle_map.erase(pih->ipcode);
	g_mtx.Unlock();
	delete pih;
}

/*初始化视频源，传入回调用于获取码流和外部触发抓拍的图片*/
ZW_VIDEO_API handle __cdecl  InitialVideoSource(VideoParam *param,
											 StreamCallback OnStreamReady,/*实时视频流回调*/
											 PixBufCallback OnPixBufReady,/*解码数据输出回调*/
											 RealTimeRGBCallback OnRealRGBReady,/*实时解码图片回调*/
											 TriggerImageCallback OnTriggerRGBReady,/*外部触发抓拍图片回调*/
											 void *pUserData)
{
	internal_handle *pih = NULL;
	unsigned long addr = inet_addr(param->CameraIp);
	char szipcode[10]={0};
	sprintf_s(szipcode,10,"%08X",addr);
	std::string ipcode = szipcode;
	g_mtx.Lock();
	std::map<std::string,internal_handle*>::iterator pos = g_handle_map.find(ipcode);
	if(pos != g_handle_map.end())
	{
		pih = pos->second;
	}
	g_mtx.Unlock();	

	if(pih == NULL)
	{
		pih = InternalInitial(param,addr,ipcode);
	}

	if(pih != NULL)
	{
		pih->nRefCount++;
		pih->OnStreamReady = OnStreamReady;
		pih->OnPixBufReady = OnPixBufReady;
		pih->OnRealRGBReady = OnRealRGBReady;
		pih->OnTriggerRGBReady = OnTriggerRGBReady;
		pih->pUserData = pUserData;
	}

	return (handle)pih;
}


/*获取视频源信息，返回0失败，1成功
** decode = 0表示由外部解码，由外部进行解码的话OnRealRGBReady不会被调用。此时codec的值必须有效。
** decode = 1表示由内部解码，内部解码出RGB图像通过OnRealRGBReady传出。
*/
ZW_VIDEO_API int   __cdecl  GetVideoInfo(handle hVS,int *decode,int *codec)
{
	internal_handle *pih = (internal_handle*)hVS;

	*decode = 1;
	*codec = ZW_CODEC_ID_NONE;

	return 1;	
}

/*返回设备是否能自动重连。支持的话返回1，否则返回0*/
ZW_VIDEO_API int    __cdecl  CanReConnect(handle hVS)
{
	return 0;
}

/*开启视频源，返回0失败，1成功*/
ZW_VIDEO_API int    __cdecl  StartVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;
	pih->bRaisCallback = true;
	return 1;
}

/*暂停视频源，调用该函数后仍然可以调用StartVideoSource开启*/
ZW_PTZ_LOCAL void   __cdecl  StopVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;
	pih->bRaisCallback = false;
}

/*终止视频源模块*/
ZW_VIDEO_API void   __cdecl  FinializeVideoSource(handle hVS)
{
	InternalFinialize((internal_handle*)hVS);
}



/*返回用于PTZ控制的句柄*/
ZW_PTZ_API handle __cdecl InitialPTZ(PTZParam* ptzParam, VideoParam* videoParam)
{
	VideoParam vparam;
	if (videoParam != NULL)
	{
		memcpy(&vparam, videoParam, sizeof(VideoParam));
	}
	else if (ptzParam != NULL)
	{
		vparam.CameraPort = ptzParam->DevicePort;
		memcpy(vparam.CameraIp, ptzParam->DeviceIp, ZW_IP_ADDR_LENGTH);
		vparam.AddressNum = ptzParam->Address;
		memcpy(vparam.UserName, ptzParam->UserName, ZW_USER_NAME_LENGTH);
		memcpy(vparam.Password, ptzParam->Password, ZW_PASSWORD_LENGTH);
	}
	else
	{
		return NULL;
	}

	internal_handle *pih = NULL;
	unsigned long addr = inet_addr(vparam.CameraIp);
	char szipcode[10]={0};
	sprintf_s(szipcode,10,"%08X",addr);
	std::string ipcode = szipcode;
	g_mtx.Lock();
	std::map<std::string,internal_handle*>::iterator pos = g_handle_map.find(ipcode);
	if(pos != g_handle_map.end())
	{
		pih = pos->second;
	}
	g_mtx.Unlock();	

	if(pih == NULL)
	{
		pih = InternalInitial(&vparam,addr,ipcode);
	}

	if(pih != NULL)
		pih->nRefCount++;

	return (handle)pih;
}


/*执行PTZ指令。param1-pan speed和其它操作的speed，param2-tilt speed
**pan和tilt的speed为1-64，其它操作的speed为1-8
**其它操作如果不支持speed直接忽略
**当cmd为ZOOM_STEP时，param1为负值表示从当前位置ZOOM_OUT多少级别，正值表示ZOOM_IN多少级别
**
**
**返回 1-表示执行成功 0-表示执行失败 -1-表示不支持的操作
*/
ZW_PTZ_API int   __cdecl ExecPTZ(handle hPTZ,int cmd,int param1,int param2)
{
	long lRet = 65536;
	internal_handle *pih = (internal_handle*)hPTZ;
	switch(cmd)
	{
	case ZW_TILT_UP:
		lRet = NP_PTZControl(NP_PTZ_Up, TRUE, param2, pih->lUserHandle);
		break;
	case ZW_TILT_DOWN:
		lRet = NP_PTZControl(NP_PTZ_Down, TRUE, param2, pih->lUserHandle);
		break;
	case ZW_PAN_LEFT:
		lRet = NP_PTZControl(NP_PTZ_Left, TRUE, param1, pih->lUserHandle);
		break;
	case ZW_PAN_RIGHT:
		lRet = NP_PTZControl(NP_PTZ_Right, TRUE, param1, pih->lUserHandle);
		break;
	case ZW_LEFTUP:
		break;
	case ZW_LEFTDOWN:
		break;
	case ZW_RIGHTUP:
		break;
	case ZW_RIGHTDOWN:
		break;
	case ZW_ZOOM_OUT:
		lRet = NP_PTZControl(NP_PTZ_ZoomReduce, TRUE, param1, pih->lUserHandle);
		break;
	case ZW_ZOOM_IN:
		lRet = NP_PTZControl(NP_PTZ_ZoomAdd, TRUE, param1, pih->lUserHandle);
		break;
	case ZW_ZOOM_STEP:
		break;
	case ZW_FOCUS_NEAR:
		lRet = NP_PTZControl(NP_PTZ_FocusAdd, TRUE, param1, pih->lUserHandle);
		break;
	case ZW_FOCUS_FAR:
		lRet = NP_PTZControl(NP_PTZ_FocusReduce, TRUE, param1, pih->lUserHandle);
		break;
	case ZW_IRIS_OPEN:
		break;
	case ZW_IRIS_CLOSE:
		break;
	case ZW_SET_PRESET:		
		lRet = NP_AddPreset(param1, "", pih->lUserHandle);
		break;
	case ZW_USE_PRESET:
		lRet = NP_CallPreset(param1, pih->lUserHandle);
		break;
	case ZW_DEL_PRESET:
		lRet = NP_DelPreset(param1, pih->lUserHandle);
		break;
	case ZW_STOP_PAN:
		lRet = NP_PTZControl(NP_PTZ_Left, FALSE, param1, pih->lUserHandle);
		lRet &= NP_PTZControl(NP_PTZ_Right, FALSE, param1, pih->lUserHandle);
		break;
	case ZW_STOP_TILT:
		lRet = NP_PTZControl(NP_PTZ_Up, FALSE, param2, pih->lUserHandle);
		lRet &= NP_PTZControl(NP_PTZ_Down, FALSE, param2, pih->lUserHandle);
		break;
	case ZW_STOP_ZOOM:
		lRet = NP_PTZControl(NP_PTZ_ZoomAdd, FALSE, param1, pih->lUserHandle);
		lRet &= NP_PTZControl(NP_PTZ_ZoomReduce, FALSE, param1, pih->lUserHandle);
		break;
	case ZW_STOP_FOCUS:
		lRet = NP_PTZControl(NP_PTZ_FocusReduce, FALSE, param1, pih->lUserHandle);
		lRet &= NP_PTZControl(NP_PTZ_FocusAdd, FALSE, param1, pih->lUserHandle);
		break;
	case ZW_STOP_IRIS:
		lRet = NP_PTZControl(NP_PTZ_IrisAdd, FALSE, param1, pih->lUserHandle);
		lRet &= NP_PTZControl(NP_PTZ_IrisReduce, FALSE, param1, pih->lUserHandle);
		break;
	case ZW_STOP_ALL:
		lRet = NP_PTZControl(NP_PTZ_Left, FALSE, param1, pih->lUserHandle);
		lRet |= NP_PTZControl(NP_PTZ_Up, FALSE, param2, pih->lUserHandle);
		lRet |= NP_PTZControl(NP_PTZ_ZoomAdd, FALSE, param1, pih->lUserHandle);
		lRet |= NP_PTZControl(NP_PTZ_FocusReduce, FALSE, param1, pih->lUserHandle);
		lRet |= NP_PTZControl(NP_PTZ_IrisAdd, FALSE, param1, pih->lUserHandle);
		break;
	case ZW_LIGHT_ON:
		lRet = NP_PTZControl(NP_PTZ_Light, TRUE, param1, pih->lUserHandle);
		break;
	case ZW_LIGHT_OFF:
		lRet = NP_PTZControl(NP_PTZ_Light, FALSE, param1, pih->lUserHandle);
		break;
	case ZW_WIPER_ON:
		lRet = NP_PTZControl(NP_PTZ_Brush, TRUE, param1, pih->lUserHandle);
		break;
	case ZW_WIPER_OFF:
		lRet = NP_PTZControl(NP_PTZ_Brush, FALSE, param1, pih->lUserHandle);
		break;
	default:
		break;
	}

	if(lRet == 0) return 1;
	else if(lRet == 65536) return -1;
	else return 0;
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
ZW_PTZ_API int   __cdecl DirectTo(handle hPTZ,int zoom,int vw,int vh,int rx, int ry, int rw,int rh)
{
	internal_handle *pih = (internal_handle*)hPTZ;
	RECT rt;

	rt.left = rx;
	rt.top = ry;
	rt.right = rt.left + rw;
	rt.bottom = rt.top + rh;
	int nRet = NP_3DPositioning(vw, vh, rt, 1,  pih->lUserHandle);
	if (0 == nRet)
	{
		return 1;
	}

	return 0;
}

/*销毁用于PTZ控制的句柄*/
ZW_PTZ_API void   __cdecl FinializePTZ(handle hPTZ)
{
	InternalFinialize((internal_handle*)hPTZ);
}
