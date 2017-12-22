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
	StreamCallback OnStreamReady;/*ʵʱ��Ƶ���ص�*/
	PixBufCallback OnPixBufReady;/*������������ص�*/
	RealTimeRGBCallback OnRealRGBReady;  /*ʵʱ����ͼƬ�ص�*/
	TriggerImageCallback OnTriggerRGBReady;  /*�ⲿ����ץ��ͼƬ�ص�*/
	bool  bRaisCallback;
	void* pUserData;
	int   nRefCount;
	std::string ipcode;

	SwsContext* pCtx;
	char* pRGBBuffer;
	long lUserHandle; //�û����

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
int g_nInternal_UserNum = 0; //�û�����

/*ʵʱ��Ƶ���ص�*/
long CALLBACK iveInternal_StreamReadyCB( long lHandle, const char * pBuf, long  nSize, RAW_INFO * pDataInfo, long lParam  )
{
	internal_handle *pih = (internal_handle*)lParam;

	if(pih == NULL || !pih->bRaisCallback)
	{
		return -1;
	}
	//1			CODEC_ID_H264				H264��Ƶ��������
	//2			CODEC_ID_MPEG4				MPEG4��Ƶ��������
	//3			CODEC_ID_MJPEG				MJPEG��Ƶ��������
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

/*ʵʱ����RGB�ص�*/
long CALLBACK iveInternal_RealRGBReadyCB( long hHandle, const char * pBuf, long  nSize, FRAME_INFO * pFrameInfo, long lParam  )
{
	internal_handle *pih = (internal_handle*)lParam;

	if(!pih->bRaisCallback)
	{
		return 0;
	}

	//2	RGB32		��Ƶ��		ÿ����4�ֽڣ����з�ʽ��λͼ���ƣ�"BGR0",��һ������λ��ͼ�����½�
	//3	YV12		��Ƶ��		yv12��ʽ������˳��"Y0Y1����"��"V0V1����"��"U0-U1����"
	//4	UYVY		��Ƶ��		uyvy��ʽ������˳��"U0Y0V0Y1U2Y2V2Y3 �� ��"����һ������λ��ͼ�����Ͻ�
	//5	YUV420		��Ƶ��		YUV420��ʽ������˳��"Y0Y1����"��"U0-U1����"��"V0V1����"
	//6	yuy2��yuyv	��Ƶ��		yuy2��yuyv��ʽ������˳��"Y0 U0 Y1 V0 Y2 U2 Y3 V2�� ��"����һ������λ��ͼ�����Ͻǡ�
	switch(pFrameInfo->nType)
	{
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:/*ע�����Գ����ĵ����ĵ�����5ʵ������yv12*/
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

	//��ʼ������
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

	//�û���¼
	nRet = NP_StartPreview(param->CameraIp, param->AddressName, param->CameraPort, 1, param->UserName, param->Password, (long)0,(long)0, &(pih->lUserHandle));
	if (0 != nRet)
	{
		delete pih;
		return NULL;
	}

	//����Ƶ�������ݻص�����
	nRet = NP_SetDecodeCallBack(iveInternal_RealRGBReadyCB,(long)pih, pih->lUserHandle);
	//����Ƶԭʼ�����ص�����
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
	if (--g_nInternal_UserNum == 0) //�����û�����Ϊ0���ͷŷ���
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
ZW_PTZ_LOCAL void   __cdecl  StopVideoSource(handle hVS)
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
		pih = InternalInitial(&vparam,addr,ipcode);
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

/*��������PTZ���Ƶľ��*/
ZW_PTZ_API void   __cdecl FinializePTZ(handle hPTZ)
{
	InternalFinialize((internal_handle*)hPTZ);
}
