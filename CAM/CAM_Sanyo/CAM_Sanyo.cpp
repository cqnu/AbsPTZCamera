 
#include <windows.h>
#include <map>
#include <string>
#include "Video.h"
#include "PTZ.h"
#include "ThreadMutex.h"
#include "ThreadTimer.h"
#include "time.h"

#include "SY_NET.h"
#include "SY_NET_Define.h"
#include "SY_PLAY.h"
#include "SY_PLAY_Define.h"

#if defined(_DEBUG)
#if defined(_MEM_LEAK_DETECT) // �ڴ�й©���֧�֡�
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <malloc.h> // ��� malloc.h �� crtdbg.h ˳���µ� Debug Assertion Failed, "Corrupted pointer passed to _freea" ��
#include <crtdbg.h>
#undef DEBUG_NEW
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif
#endif // _DEBUG

#pragma comment(lib, "SY_NET_SDK.lib")
#pragma comment(lib, "SY_PLAY_SDK.lib")

struct internal_handle
{
	StreamCallback OnStreamReady;/*ʵʱ��Ƶ���ص�*/
	PixBufCallback OnPixBufReady;/*������������ص�*/
	RealTimeRGBCallback OnRealRGBReady;  /*ʵʱ����ͼƬ�ص�*/
	TriggerImageCallback OnTriggerRGBReady;  /*�ⲿ����ץ��ͼƬ�ص�*/
	bool  bRaisCallback;
	void* pUserData;
	int   nStoreWidth;
	int   nRefCount;
	std::string ipcode;

	SY_CODEC_STREAM_STATUS status;
	long	hCamera;
	long	hPlayer;
	long	hParser;

	SY_CAMERA_CONTROL_TYPE     nCmd;
	CThreadTimer zmTimer;
	time_t       tmCreate;
	internal_handle()
	{
		nCmd = SYCCT_ZOOM_STOP;
		OnStreamReady = NULL;
		OnPixBufReady = NULL;
		OnRealRGBReady = NULL;
		OnTriggerRGBReady = NULL;
		pUserData = NULL;
		bRaisCallback = false;
		nStoreWidth = 0;
		hCamera = 0;
		hPlayer = 0;
		hParser = 0;
		nRefCount = 0;
		tmCreate = time(NULL);
		memset(&status,0,sizeof(SY_CODEC_STREAM_STATUS));
	}
};

ThreadMutex  g_mtx;
std::map<std::string,internal_handle*> g_handle_map;

VOID __stdcall ZoomFunc(LPVOID lpParam)
{
	internal_handle *pih = (internal_handle*)lpParam;

	if(pih->nCmd != SYCCT_ZOOM_STOP)
		SY_NET_CameraControl(pih->hCamera,pih->nCmd,1);
}

VOID __stdcall RestartFunc(LPVOID lpParam)
{
	internal_handle *pih = (internal_handle*)lpParam;

	time_t now = time(NULL);

	SYSTEMTIME tm;
	::GetSystemTime(&tm);

	//�賿2���ʱ���������
	if(tm.wHour == 18 && (now-pih->tmCreate)> 7200000)
	{
		SY_NET_CameraControl(pih->hCamera,SYCCT_ZOOM_TELE_ONE_STEP,1);
		pih->bRaisCallback = false;
	}
}

void CALLBACK ParseVideoFrameCallBack( long nParserID, LPSY_FRAME pFrame,int nVersion, LPVOID pvParam )
{
	internal_handle *pih = (internal_handle*)pvParam;
	if ( sizeof( SY_FRAME ) <= nVersion ) // ȷ����ǰ֡��Ϣ��Ч
	{
		SY_PLAY_PlayerInputRealData( pih->hPlayer, pFrame->pData, pFrame->dwDataSize ) ;
	}
}

void CALLBACK ParseAudioFrameCallBack( long nParserID, LPSY_FRAME pFrame, int nVersion, LPVOID pvParam )
{
	if ( sizeof( SY_FRAME ) <= nVersion ) // ȷ����ǰ֡��Ϣ��Ч
	{
		internal_handle *pih = (internal_handle*)pvParam;
		SY_PLAY_PlayerInputAudioData( pih->hPlayer, pFrame->pData, pFrame->dwDataSize ) ;
	}
}

void CALLBACK RealVideoDataCallBack( int nCameraID, LPBYTE pvRealData, DWORD dwRealDataSize, LPVOID pvParam )
{
	internal_handle *pih = (internal_handle*)pvParam;
	if(pih->bRaisCallback && pih->OnStreamReady != NULL)
	{
		pih->OnStreamReady(pih->pUserData,pvRealData,dwRealDataSize,0);
	}

	SY_PARSE_ParserInputData( pih->hParser, pvRealData, dwRealDataSize ) ;
}

static void CALLBACK DecodeVideoCallBack( int nImgWidth, int nImgHeight, LPBYTE pRGBBuffer, DWORD dwImgSize, LPVOID pvParam )
{
	internal_handle *pih = (internal_handle*)pvParam;
	if(pih->bRaisCallback && pih->OnRealRGBReady != NULL)
	{
		if(pih->nStoreWidth == 0)
			pih->nStoreWidth = nImgWidth*3;
		if (pih->OnPixBufReady != NULL)
		{
			pih->OnPixBufReady(pih->pUserData,pRGBBuffer,ZW_PIX_FMT_RGB24,nImgWidth,pih->nStoreWidth,nImgHeight);
		}
		pih->OnRealRGBReady(pih->pUserData,pRGBBuffer,nImgWidth,pih->nStoreWidth,nImgHeight);
	}
}

internal_handle* __cdecl  InternalInitial(VideoParam *param,std::string& ipcode)
{
	internal_handle *pih = new internal_handle();

	pih->hCamera = SY_NET_Login( param->CameraIp, param->CameraPort, param->UserName, param->Password) ;

	if(pih->hCamera <= 0)
		goto err;

	pih->hParser = SY_PARSE_CreateParser();

	if(pih->hParser <= 0)
		goto err;

	SY_NET_GetDeviceSetting( pih->hCamera, SYDS_CODEC_STREAM_STATUS, &pih->status, sizeof( pih->status ) ) ;

	pih->hPlayer = SY_PLAY_CreatePlayer(pih->status);

	if(pih->hPlayer <= 0)
		goto err;	

	SY_PLAY_SetDecodeCallBack( pih->hPlayer, DecodeVideoCallBack,pih);
	SY_PLAY_EnableDecoder( pih->hPlayer, TRUE ) ;
	SY_PARSE_SetParseCallBack( pih->hParser, ParseVideoFrameCallBack,pih ) ;
	SY_PARSE_SetAudioParseCallBack( pih->hParser, ParseAudioFrameCallBack,pih ) ;
	SY_NET_SetRealDataCallBack( pih->hCamera, RealVideoDataCallBack, pih ) ;
	SY_NET_RealPlay( pih->hCamera ) ;
	
	pih->ipcode = ipcode;
	g_mtx.Lock();
	g_handle_map.insert(std::make_pair(ipcode,pih));
	g_mtx.Unlock();
	pih->zmTimer.Start(RestartFunc,(LPVOID)pih,60000,30000,0);
	return pih;

err:
	if(pih->hCamera > 0)
		SY_NET_Logout(pih->hCamera);
	if (pih->hParser > 0 )
		SY_PARSE_CloseParser(pih->hParser);
	if (pih->hPlayer > 0 )
		SY_PLAY_ClosePlayer(pih->hParser);
	delete pih;
	return NULL;
}

void   __cdecl  InternalFinialize(internal_handle* pih)
{
	pih->nRefCount--;
	if(pih->nRefCount != 0)
		return;

	pih->zmTimer.Stop();

	pih->bRaisCallback = false;
	if (pih->hParser > 0 )
		SY_PARSE_CloseParser(pih->hParser);
	if (pih->hPlayer > 0 )
	{
		SY_PLAY_EnableDecoder( pih->hPlayer, FALSE ) ;
		SY_PLAY_ClosePlayer(pih->hParser);
	}
	if(pih->hCamera > 0)
	{
		SY_NET_StopRealPlay(pih->hCamera);
		SY_NET_Logout(pih->hCamera);
	}

	g_mtx.Lock();
	g_handle_map.erase(pih->ipcode);
	g_mtx.Unlock();

	delete pih;
}

/*��ʼ����ƵԴ������ص����ڻ�ȡ�������ⲿ����ץ�ĵ�ͼƬ*/
ZW_VIDEO_API handle __cdecl  InitialVideoSource(VideoParam *param,
											 StreamCallback OnStreamReady,/*ʵʱ��Ƶ���ص�*/
											 PixBufCallback OnPixBufReady,/*������������ص�*/
											 RealTimeRGBCallback OnRealRGBReady,/*ʵʱ����ͼƬ�ص�*/
											 TriggerImageCallback OnTriggerRGBReady,/*�ⲿ����ץ��ͼƬ�ص�*/
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


/*��ȡ��ƵԴ��Ϣ������0ʧ�ܣ�1�ɹ�
** decode = 0��ʾ���ⲿ���룬���ⲿ���н���Ļ�OnRealRGBReady���ᱻ���á���ʱcodec��ֵ������Ч��
** decode = 1��ʾ���ڲ����룬�ڲ������RGBͼ��ͨ��OnRealRGBReady������
*/
ZW_VIDEO_API int   __cdecl  GetVideoInfo(handle hVS,int *decode,int *codec)
{
	internal_handle *pih = (internal_handle*)hVS;

	*decode = 1;
	*codec = ZW_CODEC_ID_NONE;

	return 1;	
}

/*�����豸�Ƿ����Զ�������֧�ֵĻ�����1�����򷵻�0*/
ZW_VIDEO_API int    __cdecl  CanReConnect(handle hVS)
{
	return 0;
}

/*������ƵԴ������0ʧ�ܣ�1�ɹ�*/
ZW_VIDEO_API int    __cdecl  StartVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;
	pih->bRaisCallback = true;
	return 1;
}

/*��ͣ��ƵԴ�����øú�������Ȼ���Ե���StartVideoSource����*/
ZW_VIDEO_API void   __cdecl  StopVideoSource(handle hVS)
{
	internal_handle *pih = (internal_handle*)hVS;
	pih->bRaisCallback = false;
}

/*��ֹ��ƵԴģ��*/
ZW_VIDEO_API void   __cdecl  FinializeVideoSource(handle hVS)
{
	InternalFinialize((internal_handle*)hVS);
}



/*��������PTZ���Ƶľ��*/
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
	int nRet = 1;

	switch (cmd)
	{
	case ZW_ZOOM_IN:
		//pih->nCmd = SYCCT_ZOOM_TELE_ONE_STEP;
		//pih->zmTimer.Start(ZoomFunc,(LPVOID)pih,120,100,0);
		SY_NET_CameraControl(pih->hCamera,SYCCT_ZOOM_IN,1);
		break;
	case ZW_ZOOM_OUT:
		//pih->nCmd = SYCCT_ZOOM_WIDE_ONE_STEP;
		//pih->zmTimer.Start(ZoomFunc,(LPVOID)pih,120,100,0);
		SY_NET_CameraControl(pih->hCamera,SYCCT_ZOOM_OUT,1);
		break;
	case ZW_STOP_ZOOM:
		//pih->nCmd = SYCCT_ZOOM_STOP;
		//pih->zmTimer.Stop();
		SY_NET_CameraControl(pih->hCamera,SYCCT_ZOOM_STOP,0);
		break;
	case ZW_FOCUS_NEAR:
		SY_NET_CameraControl(pih->hCamera,SYCCT_FOCUS,0);
		break;
	case ZW_FOCUS_FAR:
		SY_NET_CameraControl(pih->hCamera,SYCCT_FOCUS,1);
		break;
	case ZW_STOP_FOCUS:
		SY_NET_CameraControl(pih->hCamera,SYCCT_FOCUS,2);
		break;
	case ZW_IRIS_OPEN:
		SY_NET_CameraControl(pih->hCamera,SYCCT_IRIS,0);
		break;
	case ZW_IRIS_CLOSE:
		SY_NET_CameraControl(pih->hCamera,SYCCT_IRIS,1);
		break;
	case ZW_STOP_IRIS:
		break;
	case ZW_USE_PRESET:
		SY_NET_CameraControl(pih->hCamera,SYCCT_PRESET_GOTO,param1);
		break;
	case ZW_SET_PRESET:
		SY_NET_CameraControl(pih->hCamera,SYCCT_PRESET_REGISTRY,param1);
		break;
	case ZW_DEL_PRESET:
		SY_NET_CameraControl(pih->hCamera,SYPTZ_PRESET_REMOVE,param1);
		break;
	case ZW_ZOOM_STEP://�����ƽ����sdk��ʵ������Ϊ�������
		SY_NET_CameraControl(pih->hCamera,SYCCT_ZOOM_TELE_ONE_STEP,param1);
		break;
	default:
		nRet = -1;
		break;
	}

	return -1;
}


/*�����豸�Ƿ�֧��3D��λ��֧�ֵĻ�����1�����򷵻�0*/
ZW_PTZ_API int    __cdecl CanDirectTo(handle hPTZ)
{
	return 0;
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
	InternalFinialize((internal_handle*)hPTZ);
}
