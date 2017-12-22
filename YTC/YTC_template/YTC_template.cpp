 
#include <windows.h>
#include <winnt.h>
#include <map>
#include <string>
#include "Video.h"
#include "PTZ.h"
#include "ThreadMutex.h"
#include <assert.h>

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

		started = false;
	}
};

ThreadMutex  g_mtx;
std::map<std::string,internal_handle*> g_handle_map;


internal_handle* __cdecl  InternalInitial(ZWVideoParam *param,unsigned long addr)
{
	internal_handle *pih = new internal_handle();

	memcpy(&(pih->param), &param, sizeof(param));
	
	return pih;
}

void   __cdecl  InternalFinialize(internal_handle* pih)
{
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
	assert(realFixFmt < 100);
	assert(triggerFixFmt < 100);
	internal_handle *pih = NULL;
	unsigned long addr = inet_addr(param->CameraIp);
	char szipcode[10]={0};
	sprintf_s(szipcode,10,"%08X",addr);
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

	if(pih != NULL)
	{
		InterlockedIncrement(&(pih->nRefCount));
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
	return 0;
}

/*开启视频源，返回0失败，1成功*/
ZW_VIDEO_API int    __cdecl  StartVideoSource(handle hVS, handle hPlayWnd)
{
	internal_handle *pih = (internal_handle*)hVS;
	if (pih->started)
		return 0;
	return 1;
}

/*触发功能*/
ZW_VIDEO_API int    __cdecl  TriggerPeripheral(handle hVS,int func)
{
	internal_handle *pih = (internal_handle*)hVS;

	return 0;
}

/*暂停视频源，调用该函数后仍然可以调用StartVideoSource开启*/
ZW_VIDEO_API void   __cdecl  StopVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;
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
**当cmd为ZOOM_STEP时，param1为负值表示从当前位置ZOOM_OUT多少级别，正值表示ZOOM_IN多少级别
**
**
**返回 1-表示执行成功 0-表示执行失败 -1-表示不支持的操作
*/
ZW_PTZ_API int   __cdecl ExecPTZ(handle hPTZ,int cmd,int param1,int param2)
{
	internal_handle *pih = (internal_handle*)hPTZ;

	switch(cmd)
	{
	case ZW_TILT_UP:
		break;
	case ZW_TILT_DOWN:
		break;
	case ZW_PAN_LEFT:
		break;
	case ZW_PAN_RIGHT:
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
		break;
	case ZW_ZOOM_IN:
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
		break;
	case ZW_USE_PRESET:
		break;
	case ZW_DEL_PRESET:
		break;    
	case ZW_STOP_PAN: 
		break;
	case ZW_STOP_TILT:
		break;
	case ZW_STOP_ZOOM:
		break;
	case ZW_STOP_FOCUS:
		break;
	case ZW_STOP_IRIS:
		break;
	case ZW_STOP_ALL:
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

	return 1;
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
	return 0;
}

/*销毁用于PTZ控制的句柄*/
ZW_PTZ_API void   __cdecl FinializePTZ(handle hPTZ)
{
	FinializeVideoSource(hPTZ);
}
