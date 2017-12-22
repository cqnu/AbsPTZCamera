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

#include <windows.h>
#include <map>
#include <string>
#include "Video.h"
#include "PTZ.h"
#include "ThreadMutex.h"

#include "IPCAPI.h"
#pragma  comment(lib,"NetDvr2.lib")

const int SWS_FAST_FLAGS = SWS_CPU_CAPS_MMX | SWS_CPU_CAPS_MMX2 | SWS_CPU_CAPS_3DNOW | SWS_CPU_CAPS_ALTIVEC | SWS_CPU_CAPS_BFIN ;


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

	int   hDvr;
	int   hRealPlay;
	int   nAddress;

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

		hDvr = 0;
		hRealPlay = 0;
		nAddress = 0;
	}
};

int g_nSdkClientCount = 0;
ThreadMutex  g_mtx;
std::map<std::string,internal_handle*> g_handle_map;

void __stdcall CBReconnMsg(enum RECONNMSG msg, unsigned int dwContent)
{
	return;
}

void CALLBACK DealDecVideoFrame(pFrameHeadrDec pFrmHdrDec, unsigned int dwContext)
{
	internal_handle *pih =  (internal_handle*)dwContext;

	if (pFrmHdrDec->mediaType!=MEDIA_TYPE_H264||!pih->bRaisCallback)
	{
		return;
	}
	if(pih->OnPixBufReady)
		pih->OnPixBufReady(pih->pUserData,(void*)pFrmHdrDec->data,ZW_PIX_FMT_YUV420P,pFrmHdrDec->video_param.width,0,pFrmHdrDec->video_param.height);

	if( pih->OnRealRGBReady != NULL)
	{
		//YUV转RGB
		if(pih->pCtx == NULL)
		{
			pih->pCtx = sws_getContext(pFrmHdrDec->video_param.width,
				pFrmHdrDec->video_param.height,
				AV_PIX_FMT_YUV420P,
				pFrmHdrDec->video_param.width,
				pFrmHdrDec->video_param.height,
				AV_PIX_FMT_RGB24,
				SWS_BICUBIC | SWS_FAST_FLAGS,
				NULL,NULL,NULL);
		}	

		if(pih->pCtx != NULL)
		{
			if(pih->pRGBBuffer == NULL)
			{
				pih->pRGBBuffer = new char[3*pFrmHdrDec->video_param.width*pFrmHdrDec->video_param.height];
			}		

			AVPicture avSrc;
			avpicture_fill(&avSrc,(uint8_t*)pFrmHdrDec->data,AV_PIX_FMT_YUV420P,pFrmHdrDec->video_param.width,pFrmHdrDec->video_param.height);

			AVPicture avDst;
			avpicture_fill(&avDst,(uint8_t*)pih->pRGBBuffer,AV_PIX_FMT_RGB24,pFrmHdrDec->video_param.width,pFrmHdrDec->video_param.height);

			if(sws_scale(  pih->pCtx,avSrc.data,avSrc.linesize,0,pFrmHdrDec->video_param.height,avDst.data,avDst.linesize)>0)
			{   //RGB回调
				pih->OnRealRGBReady(pih->pUserData,pih->pRGBBuffer,pFrmHdrDec->video_param.width,avDst.linesize[0],pFrmHdrDec->video_param.height);
			}
		}
	}
}

void CALLBACK get_encframe(pFrameHeadr pFrmHdr, unsigned int dwContextEnc)
{
	internal_handle *pih =  (internal_handle*)dwContextEnc;

	if (pFrmHdr->mediaType == MEDIA_TYPE_H264 && pih->bRaisCallback && pih->OnStreamReady != NULL)
	{
		pih->OnStreamReady(pih->pUserData, (void*)pFrmHdr->pData, pFrmHdr->dataSize, pFrmHdr->videoParam.keyFrame?1:3);
	}	
}


internal_handle* __cdecl  InternalInitial(VideoParam *param,unsigned long ip,std::string& ipcode)
{
	if(g_nSdkClientCount == 0)
	{
		if (NETDVR_SUCCESS != NETDVR_startup())
		{
			return NULL;
		}
	}

	internal_handle *pih = new internal_handle();

	if(NETDVR_SUCCESS == NETDVR_createDVR(&pih->hDvr, ip, param->CameraPort))
	{
		struct NETDVR_loginInfo_t logininfo;
		memset(&logininfo, 0, sizeof(struct NETDVR_loginInfo_t));
		memcpy(logininfo.username,param->UserName,12);
		memcpy(logininfo.loginpass, param->Password, 12);
		if(NETDVR_SUCCESS == NETDVR_loginServer(pih->hDvr, &logininfo))
		{
			NETDVR_regCBMsgReconnect(pih->hDvr, CBReconnMsg, 0);
			NETDVR_setReconnectFlag(pih->hDvr, 1);			

			RealPlayClientInfo_t rpci;
			rpci.rcv_chn = 0;
			rpci.streamtype = param->AddressNum==0?0:1;
			rpci.pEncFrameCBFunc = get_encframe;
			rpci.dwEncFrameContent = (UINT)pih;
			rpci.pDecFrameCBFunc = DealDecVideoFrame;
			rpci.dwDecFrameContent = (UINT)pih;

			if(NETDVR_SUCCESS == NETDVR_StartRealPlay(pih->hDvr, &rpci, &pih->hRealPlay))
			{
				NETDVR_SerialStart(pih->hDvr,1,NULL,0);
				pih->ipcode = ipcode;
				g_mtx.Lock();
				g_handle_map.insert(std::make_pair(ipcode,pih));
				g_mtx.Unlock();
				g_nSdkClientCount++;
				return pih;
			}

			NETDVR_logoutServer(pih->hDvr);
		}
		NETDVR_destroyDVR(pih->hDvr);
	}

	NETDVR_cleanup();
	delete pih;
	return NULL;
}

void   __cdecl  InternalFinialize(internal_handle* pih)
{
	pih->nRefCount--;
	if(pih->nRefCount != 0)
		return;

	pih->bRaisCallback = false;
	NETDVR_SerialStop(pih->hDvr,1);
	NETDVR_StopRealPlay(pih->hRealPlay);
	NETDVR_logoutServer(pih->hDvr);
	NETDVR_destroyDVR(pih->hDvr);

	g_nSdkClientCount--;
	if(g_nSdkClientCount == 0) 
		NETDVR_cleanup();

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
	std::map<std::string,internal_handle*>::iterator pos = g_handle_map.find(ipcode);
	g_mtx.Lock();
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

	param1 = param1<1?1:(param1>64?64:param1);

	unsigned char speed = (param1-1)/8+1;


	unsigned char cmd_cam[10];
	NETDVR_PtzCtrl_t ptzctr;
	bool  support = true;
	int   ret = NETDVR_SUCCESS;
	memset(&ptzctr,0,sizeof(ptzctr));
	ptzctr.chn = pih->nAddress;

	if(0x0200>cmd)
	{
		int speed = (param1>param2?param1:param2)/22;
		switch(speed)
		{
		case 0:
			ptzctr.cmd = NETDVR_PTZ_COM_SLOW;
			break;
		case 1:
			ptzctr.cmd = NETDVR_PTZ_COM_NORMAL;
			break;
		default:
			ptzctr.cmd = NETDVR_PTZ_COM_FAST;
			break;
		}

		NETDVR_PtzControl(pih->hDvr, &ptzctr);
	}
	
	switch(cmd)
	{
	case ZW_TILT_UP:
		ptzctr.cmd = NETDVR_PTZ_COM_MOVEUP;
		break;
	case ZW_TILT_DOWN:
		ptzctr.cmd = NETDVR_PTZ_COM_MOVEDOWN;
		break;
	case ZW_PAN_LEFT:
		ptzctr.cmd = NETDVR_PTZ_COM_MOVELEFT;
		break;
	case ZW_PAN_RIGHT:
		ptzctr.cmd = NETDVR_PTZ_COM_MOVERIGHT;
		break;
	case ZW_LEFTUP:
		support = false;
		break;
	case ZW_LEFTDOWN:
		support = false;
		break;
    case ZW_RIGHTUP:
		support = false;
		break;
    case ZW_RIGHTDOWN:
		support = false;
		break;
    case ZW_ZOOM_OUT:
		break;
		cmd_cam[0]=0x81;
		cmd_cam[1]=0x01;
		cmd_cam[2]=0x04;
		cmd_cam[3]=0x07;
		cmd_cam[4]=0x30|speed;
		cmd_cam[5]=0xFF;		
		NETDVR_SerialSend(pih->hDvr,1,0,(char*)cmd_cam,6);
		support = false;
		ptzctr.cmd = NETDVR_PTZ_COM_ZOOMSUBTRACT;
		break;
    case ZW_ZOOM_IN:
		break;
		cmd_cam[0]=0x81;
		cmd_cam[1]=0x01;
		cmd_cam[2]=0x04;
		cmd_cam[3]=0x07;
		cmd_cam[4]=0x20|speed;
		cmd_cam[5]=0xFF;
		NETDVR_SerialSend(pih->hDvr,1,0,(char*)cmd_cam,6);
		support = false;
		ptzctr.cmd = NETDVR_PTZ_COM_ZOOMADD;
		break;
	case ZW_ZOOM_STEP:
		support = false;
		break;
    case ZW_FOCUS_NEAR:
		ptzctr.cmd = NETDVR_PTZ_COM_FOCUSSUBTRACT;
		break;
    case ZW_FOCUS_FAR:
		ptzctr.cmd = NETDVR_PTZ_COM_FOCUSADD;
		break;
    case ZW_IRIS_OPEN:
		ptzctr.cmd = NETDVR_PTZ_COM_APERTUREADD;
		break;
    case ZW_IRIS_CLOSE:
		ptzctr.cmd = NETDVR_PTZ_COM_APERTURESUBTRACT;
		break;
    case ZW_SET_PRESET:
		ret = NETDVR_SetYTPresetPoint(pih->hDvr, pih->nAddress, param1, NETDVR_YT_COM_ADDPRESETPOINT);
		return NETDVR_SUCCESS == ret?1:0;		
    case ZW_USE_PRESET:
		ret = NETDVR_SetYTPresetPoint(pih->hDvr, pih->nAddress, param1, NETDVR_YT_COM_TOPRESETPOINT);
		return NETDVR_SUCCESS == ret?1:0;
    case ZW_DEL_PRESET:
		ret = NETDVR_SetYTPresetPoint(pih->hDvr, pih->nAddress, param1, NETDVR_YT_COM_DELPRESETPOINT);
		return NETDVR_SUCCESS == ret?1:0;    
	case ZW_STOP_PAN:
		ptzctr.cmd = NETDVR_PTZ_COM_STOP;
		break;    
	case ZW_STOP_TILT:
		ptzctr.cmd = NETDVR_PTZ_COM_STOP;
		break;    
	case ZW_STOP_ZOOM:
		break;
		cmd_cam[0]=0x81;
		cmd_cam[1]=0x01;
		cmd_cam[2]=0x04;
		cmd_cam[3]=0x07;
		cmd_cam[4]=0x00;
		cmd_cam[5]=0xFF;
		NETDVR_SerialSend(pih->hDvr,1,0,(char*)cmd_cam,6);

		cmd_cam[0]=0x81;
		cmd_cam[1]=0x01;
		cmd_cam[2]=0x04;
		cmd_cam[3]=0x07;
		cmd_cam[4]=0x00;
		cmd_cam[5]=0xFF;
		NETDVR_SerialSend(pih->hDvr,1,0,(char*)cmd_cam,6);
		support = false;
		break;
    case ZW_STOP_FOCUS:
		ptzctr.cmd = NETDVR_PTZ_COM_FOCUSADDSTOP;
		break;
    case ZW_STOP_IRIS:
		ptzctr.cmd = NETDVR_PTZ_COM_IRISLARGESTOP;
		break;
    case ZW_STOP_ALL:
		ptzctr.cmd = NETDVR_PTZ_COM_STOP;
		cmd_cam[0]=0x81;
		cmd_cam[1]=0x01;
		cmd_cam[2]=0x04;
		cmd_cam[3]=0x07;
		cmd_cam[4]=0x00;
		cmd_cam[5]=0xFF;
		NETDVR_SerialSend(pih->hDvr,1,0,(char*)cmd_cam,6);
		break;
    case ZW_LIGHT_ON:
		ptzctr.cmd = NETDVR_PTZ_COM_LIGHTINGOPEN;
		break;
    case ZW_LIGHT_OFF:
		ptzctr.cmd = NETDVR_PTZ_COM_LIGHTINGCLOSE;
		break;
    case ZW_WIPER_ON:
		ptzctr.cmd = NETDVR_PTZ_COM_WIPERSOPEN;
		break;
    case ZW_WIPER_OFF:
		ptzctr.cmd = NETDVR_PTZ_COM_WIPERSCLOSE;
		break;
    default:
		support = false;
		break;
	}

	if(support)
	{
		int ret = NETDVR_PtzControl(pih->hDvr, &ptzctr);
		return NETDVR_SUCCESS == ret?1:0;	
	}

	return -1;
}


/*返回设备是否支持3D定位。支持的话返回1，否则返回0*/
ZW_PTZ_API int    __cdecl CanDirectTo(handle hPTZ)
{
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
	return 0;
}

/*销毁用于PTZ控制的句柄*/
ZW_PTZ_API void   __cdecl FinializePTZ(handle hPTZ)
{
	InternalFinialize((internal_handle*)hPTZ);
}
