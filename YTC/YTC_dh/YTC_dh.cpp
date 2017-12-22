#include <windows.h>
#include <map>

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


#include <string>
#include "Video.h"
#include "PTZ.h"
#include "ThreadMutex.h"

#include "dhnetsdk.h"
#include "dhplay.h"

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

#pragma comment(lib, "dhnetsdk.lib")
#pragma comment(lib, "dhplay.lib")


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
	long lUserHandle;           //用户句柄
	long lRealHandle;		    //实时流句柄
	long lChannel;              //实时流通道
	long lPort;                 //解码器通道

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
		lRealHandle = -1;
		lChannel = -1;
		lPort = -1;
	}
};

long  g_lPlayPort = 0;/*当前可用的解码通道*/

ThreadMutex  g_mtx;
std::map<std::string,internal_handle*> g_handle_map;


/*实时视频流回调*/
void CALLBACK iveInternal_StreamReadyCB(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, LDWORD dwUser)
{
	internal_handle *pih = (internal_handle*)dwUser;
	int nRet = 0;
	if (dwDataType == 0 && pih->lPort != -1)
	{	
		if (pih->bRaisCallback && pih->OnStreamReady != NULL)
		{
			//标准流回调
			pih->OnStreamReady(pih->pUserData, (void*)pBuffer, dwBufSize, 0);
		}

		nRet = PLAY_InputData(pih->lPort, pBuffer, dwBufSize);		
	}	
}

/*实时解码RGB回调*/
void CALLBACK iveInternal_RealRGBReadyCB(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2)
{
	internal_handle *pih = (internal_handle*)nReserved1;

	if(!pih->bRaisCallback || pFrameInfo->nWidth == 0)
	{
		return;
	}	

	if(pih->OnPixBufReady != NULL)
	{
		pih->OnPixBufReady(pih->pUserData,pBuf,ZW_PIX_FMT_YUV420P,pFrameInfo->nWidth,0,pFrameInfo->nHeight);
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

			if(sws_scale(pih->pCtx,avSrc.data,avSrc.linesize,0,pFrameInfo->nHeight,avDst.data,avDst.linesize)>0)
			{   //RGB回调
				pih->OnRealRGBReady(pih->pUserData,pih->pRGBBuffer,pFrameInfo->nWidth,avDst.linesize[0],pFrameInfo->nHeight);
			}
		}	
	}
}


internal_handle* __cdecl  InternalInitial(VideoParam *param,std::string& ipcode)
{
	BOOL bRet = false;
	//用户登录
	NET_DEVICEINFO info;
	int nError = 0;
	int nRet = 0;

	bRet = CLIENT_Init(0, 0);
	if (!bRet)
		return NULL;

	internal_handle *pih = NULL;

	try
	{
		pih	= new internal_handle();
	}
	catch(...)
	{
		pih = NULL;
	}

	if(pih == NULL)
		return NULL;

	pih->lChannel = param->AddressNum;

	//设置断线重连
	CLIENT_SetAutoReconnect(NULL, 0);
	//设置连接等待时间
	CLIENT_SetConnectTime(3000, 3);	

	pih->lUserHandle = CLIENT_Login(param->CameraIp, param->CameraPort, param->UserName, param->Password, &info, &nError);
	if (pih->lUserHandle <= 0)
	{
		delete pih;
		return NULL;
	}

	pih->lRealHandle = CLIENT_RealPlayEx(pih->lUserHandle, pih->lChannel,NULL);	
	if (pih->lRealHandle <= 0)
	{
		CLIENT_Logout(pih->lUserHandle);
		delete pih;
		return NULL;
	}

	bRet = CLIENT_SetRealDataCallBackEx(pih->lRealHandle, iveInternal_StreamReadyCB, LDWORD(pih), 0x00000007);
	if (!bRet)
	{
		CLIENT_StopRealPlay(pih->lRealHandle);
		CLIENT_Logout(pih->lUserHandle);
		delete pih;
		return NULL;
	}

	//解码设置
	//nRet = PLAY_GetFreePort(&pih->lPort);
	//if(nRet)
	//{
	//	nRet = PLAY_SetStreamOpenMode(pih->lPort,STREAME_REALTIME);
	//	if(nRet)
	//		nRet = PLAY_OpenStream(pih->lPort, 0,0,1920*1080);
	//}
	
	pih->lPort = g_lPlayPort++;
	nRet = PLAY_OpenStream(pih->lPort, 0,0,1920*1080);
	if (!bRet)
	{
		CLIENT_StopRealPlay(pih->lRealHandle);
		CLIENT_Logout(pih->lUserHandle);
		delete pih;
		return NULL;
	}
	nRet = PLAY_Play(pih->lPort, NULL);
	if (!bRet)
	{
		PLAY_CloseStream(pih->lPort);	
		CLIENT_StopRealPlay(pih->lRealHandle);
		CLIENT_Logout(pih->lUserHandle);
		delete pih;
		return NULL;
	}
	nRet = PLAY_SetDecCallBackEx(pih->lPort, iveInternal_RealRGBReadyCB, (long)pih);
	if (!bRet)
	{
		PLAY_Stop(pih->lPort);
		PLAY_CloseStream(pih->lPort);	
		CLIENT_StopRealPlay(pih->lRealHandle);
		CLIENT_Logout(pih->lUserHandle);
		delete pih;
		return NULL;
	}	

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

	PLAY_Stop(pih->lPort);
	PLAY_CloseStream(pih->lPort);	

	CLIENT_StopRealPlay(pih->lRealHandle);
	CLIENT_Logout(pih->lUserHandle);

	CLIENT_Cleanup();

	if(pih->pCtx != NULL)
		sws_freeContext(pih->pCtx);

	if(pih->pRGBBuffer != NULL)
		delete [] pih->pRGBBuffer;

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
		pih = InternalInitial(param,ipcode);
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
ZW_VIDEO_API int   __cdecl  GetVideoInfo(handle hVS,int *decode, int *codec)
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
		pih = InternalInitial(&vparam,ipcode);
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

	param1 = param1<1?1:(param1>64?64:param1);
	param2 = param2<1?1:(param2>64?64:param2);


	DWORD pSpeed = (param1-1)/8+1;	
	DWORD tSpeed = (param2-1)/8+1;

	int  ret = 0;
	BOOL bRet = FALSE;
	switch(cmd)
	{
	case ZW_TILT_UP:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_UP_CONTROL, 0, tSpeed, 0, FALSE);
		break;
	case ZW_TILT_DOWN:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_DOWN_CONTROL, 0, tSpeed, 0, FALSE);
		break;
	case ZW_PAN_LEFT:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_LEFT_CONTROL, 0, pSpeed, 0, FALSE);
		break;
	case ZW_PAN_RIGHT:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_RIGHT_CONTROL, 0, pSpeed, 0, FALSE);
		break;
	case ZW_LEFTUP:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_EXTPTZ_LEFTTOP, pSpeed, tSpeed, 0, FALSE);
		break;
	case ZW_LEFTDOWN:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_EXTPTZ_LEFTDOWN, pSpeed, tSpeed, 0, FALSE);
		break;
    case ZW_RIGHTUP:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_EXTPTZ_RIGHTTOP, pSpeed, tSpeed, 0, FALSE);
		break;
    case ZW_RIGHTDOWN:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_EXTPTZ_RIGHTDOWN, pSpeed, tSpeed, 0, FALSE);
		break;
    case ZW_ZOOM_OUT:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_ZOOM_DEC_CONTROL, 0, pSpeed, 0, FALSE);	
		break;
    case ZW_ZOOM_IN:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_ZOOM_ADD_CONTROL, 0, pSpeed, 0, FALSE);
		break;
	case ZW_ZOOM_STEP:
		ret = -1;
		break;
    case ZW_FOCUS_NEAR:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_FOCUS_DEC_CONTROL, 0, pSpeed, 0, FALSE);
		break;
    case ZW_FOCUS_FAR:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_FOCUS_ADD_CONTROL, 0, pSpeed, 0, FALSE);
		break;
    case ZW_IRIS_OPEN:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_APERTURE_ADD_CONTROL, 0, pSpeed, 0, FALSE);
		break;
    case ZW_IRIS_CLOSE:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_APERTURE_DEC_CONTROL, 0, pSpeed, 0, FALSE);
		break;
    case ZW_SET_PRESET:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_POINT_SET_CONTROL, 0, param1, 0, FALSE);
		break;
    case ZW_USE_PRESET:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_POINT_MOVE_CONTROL, 0, param1, 0, FALSE);
		break;
    case ZW_DEL_PRESET:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_POINT_DEL_CONTROL, 0, param1, 0, FALSE);
		break;    
	case ZW_STOP_PAN:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_LEFT_CONTROL, 0, pSpeed, 0, TRUE);
		bRet &= CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_RIGHT_CONTROL, 0, pSpeed, 0, TRUE);
		break;    
	case ZW_STOP_TILT:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_UP_CONTROL, 0, tSpeed, 0, TRUE);
		bRet &= CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_DOWN_CONTROL, 0, tSpeed, 0, TRUE);
		break;    
	case ZW_STOP_ZOOM:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_ZOOM_ADD_CONTROL, 0, pSpeed, 0, TRUE);
		bRet &= CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_ZOOM_DEC_CONTROL, 0, pSpeed, 0, TRUE);
		break;
    case ZW_STOP_FOCUS:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_FOCUS_DEC_CONTROL, 0, pSpeed, 0, TRUE);
		break;
    case ZW_STOP_IRIS:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_APERTURE_ADD_CONTROL, 0, pSpeed, 0, TRUE);
		break;
    case ZW_STOP_ALL:
		bRet = CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_LEFT_CONTROL, 0, pSpeed, 0, TRUE);
		bRet &= CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_RIGHT_CONTROL, 0, pSpeed, 0, TRUE);
		bRet &= CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_UP_CONTROL, 0, tSpeed, 0, TRUE);
		bRet &= CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_DOWN_CONTROL, 0, tSpeed, 0, TRUE);
		bRet &= CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_ZOOM_DEC_CONTROL, 0, pSpeed, 0, TRUE);
		bRet &= CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_ZOOM_ADD_CONTROL, 0, pSpeed, 0, TRUE);
		bRet &= CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_FOCUS_DEC_CONTROL, 0, pSpeed, 0, TRUE);
		bRet &= CLIENT_DHPTZControlEx(pih->lUserHandle, pih->lChannel, DH_PTZ_APERTURE_ADD_CONTROL, 0, pSpeed, 0, TRUE);
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

	if(ret == -1)
		return -1;
	else
		return bRet?1:0;
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
	/*控制云台设备完成3D定位*/
	if (vw <= 0 || vh <= 0 || rw <= 0 || rh <= 0 || rw <= 0 || rh <= 0)
	{
		return 0;
	}

	internal_handle *pih = (internal_handle*)hPTZ;
	int nPosX = 0;
	int nPosY = 0;
	int nPosZoom = 0;

	int ndx = rx + rw/2;
	int ndy = ry + rh/2;

	nPosX = (ndx - vw/2) * 8192 * 2 / vw;
	nPosY = (ndy - vh/2) * 8192 * 2 / vh;
	if (0 == zoom)
	{
		nPosZoom = 0;
	}
	else
	{
		nPosZoom = (vw*vh) / (rw*rh);
	}	

	BOOL bRet = CLIENT_DHPTZControlEx(pih->lUserHandle,pih->lChannel,DH_EXTPTZ_FASTGOTO,nPosX,nPosY,nPosZoom,FALSE);

	if (bRet)
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
