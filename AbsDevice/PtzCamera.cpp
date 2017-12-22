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

/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
/*捕获的异常值的定义参考Video.h文件中ERR_MSG的定义*/
/*当ptzParam等于NULL时不创建PTZService部分*/
/*当videoParam等于NULL时不创建VideoService部分*/
/*id为云台相机编号不同设备的id不同*/
PtzCamera* PtzCamera::GetInstance(
	int id,/*云台相机编号*/
	ZWPTZParam* ptzParam,/*PTZ参数*/
	ZWVideoParam* videoParam,/*视频源参数*/
	VideoStreamCallback OnVideoStreamReady,/*实时视频流回调*/
	RealTimeImageCallback OnRealImgReady,/*实时图像输出回调*/
	RealTimeImageCallback OnRealFixFmtImgReady,/*实时转码图像输出回调*/
	int realFixFmt,/*实时转码图像的格式*/
	TriggerImageCallback OnTriggerImgReady,/*外部触发抓拍图片回调*/
	TriggerImageCallback OnTriggerFixFmtImgReady,/*外部触发抓拍转码图片回调*/
	int triggerFixFmt,/*外部触发抓拍转码图片的格式*/
	void *pUserData /*= NULL /*用户数据*/
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
	ZWVideoParam* param,/*视频源参数*/
	VideoStreamCallback OnVideoStreamReady,/*实时视频流回调*/
	RealTimeImageCallback OnRealImgReady,/*实时图像输出回调*/
	RealTimeImageCallback OnRealFixFmtImgReady,/*实时转码图像输出回调*/
	int realFixFmt,/*实时转码图像的格式*/
	TriggerImageCallback OnTriggerImgReady,/*外部触发抓拍图片回调*/
	TriggerImageCallback OnTriggerFixFmtImgReady,/*外部触发抓拍转码图片回调*/
	int triggerFixFmt,/*外部触发抓拍转码图片的格式*/
	void *pUserData /* = NULL*/ /*用户数据*/)
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

/*执行PTZ指令。param1-pan speed和其它操作的speed，param2-tilt speed
**pan和tilt的speed为1-64，其它操作的speed为1-8
**其它操作如果不支持speed直接忽略
**当cmd为ZW_ZOOM_STEP时，param1为负值表示从当前位置ZW_ZOOM_OUT多少级别，正值表示ZW_ZOOM_IN多少级别
**
**
**返回 1-表示执行成功 0-表示执行失败 -1-表示不支持的操作
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

/*返回设备是否支持3D定位。支持的话返回1，否则返回0*/
int    PtzCamera::CanDirectTo()
{
	if(m_bPtzPart)
		return ((PTZActivator*)m_pPTZActivator)->m_pfnCanDirectTo(m_hPTZ);
	else
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
			/*如果超过30秒没有收到数据*/
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
