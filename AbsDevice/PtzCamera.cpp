#include "StdAfx.h"


#include "PtzCamera.h"
#include "PTZActivator.h"
#include "VideoActivator.h"
#include "ConfigManager.h"
#include <process.h>
#include "ping.h"
#include "ThreadMutex.h"

#define DECODE_BUFFER_SIZE 100000

CMap<int, int,PtzCamera*, PtzCamera*>  PtzCamera::m_sPtzCameras;
ThreadMutex  g_mtx;

/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
/*������쳣ֵ�Ķ���ο�Video.h�ļ���ERR_MSG�Ķ���*/
/*��ptzParam����NULLʱ������PTZService����*/
/*��videoParam����NULLʱ������VideoService����*/
/*idΪ��̨�����Ų�ͬ�豸��id��ͬ*/
PtzCamera* PtzCamera::GetInstance(
	int id,/*��̨������*/
	ZWPTZParam* ptzParam,/*PTZ����*/
	ZWVideoParam* videoParam,/*��ƵԴ����*/
	VideoStreamCallback OnVideoStreamReady,/*ʵʱ��Ƶ���ص�*/
	RealTimeImageCallback OnRealImgReady,/*ʵʱͼ������ص�*/
	RealTimeImageCallback OnRealFixFmtImgReady,/*ʵʱת��ͼ������ص�*/
	int realFixFmt,/*ʵʱת��ͼ��ĸ�ʽ*/
	TriggerImageCallback OnTriggerImgReady,/*�ⲿ����ץ��ͼƬ�ص�*/
	TriggerImageCallback OnTriggerFixFmtImgReady,/*�ⲿ����ץ��ת��ͼƬ�ص�*/
	int triggerFixFmt,/*�ⲿ����ץ��ת��ͼƬ�ĸ�ʽ*/
	void *pUserData /*= NULL /*�û�����*/
	)
{
	PtzCamera* ins = NULL;
	bool insNew = true;

	g_mtx.Lock();
	if (PtzCamera::m_sPtzCameras.Lookup(id,ins))
		insNew = false;
	else
		ins = new PtzCamera(id);

	if (ptzParam != NULL)
	{
		try
		{
			ins->CreatePtzPart(ptzParam,videoParam);
		}
		catch (int &err)
		{
			if(insNew) delete ins;
			g_mtx.Unlock();
			throw err;
		}		
	}

	if (videoParam != NULL)
	{
		try
		{
			ins->CreateVideoPart(videoParam,
				OnVideoStreamReady,
				OnRealImgReady,
				OnRealFixFmtImgReady,
				realFixFmt,
				OnTriggerImgReady,
				OnTriggerFixFmtImgReady,
				triggerFixFmt,
				pUserData);
		}
		catch (int &err)
		{
			if(insNew) delete ins;
			g_mtx.Unlock();
			throw err;
		}	
	}

	if(ins != NULL)
		ins->m_nRefCount++;

	if(insNew)
		PtzCamera::m_sPtzCameras[id] = ins;
	g_mtx.Unlock();

	return ins;
}

void   PtzCamera::ReleaseInstance()
{
	g_mtx.Lock();
	if(m_nRefCount != 0)
		m_nRefCount--;
	if(m_nRefCount == 0)
	{
		PtzCamera::m_sPtzCameras.RemoveKey(m_nId);
		delete this;
	}
	g_mtx.Unlock();
}

PtzCamera::PtzCamera(int id)
{
	m_nId = id;
	m_nRefCount = 0;
	m_bPtzPart = false;
	m_bVideoPart = false;
	m_bVideoPtz1 = false;
	m_bVideoPtz2 = false;

	m_pPTZActivator = NULL;
	m_pPTZActivator2 = NULL;
	m_hPTZ = NULL;
	m_hPTZ2 = NULL;

	m_pVideoActivator = NULL;
	m_hReconnectThread = NULL;	
	m_bVideoStarted = false;
	m_bManualStarted = false;
	m_bRunReconnect = false;

	m_pfnOnVideoStreamReady = NULL;
	m_pfnOnRealImgReady = NULL;
	m_pfnOnRealFixFmtImgReady = NULL;
	m_pfnOnTriggerImgReady = NULL;
	m_pfnOnTriggerFixFmtImgReady = NULL;
	m_nRealFixFmt = ZW_PIX_FMT_NONE;
	m_nTriggerFixFmt = ZW_PIX_FMT_NONE;
	m_pUserData = NULL;

	m_hPlayWnd = NULL;

	m_hVideo = NULL;
}

PtzCamera::~PtzCamera(void)
{
	if (m_bVideoPart)
	{
		if(m_bRunReconnect)
		{
			m_bRunReconnect = false;
			WaitForSingleObject(m_hReconnectThread,INFINITE);
			CloseHandle(m_hReconnectThread);
		}

		FinializeVideoSource();

		DeleteCriticalSection(&m_csInitial);

		((VideoActivator*)m_pVideoActivator)->ReleaseInstance();		
	}

	if (m_bPtzPart)
	{
		((PTZActivator*)m_pPTZActivator)->m_pfnFinializePTZ(m_hPTZ);
		((PTZActivator*)m_pPTZActivator)->ReleaseInstance();
		if(m_pPTZActivator2 != NULL)
		{
			((PTZActivator*)m_pPTZActivator2)->m_pfnFinializePTZ(m_hPTZ2);
			((PTZActivator*)m_pPTZActivator2)->ReleaseInstance();
		}
	}
}

void   PtzCamera::CreatePtzPart(ZWPTZParam* ptzParam, ZWVideoParam* videoParam)
{
	if (m_bPtzPart)
		return;
	if(ptzParam != NULL)
		memcpy(&m_PTZParam, ptzParam,sizeof(ZWPTZParam));

	ConfigManager& pConf = ConfigManager::GetInstance();
	
	PTZActivator* activator = PTZActivator::GetInstance(pConf.PTZPlugin.GetBuffer());
	pConf.PTZPlugin.ReleaseBuffer();
	m_hPTZ = activator->m_pfnInitialPTZ(ptzParam, NULL);

	if(m_hPTZ == NULL)
	{
		activator->ReleaseInstance();
		throw ZW_ERR_PTZ1_FAULT;
	}
	else
	{
		m_pPTZActivator = activator;
	}

	if(!pConf.PTZPlugin2.IsEmpty())
	{
		PTZActivator* activator2  = PTZActivator::GetInstance(pConf.PTZPlugin2.GetBuffer());
		pConf.PTZPlugin2.ReleaseBuffer();
		m_hPTZ2 = activator2->m_pfnInitialPTZ(ptzParam, videoParam);
		
		if(m_hPTZ2 == NULL)
		{
			activator->m_pfnFinializePTZ(m_hPTZ);
			activator->ReleaseInstance();
			activator2->ReleaseInstance();
			throw ZW_ERR_PTZ2_FAULT;
		}
		else
		{
			m_pPTZActivator2 = activator2;
			m_bVideoPtz2 = (m_hPTZ2 == m_hVideo);
		}
	}

	m_bVideoPtz1 = (m_hPTZ == m_hVideo);
	m_bPtzPart = true;
}

void   PtzCamera::CreateVideoPart(
	ZWVideoParam* param,/*��ƵԴ����*/
	VideoStreamCallback OnVideoStreamReady,/*ʵʱ��Ƶ���ص�*/
	RealTimeImageCallback OnRealImgReady,/*ʵʱͼ������ص�*/
	RealTimeImageCallback OnRealFixFmtImgReady,/*ʵʱת��ͼ������ص�*/
	int realFixFmt,/*ʵʱת��ͼ��ĸ�ʽ*/
	TriggerImageCallback OnTriggerImgReady,/*�ⲿ����ץ��ͼƬ�ص�*/
	TriggerImageCallback OnTriggerFixFmtImgReady,/*�ⲿ����ץ��ת��ͼƬ�ص�*/
	int triggerFixFmt,/*�ⲿ����ץ��ת��ͼƬ�ĸ�ʽ*/
	void *pUserData /* = NULL*/ /*�û�����*/)
{
	m_pfnOnVideoStreamReady = OnVideoStreamReady;
	m_pfnOnRealImgReady = OnRealImgReady;
	m_pfnOnRealFixFmtImgReady = OnRealFixFmtImgReady;
	m_pfnOnTriggerImgReady = OnTriggerImgReady;
	m_pfnOnTriggerFixFmtImgReady = OnTriggerFixFmtImgReady;
	m_nRealFixFmt = realFixFmt;
	m_nTriggerFixFmt = triggerFixFmt;
	m_pUserData = pUserData;

	if (m_bVideoPart)
		return;

	memcpy(&m_VideoParam,param,sizeof(ZWVideoParam));
	ConfigManager& pConf = ConfigManager::GetInstance();

	InitializeCriticalSection(&m_csInitial);

	VideoActivator* activator = VideoActivator::GetInstance(pConf.VideoPlugin.GetBuffer());
	pConf.VideoPlugin.ReleaseBuffer();
	m_pVideoActivator = activator; 

	if(!InitialVideoSource())
	{
		m_pVideoActivator = NULL;
		activator->ReleaseInstance();
		throw ZW_ERR_VIDEO_FAULT;
	}
	else
	{
		if(activator->m_pfnCanReConnect != NULL && !activator->m_pfnCanReConnect(m_hVideo))
		{
			m_bRunReconnect = true;
			m_hReconnectThread = (HANDLE)_beginthreadex(NULL,0,PtzCamera::ReconnectThreadFunc,
				(LPVOID)this,0,NULL);
		}

		m_bVideoPtz1 = (m_hPTZ == m_hVideo);
		m_bVideoPtz2 = (m_hPTZ2 == m_hVideo);
	}

	m_bVideoPart = true;
}

/*ִ��PTZָ�param1-pan speed������������speed��param2-tilt speed
**pan��tilt��speedΪ1-64������������speedΪ1-8
**�������������֧��speedֱ�Ӻ���
**��cmdΪZW_ZOOM_STEPʱ��param1Ϊ��ֵ��ʾ�ӵ�ǰλ��ZW_ZOOM_OUT���ټ�����ֵ��ʾZW_ZOOM_IN���ټ���
**
**
**���� 1-��ʾִ�гɹ� 0-��ʾִ��ʧ�� -1-��ʾ��֧�ֵĲ���
*/
int   PtzCamera::ExecPTZ(int cmd,int param1,int param2)
{
	int ret = 0;
	if(m_bPtzPart)
	{
		ret = ((PTZActivator*)m_pPTZActivator)->m_pfnExecPTZ(m_hPTZ,cmd,param1,param2);
		if(m_pPTZActivator2 != NULL && cmd > ZW_RIGHTDOWN)
			((PTZActivator*)m_pPTZActivator2)->m_pfnExecPTZ(m_hPTZ2,cmd,param1,param2);
	}

	return ret;
}

/*�����豸�Ƿ�֧��3D��λ��֧�ֵĻ�����1�����򷵻�0*/
int    PtzCamera::CanDirectTo()
{
	if(m_bPtzPart)
		return ((PTZActivator*)m_pPTZActivator)->m_pfnCanDirectTo(m_hPTZ);
	else
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
int   PtzCamera::DirectTo(int zoom,int vw,int vh,int rx, int ry, int rw,int rh)
{
	int ret = 0;
	if(m_bPtzPart)
	{
		ret = ((PTZActivator*)m_pPTZActivator)->m_pfnDirectTo(m_hPTZ,zoom,vw,vh,rx,ry,rw,rh);
		if(m_pPTZActivator2 != NULL)
			((PTZActivator*)m_pPTZActivator2)->m_pfnDirectTo(m_hPTZ2,zoom,vw,vh,rx,ry,rw,rh);
	}

	return ret;
}


bool PtzCamera::InitialVideoSource()
{
	bool ret = true;

	m_hVideo = ((VideoActivator*)m_pVideoActivator)->m_pfnInitialVideoSource(&m_VideoParam,
		OnVideoStreamReadyProc,
		OnRealImgReadyProc,
		OnRealFixFmtImgReadyProc,
		m_nRealFixFmt,
		OnTriggerImgReadyProc,
		m_pfnOnTriggerFixFmtImgReady,
		m_nTriggerFixFmt,
		(void*)this);

	if(m_hVideo == NULL) 
	{
		ret = false;
	}

	return ret;
}

void PtzCamera::FinializeVideoSource()
{
	if(m_hVideo != NULL) 
	{
		((VideoActivator*)m_pVideoActivator)->m_pfnFinializeVideoSource(m_hVideo);
		m_hVideo = NULL;
	}
}

bool PtzCamera::InitialPtzDevice()
{
	if (m_pPTZActivator != NULL)
	{
		m_hPTZ = ((PTZActivator*)m_pPTZActivator)->m_pfnInitialPTZ(&m_PTZParam, NULL);
	}
	if (m_hPTZ == NULL) return false;

	if(m_pPTZActivator2 != NULL)
	{
		m_hPTZ2 = ((PTZActivator*)m_pPTZActivator2)->m_pfnInitialPTZ(&m_PTZParam, &m_VideoParam);
		if(m_hPTZ2 == NULL)
		{
			((PTZActivator*)m_pPTZActivator)->m_pfnFinializePTZ(m_hPTZ);
			return false;
		}
		m_bVideoPtz2 = (m_hPTZ2 == m_hVideo);
	}

	m_bVideoPtz1 = (m_hPTZ == m_hVideo);
	m_bPtzPart = true;

	return true;
}

void PtzCamera::FinializePtzDevice()
{
	m_bPtzPart = false;
	if(m_hPTZ != NULL)
	{
		((PTZActivator*)m_pPTZActivator)->m_pfnFinializePTZ(m_hPTZ);
		m_hPTZ = NULL;
	}
	if(m_pPTZActivator2 != NULL && m_hPTZ2 != NULL)
	{
		((PTZActivator*)m_pPTZActivator2)->m_pfnFinializePTZ(m_hPTZ2);
		m_hPTZ2 = NULL;
	}
}

int  PtzCamera::GetCodec()
{
	if(m_nCodec < 0)
		m_nCodec = ((VideoActivator*)m_pVideoActivator)->m_pfnGetVideoCodec(m_hVideo);

	return m_nCodec;
}

int PtzCamera::TriggerPeripheral(int func)
{
	int ret = 0;
	VideoActivator* pVideoActivator = (VideoActivator*)m_pVideoActivator;
	if(pVideoActivator->m_pfnTriggerPeripheral != NULL)
		ret = pVideoActivator->m_pfnTriggerPeripheral(m_hVideo,func);
	return ret;
}

int  PtzCamera::StartVideoSource(handle hPlayWnd)
{
	int ret = 0;
	if(m_bVideoPart)
	{
		EnterCriticalSection(&m_csInitial);
		ret = StartVideoSource(true, hPlayWnd);
		LeaveCriticalSection(&m_csInitial);
	}

	return ret;
}

void  PtzCamera::StopVideoSource()
{
	if(m_bVideoPart)
	{
		EnterCriticalSection(&m_csInitial);
		StopVideoSource(true);
		LeaveCriticalSection(&m_csInitial);
	}
}

int  PtzCamera::StartVideoSource(bool manual,handle hPlayWnd)
{
	int ret = 0;

	if (!m_bVideoStarted)
	{
		ret = ((VideoActivator*)m_pVideoActivator)->m_pfnStartVideoSource(m_hVideo, hPlayWnd);
		m_hPlayWnd = hPlayWnd;
	}
	m_tmtLastRecvVideo = time(NULL);
	m_bVideoStarted = true;
	if(manual) m_bManualStarted = true;

	return ret;
}

void  PtzCamera::StopVideoSource(bool manual)
{
	if(m_bVideoStarted)
		((VideoActivator*)m_pVideoActivator)->m_pfnStopVideoSource(m_hVideo);
	m_bVideoStarted = false;
	if(manual) m_bManualStarted = false;
}

UINT PtzCamera::ReconnectThreadFunc(LPVOID lpParam)
{
	PtzCamera* pService = (PtzCamera*)lpParam;

	while(pService->m_bRunReconnect)
	{
		time_t now = time(NULL);
		if(pService->m_bManualStarted)
		{
			/*�������30��û���յ�����*/
			if((now-pService->m_tmtLastRecvVideo) > 10)
			{
				bool go = true;
				if(strcmp(pService->m_VideoParam.CameraIp,_T("")) != 0)
				{
					try
					{
						VCEXT::CPing p1;
						VCEXT::CPingReply pr1;
						if (!p1.Ping1(pService->m_VideoParam.CameraIp, pr1))
						{
							go = false;
						}
					}
					catch(CException *e)
					{
						e->Delete();
						go = false;
					}
					catch(...)
					{
						go = false;
					}
				}

				if(go)
				{
					EnterCriticalSection(&pService->m_csInitial);
					pService->StopVideoSource(false);
					pService->FinializeVideoSource();
					pService->FinializePtzDevice();
					Sleep(10);
					if(pService->InitialVideoSource())
					{
						pService->StartVideoSource(false,pService->m_hPlayWnd);
						pService->InitialPtzDevice();
					}
					LeaveCriticalSection(&pService->m_csInitial);
				}
			}
		}
		Sleep(500);
	}

	return 0;
}

void  PtzCamera::OnVideoStreamReadyProc(void *pUserData, void* buff, long size, int frametype)
{
	//static FILE *fp = fopen("d:\\1.264","wb");
	//fwrite(buff,size,1,fp);

	PtzCamera* pvs = (PtzCamera*)pUserData;

	pvs->m_tmtLastRecvVideo = time(NULL);

	if (pvs->m_pfnOnVideoStreamReady != NULL)
		pvs->m_pfnOnVideoStreamReady(pvs->m_pUserData, buff, size, frametype);
}

void  PtzCamera::OnTriggerImgReadyProc(void *pUserData, ZWImageData* img, void* data, int func)
{
	PtzCamera* pvs = (PtzCamera*)pUserData;
	if(pvs->m_pfnOnTriggerImgReady != NULL)
		pvs->m_pfnOnTriggerImgReady(pvs->m_pUserData, img, data, func);
}

void  PtzCamera::OnTriggerFixFmtImgReadyProc(void *pUserData, ZWImageData* img, void* data, int func)
{
	PtzCamera* pvs = (PtzCamera*)pUserData;
	if (pvs->m_pfnOnTriggerFixFmtImgReady != NULL)
		pvs->m_pfnOnTriggerFixFmtImgReady(pvs->m_pUserData, img, data, func);
}

void PtzCamera::OnRealImgReadyProc( void *pUserData, ZWImageData* img)
{
	PtzCamera* pvs = (PtzCamera*)pUserData;
	if(pvs->m_pfnOnRealImgReady != NULL)
		pvs->m_pfnOnRealImgReady(pvs->m_pUserData,img);
}

void  PtzCamera::OnRealFixFmtImgReadyProc(void *pUserData, ZWImageData* img)
{
	PtzCamera* pvs = (PtzCamera*)pUserData;
	pvs->m_tmtLastRecvVideo = time(NULL);
	if (pvs->m_pfnOnRealFixFmtImgReady != NULL)
		pvs->m_pfnOnRealFixFmtImgReady(pvs->m_pUserData, img);
}
