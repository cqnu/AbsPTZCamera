extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
};

#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"swscale.lib")


const int SWS_FAST_FLAGS = SWS_CPU_CAPS_MMX | SWS_CPU_CAPS_MMX2 | SWS_CPU_CAPS_3DNOW | SWS_CPU_CAPS_ALTIVEC | SWS_CPU_CAPS_BFIN ;


#include <windows.h>
#include <map>
#include <string>
#include "Video.h"
#include "PTZ.h"
#include "ThreadMutex.h"

#define SYSTEM_WIN32
#include "HIE_MP4Player.h"
#include "HieClientUnit.h"

#pragma comment(lib, "HieClientUnit.lib")
#pragma comment(lib, "HIE_MP4Play.lib")

struct internal_handle
{
	StreamCallback OnStreamReady;/*实时视频流回调*/
	PixBufCallback OnPixBufReady;/*解码数据输出回调*/
	RealTimeRGBCallback OnRealRGBReady;  /*实时解码图片回调*/
	TriggerImageCallback OnTriggerRGBReady;  /*外部触发抓拍图片回调*/
	bool  bRaisCallback;
	void* pUserData;
	int   nRefCount;
	bool  bConnected;
	std::string ipcode;

	SwsContext* pCtx;
	char* pRGBBuffer;
	HUSER hUserHandle;          //用户句柄
	HSTREAM hRealHandle;		//实时流句柄
	DWORD   dwChannel;              //实时流通道
	long lPort;	                //实时流播放通道号 
	ConfigInformation tConfigInfo;

	internal_handle()
	{
		OnStreamReady = NULL;
		OnPixBufReady = NULL;
		OnRealRGBReady = NULL;
		OnTriggerRGBReady = NULL;
		pUserData = NULL;
		bRaisCallback = false;
		nRefCount = 0;
		bConnected = false;

		pCtx = NULL;
		pRGBBuffer = NULL;
		hUserHandle = NULL;
		hRealHandle = NULL;
		dwChannel = 0;
		lPort = NULL;
		memset(&tConfigInfo,0,sizeof(ConfigInformation));
	}
};

long  g_lPlayPort = 0;/*当前可用的解码通道*/
int g_nSdkClientCount = 0;
unsigned char   g_FileHeadBuffer[] = {0x00,0x00,0x01,0xc6,0x4d,0x50,0x47,0x34,0x03,0x00,0x1e,0x80,0x8d,0x5b,0x00,0x00,0x05,0xd0,0x02};
ThreadMutex  g_mtx;
std::map<std::string,internal_handle*> g_handle_map;

int CALLBACK UserEventProc(IN HUSER hUser, IN DWORD dwEventType, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, IN DWORD dwUserData)
{
	internal_handle *pih = (internal_handle*)dwUserData;

	if(dwEventType&USEREVENT_HEARTBEAT_LOST)
		pih->bConnected = false;

	if(dwEventType&USEREVENT_NET_RECOVER )
		pih->bConnected = true;

	return 0;
}

/*实时视频流回调*/
int CALLBACK iveInternal_StreamReadyCB(IN HSTREAM hStream, IN const StreamMediaFrame *cFrame, IN DWORD dwUserData)
{
	internal_handle *pih = (internal_handle*)dwUserData;
	
	if (pih->bRaisCallback && (eFrameIFrames == cFrame->dwFrameType || eFramePFrames == cFrame->dwFrameType))
	{
		//流解码
		MP4Play_InputData(pih->lPort,(BYTE*)cFrame->cFrameBuffer.pBuffer, cFrame->cFrameBuffer.dwBufLen);

		int nFrametype = 3;
		if (eFrameIFrames == cFrame->dwFrameType) //p帧
		{
			nFrametype = 1;
		}

		if (pih->OnStreamReady != NULL)
		{   //标准流回调
			pih->OnStreamReady(pih->pUserData, (void*)cFrame->cFrameBuffer.pBuffer, cFrame->cFrameBuffer.dwBufLen, nFrametype);
		}
	}
	return 0;
}

/*实时解码RGB回调*/
void CALLBACK iveInternal_RealRGBReadyCB(long nPort,char *pBuf,long nSize,FRAME_INFO *pFrameInfo,long lUser,long nReserved2)
{
	if (pFrameInfo->nType == T_AUDIO16)
	{
		return;
	}

	internal_handle *pih = (internal_handle*)lUser;
	
	if(!pih->bRaisCallback)
	{
		return;
	}

	if(pih->OnPixBufReady != NULL)
	{
		pih->OnPixBufReady(pih->pUserData,(void*)pBuf,ZW_PIX_FMT_YUV420P,pFrameInfo->nWidth,0,pFrameInfo->nHeight);
	}

	if(pih->OnRealRGBReady != NULL)
	{
		//YUV转RGB
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
			{
				pih->pRGBBuffer = new char[3*pFrameInfo->nWidth*pFrameInfo->nHeight];
			}		

			AVPicture avSrc;
			avpicture_fill(&avSrc,(uint8_t*)pBuf,AV_PIX_FMT_YUV420P,pFrameInfo->nWidth,pFrameInfo->nHeight);

			AVPicture avDst;
			avpicture_fill(&avDst,(uint8_t*)pih->pRGBBuffer,AV_PIX_FMT_RGB24,pFrameInfo->nWidth,pFrameInfo->nHeight);

			if(sws_scale(  pih->pCtx,avSrc.data,avSrc.linesize,0,pFrameInfo->nHeight,avDst.data,avDst.linesize)>0)
			{   //RGB回调
				pih->OnRealRGBReady(pih->pUserData,pih->pRGBBuffer,pFrameInfo->nWidth,avDst.linesize[0],pFrameInfo->nHeight);
			}
		}
	}
}

internal_handle* __cdecl  InternalInitial(VideoParam *param,unsigned long addr,std::string& ipcode)
{
	int nRet = 0;
	BOOL bRet = FALSE;
	if(g_nSdkClientCount == 0)
	{
		nRet = HieClient_Start();
		if(nRet != 0) return NULL;
	}
		
	internal_handle *pih = new internal_handle();

	UserLoginPara userLoginPara;
	memset(&userLoginPara, 0, sizeof(UserLoginPara));
	memcpy(userLoginPara.sServerIP, param->CameraIp, ZW_IP_ADDR_LENGTH);
	userLoginPara.dwCommandPort = param->CameraPort;
	strcpy_s(userLoginPara.sUName,param->UserName);
	strcpy_s(userLoginPara.sUPass,param->Password);

	nRet = HieClient_UserLogin(&(pih->hUserHandle),&userLoginPara);
	bRet &= HieClient_SubscribeEvent(pih->hUserHandle,USEREVENT_HEARTBEAT_LOST|USEREVENT_NET_RECOVER,UserEventProc,reinterpret_cast<DWORD>(pih));

	if (nRet != 0)
	{
		if (0 == g_nSdkClientCount)
			HieClient_Stop();	
		delete pih;
		return NULL;
	}

	pih->dwChannel = param->AddressNum;
	RealStreamPara tRealStreamPara;						
	memset( &tRealStreamPara, 0, sizeof(RealStreamPara) );
	tRealStreamPara.dwChannel     = param->AddressNum;    //通道 1		
	tRealStreamPara.eMediaType    = eMainVideo;			  //主码流视频
	tRealStreamPara.eTransferMode = eGeneralTCP;          //普通 TCP

	//建立实时流连接
	nRet = HieClient_RealStreamConnect(&pih->hRealHandle, pih->hUserHandle, &tRealStreamPara);	
	if (nRet != 0)
	{
		HieClient_UserLogout(pih->hUserHandle);
		if (0 == g_nSdkClientCount)
			HieClient_Stop();	
		delete pih;
		return NULL;
	}

	//设置流媒体数据回调
	nRet = HieClient_StreamMediaCB(pih->hRealHandle, iveInternal_StreamReadyCB, (DWORD)pih);
	
	if (nRet != 0)
	{
		HieClient_RealStreamDisconnect(pih->hRealHandle);
		HieClient_UserLogout(pih->hUserHandle);
		if (0 == g_nSdkClientCount)
			HieClient_Stop();	
		delete pih;
		return NULL;
	}

	//启用流媒体数据接收
	nRet = HieClient_StreamMediaControl(pih->hRealHandle, eTaskStart);

	if (nRet != 0)
	{
		HieClient_RealStreamDisconnect(pih->hRealHandle);
		HieClient_UserLogout(pih->hUserHandle);
		if (0 == g_nSdkClientCount)
			HieClient_Stop();	
		delete pih;
		return NULL;
	}

	pih->lPort = g_lPlayPort++;

	//创建播放端口
	bRet = MP4Play_LockNewPort(&pih->lPort);

	if (!bRet)
	{
		HieClient_RealStreamDisconnect(pih->hRealHandle);
		HieClient_UserLogout(pih->hUserHandle);
		if (0 == g_nSdkClientCount)
			HieClient_Stop();	
		delete pih;
		return NULL;
	}

	bRet = MP4Play_SetStreamOpenMode(pih->lPort, STREAME_REALTIME);  

	if (!bRet)
	{
		MP4Play_UnlockPort(pih->lPort);
		HieClient_RealStreamDisconnect(pih->hRealHandle);
		HieClient_UserLogout(pih->hUserHandle);
		if (0 == g_nSdkClientCount)
			HieClient_Stop();	
		delete pih;
		return NULL;
	}

	bRet = MP4Play_OpenStream(pih->lPort, g_FileHeadBuffer, 19, SOURCE_BUF_MAX);            //开启视频流

	if (!bRet)
	{
		MP4Play_UnlockPort(pih->lPort);
		HieClient_RealStreamDisconnect(pih->hRealHandle);
		HieClient_UserLogout(pih->hUserHandle);
		if (0 == g_nSdkClientCount)
			HieClient_Stop();	
		delete pih;
		return NULL;
	}

	bRet = MP4Play_SetDecCallBackUser(pih->lPort ,iveInternal_RealRGBReadyCB,(long)pih);	//解码回调函数
	
	if (!bRet)
	{
		MP4Play_CloseStream(pih->lPort);
		MP4Play_UnlockPort(pih->lPort);
		HieClient_RealStreamDisconnect(pih->hRealHandle);
		HieClient_UserLogout(pih->hUserHandle);
		if (0 == g_nSdkClientCount)
			HieClient_Stop();	
		delete pih;
		return NULL;
	}

	bRet = MP4Play_Play(pih->lPort, 0);

	if (!bRet)
	{
		MP4Play_Stop(pih->lPort);
		MP4Play_CloseStream(pih->lPort);
		MP4Play_UnlockPort(pih->lPort);
		HieClient_RealStreamDisconnect(pih->hRealHandle);
		HieClient_UserLogout(pih->hUserHandle);
		if (0 == g_nSdkClientCount)
			HieClient_Stop();	
		delete pih;
		return NULL;
	}

	pih->ipcode = ipcode;
	g_mtx.Lock();
	g_handle_map.insert(std::make_pair(ipcode,pih));
	g_mtx.Unlock();
	pih->bConnected = true;
	g_nSdkClientCount++;
	return pih;
}

void   __cdecl  InternalFinialize(internal_handle* pih)
{
	pih->nRefCount--;
	if(pih->nRefCount != 0)
		return;

	MP4Play_Stop(pih->lPort);
	MP4Play_CloseStream(pih->lPort);
	MP4Play_UnlockPort(pih->lPort);
	HieClient_RealStreamDisconnect(pih->hRealHandle);
	HieClient_UnSubscribeEvent(pih->hRealHandle,USEREVENT_HEARTBEAT_LOST|USEREVENT_NET_RECOVER);
	HieClient_UserLogout(pih->hUserHandle);
	g_nSdkClientCount--;
	if (0 == g_nSdkClientCount)
		HieClient_Stop();	

	if(pih->pCtx != NULL)
		sws_freeContext(pih->pCtx);

	if(pih->pRGBBuffer != NULL)
		delete [] pih->pRGBBuffer;

	g_mtx.Lock();
	g_handle_map.erase(pih->ipcode);
	g_mtx.Unlock();
	pih->bConnected = false;
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
	std::string ipcode(szipcode);
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
	*codec = ZW_CODEC_ID_H264;

	return 1;	
}

/*返回设备是否能自动重连。支持的话返回1，否则返回0*/
ZW_VIDEO_API int    __cdecl  CanReConnect(handle hVS)
{
	return 1;
}

/*开启视频源，返回0失败，1成功*/
ZW_VIDEO_API int    __cdecl  StartVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;
	pih->bRaisCallback = true;
	return 1;
}

/*暂停视频源，调用该函数后仍然可以调用StartVideoSource开启*/
ZW_VIDEO_API void   __cdecl  StopVideoSource(handle hVS)
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
	internal_handle *pih = (internal_handle*)hPTZ;

	if(!(pih->bConnected)) return 0;

	param1 = param1<1?1:(param1>64?64:param1);
	param2 = param2<1?1:(param2>64?64:param2);

	DWORD pSpeed = (param1-1)/8+1;	
	DWORD tSpeed = (param2-1)/8+1;

	int nRet = -1;
	switch(cmd)
	{
	case ZW_TILT_UP:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeUp, 1, tSpeed, 0, 0);
		break;
	case ZW_TILT_DOWN:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeDown, 1, tSpeed, 0, 0);
		break;
	case ZW_PAN_LEFT:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeLeft, 1, pSpeed, 0, 0);
		break;
	case ZW_PAN_RIGHT:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeRight, 1, pSpeed, 0, 0);
		break;
	case ZW_LEFTUP:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeUpLeft, 1, pSpeed, 0, 0);
		break;
	case ZW_LEFTDOWN:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeDownLeft, 1, pSpeed, 0, 0);
		break;
    case ZW_RIGHTUP:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeUpRight, 1, pSpeed, 0, 0);
		break;
    case ZW_RIGHTDOWN:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeDownRight, 1, pSpeed, 0, 0);
		break;
    case ZW_ZOOM_OUT:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeZoomIn, 1, pSpeed, 0, 0);	
		break;
    case ZW_ZOOM_IN:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeZoomOut, 1, pSpeed, 0, 0);
		break;
	case ZW_ZOOM_STEP:
		break;
    case ZW_FOCUS_NEAR:
		break;
    case ZW_FOCUS_FAR:
		break;
    case ZW_IRIS_OPEN:
		break;
    case ZW_IRIS_CLOSE:
		break;
    case ZW_SET_PRESET:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodePresetSet, param1, 0, 0, 0);
		break;
    case ZW_USE_PRESET:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodePresetCall, param1, 0, 0, 0);
		break;
    case ZW_DEL_PRESET:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodePresetClear, param1, 0, 0, 0);
		break;
	case ZW_STOP_PAN:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeLeft, 0, pSpeed, 0, 0);
		nRet |= HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeRight, 0, pSpeed, 0, 0);
		break;    
	case ZW_STOP_TILT:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeUp, 0, tSpeed, 0, 0);
		nRet |= HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeDown, 0, tSpeed, 0, 0);
		break;    
	case ZW_STOP_ZOOM:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeZoomOut, 0, pSpeed, 0, 0);
		nRet |= HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeZoomIn, 0, pSpeed, 0, 0);
		break;
    case ZW_STOP_FOCUS:
		break;
    case ZW_STOP_IRIS:
		break;
    case ZW_STOP_ALL:
		nRet = HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeLeft, 0, pSpeed, 0, 0);
		nRet |= HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeRight, 0, pSpeed, 0, 0);
		nRet |= HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeUp, 0, tSpeed, 0, 0);
		nRet |= HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeDown, 0, tSpeed, 0, 0);
		nRet |= HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeZoomOut, 0, pSpeed, 0, 0);
		nRet |= HieClient_PTZControl(pih->hUserHandle, 0, ePTZControlCodeZoomIn, 0, pSpeed, 0, 0);
		break;
    case ZW_LIGHT_ON:
		break;
    case ZW_LIGHT_OFF:
		break;
    case ZW_WIPER_ON:
		break;
    case ZW_WIPER_OFF:
		break;
    default:
		break;
	}

	if (0 == nRet)
	{
		return 1;
	}
	else if (-1 == nRet)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}


/*返回设备是否支持3D定位。支持的话返回1，否则返回0*/
ZW_PTZ_API int    __cdecl CanDirectTo(handle hPTZ)
{
	return 1;
}


int GetPTZConfig(internal_handle *pih,HY_DVR_PTZ_REGION_ZOOM *ZoomCfg)
{
	int nErrorID = -1;
	memset(&pih->tConfigInfo,0,sizeof(ConfigInformation));
    pih->tConfigInfo.dwMainCommand = HY_DVR_GET_SERIALCFG;
    pih->tConfigInfo.dwAssistCommand = PTZCFG_REGION;
    pih->tConfigInfo.dwConfigLen = sizeof(HY_DVR_PTZ_REGION_ZOOM);
	nErrorID = HieClient_GetConfig(pih->hUserHandle,&pih->tConfigInfo);
    if(nErrorID != 0)
        return 0;
    memcpy(ZoomCfg,pih->tConfigInfo.sConfig,sizeof(HY_DVR_PTZ_REGION_ZOOM));
    if(ZoomCfg->bValid == 0)
        return 0;
    return 1;
}

int SetPTZConfig(internal_handle *pih,HY_DVR_PTZ_REGION_ZOOM *ZoomCfg,int Status)
{
	int nErrorID = -1;
	ZoomCfg->bValid = 1;	
	ZoomCfg->iStatus  = Status;//1放大，0还原
    memset(&pih->tConfigInfo,0,sizeof(ConfigInformation));
    pih->tConfigInfo.dwMainCommand = HY_DVR_SET_SERIALCFG;
    pih->tConfigInfo.dwAssistCommand = PTZCFG_REGION;
    pih->tConfigInfo.dwConfigLen = sizeof(HY_DVR_PTZ_REGION_ZOOM);
    memcpy(pih->tConfigInfo.sConfig,ZoomCfg,pih->tConfigInfo.dwConfigLen);
	nErrorID = HieClient_SetConfig(pih->hUserHandle,&pih->tConfigInfo);
    if(nErrorID != 0)
		return 0;
    return 1;
}

int Position3D(internal_handle *pih,int LeftUp_x,int LeftUp_y,int RightDown_x,int RightDown_y,int Status )
{
	HY_DVR_PTZ_REGION_ZOOM cfgZoom;

	if(GetPTZConfig(pih,&cfgZoom))
	{
		cfgZoom.iStart_x = LeftUp_x;
		cfgZoom.iStart_y = LeftUp_y;
		cfgZoom.iEnd_x = RightDown_x;
		cfgZoom.iEnd_y = RightDown_y;
		return SetPTZConfig(pih,&cfgZoom,Status);
	}

	return 0;
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

	if(!(pih->bConnected)) return 0;

	float fWRate = ((float)1920.0)/(float)vw;
	float fHRate = ((float)1080.0)/(float)vh;
	int nrx = (int)((float)rx*fWRate);
	int nry = (int)((float)ry*fHRate);
	int nrw = (int)((float)rw*fWRate);
	int nrh = (int)((float)rh*fHRate);

	int x1 = nrx-960;
	int y1 = 540-nry;

	int x2 = x1+nrw;
	int y2 = y1-nrh;

	x1 = x1 < -960?-960:x1>960?960:x1;
	x2 = x2 < -960?-960:x2>960?960:x2;
	y1 = y1 < -540?-540:y1>540?540:y1;
	y2 = y2 < -540?-540:y2>540?540:y2;

	return Position3D(pih,x1,y1,x2,y2,1);
}

/*销毁用于PTZ控制的句柄*/
ZW_PTZ_API void   __cdecl FinializePTZ(handle hPTZ)
{
	InternalFinialize((internal_handle*)hPTZ);
}
