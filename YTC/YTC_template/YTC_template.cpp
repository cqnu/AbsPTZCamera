 
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

	VideoStreamCallback OnVideoStreamReady;/*ʵʱ��Ƶ���ص�*/
	RealTimeImageCallback OnRealImgReady;/*ʵʱͼ������ص�*/
	RealTimeImageCallback OnRealFixFmtImgReady;/*ʵʱת��ͼ������ص�*/
	int nRealFixFmt;/*ʵʱת��ͼ��ĸ�ʽ*/
	TriggerImageCallback OnTriggerImgReady;  /*�ⲿ����ץ��ͼƬ�ص�*/
	TriggerImageCallback OnTriggerFixFmtImgReady;/*�ⲿ����ץ��ת��ͼƬ�ص�*/
	int nTriggerFixFmt;/*�ⲿ����ץ��ת��ͼƬ�ĸ�ʽ*/
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

/*��ʼ����ƵԴ������ص����ڻ�ȡ�������ⲿ����ץ�ĵ�ͼƬ*/
ZW_VIDEO_API handle __cdecl  InitialVideoSource(ZWVideoParam *param,
	                                         VideoStreamCallback OnVideoStreamReady,/*ʵʱ��Ƶ���ص�*/
	                                         RealTimeImageCallback OnRealImgReady,/*ʵʱͼ������ص�*/
	                                         RealTimeImageCallback OnRealFixFmtImgReady,/*ʵʱת��ͼ������ص�*/
	                                         int realFixFmt,/*ʵʱת��ͼ��ĸ�ʽ*/
	                                         TriggerImageCallback OnTriggerImgReady,/*�ⲿ����ץ��ͼƬ�ص�*/
	                                         TriggerImageCallback OnTriggerFixFmtImgReady,/*�ⲿ����ץ��ת��ͼƬ�ص�*/
	                                         int triggerFixFmt,/*�ⲿ����ץ��ת��ͼƬ�ĸ�ʽ*/
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


/*��ȡ��ƵԴ���룬����ZW_CODEC_ID_? */
ZW_VIDEO_API int   __cdecl  GetVideoCodec(handle hVS)
{
	return ZW_CODEC_ID_NONE;
}

/*�����豸�Ƿ����Զ�������֧�ֵĻ�����1�����򷵻�0*/
ZW_VIDEO_API int    __cdecl  CanReConnect(handle hVS)
{
	return 0;
}

/*������ƵԴ������0ʧ�ܣ�1�ɹ�*/
ZW_VIDEO_API int    __cdecl  StartVideoSource(handle hVS, handle hPlayWnd)
{
	internal_handle *pih = (internal_handle*)hVS;
	if (pih->started)
		return 0;
	return 1;
}

/*��������*/
ZW_VIDEO_API int    __cdecl  TriggerPeripheral(handle hVS,int func)
{
	internal_handle *pih = (internal_handle*)hVS;

	return 0;
}

/*��ͣ��ƵԴ�����øú�������Ȼ���Ե���StartVideoSource����*/
ZW_VIDEO_API void   __cdecl  StopVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;
}

/*��ֹ��ƵԴģ��*/
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



/*��������PTZ���Ƶľ��*/
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


/*ִ��PTZָ�param1-pan speed������������speed��param2-tilt speed
**pan��tilt��speedΪ1-64������������speedΪ1-8
**�������������֧��speedֱ�Ӻ���
**��cmdΪZOOM_STEPʱ��param1Ϊ��ֵ��ʾ�ӵ�ǰλ��ZOOM_OUT���ټ�����ֵ��ʾZOOM_IN���ټ���
**
**
**���� 1-��ʾִ�гɹ� 0-��ʾִ��ʧ�� -1-��ʾ��֧�ֵĲ���
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


/*�����豸�Ƿ�֧��3D��λ��֧�ֵĻ�����1�����򷵻�0*/
ZW_PTZ_API int    __cdecl CanDirectTo(handle hPTZ)
{
	return 1;
}

/*����3D��λ���ܡ�
**zoom ��ʾ3D��λ��ʱ���Ƿ�ִ��ZOOM������0��ʾ��ִ�У�1��ʾִ��
**vw��vh ��Ƶ��������Ŀ�ȡ��߶�   
**rx��ry��rw��rh   3D��λ���ܿ�ѡ���x���ꡢy���ꡢ��ȡ��߶�
**3D��λ���ܿ�ѡ�������ϵ�Ķ��������£�
**����ԭ������Ƶ������������Ͻǣ�y�������£�x��������
**
**
**���� 1-��ʾִ�гɹ� 0-��ʾִ��ʧ��
*/
ZW_PTZ_API int   __cdecl DirectTo(handle hPTZ,int zoom,int vw,int vh,int rx, int ry, int rw,int rh)
{
	return 0;
}

/*��������PTZ���Ƶľ��*/
ZW_PTZ_API void   __cdecl FinializePTZ(handle hPTZ)
{
	FinializeVideoSource(hPTZ);
}
