#include "StdAfx.h"
#include "VideoActivator.h"


CMap<CString, CString&,VideoActivator*, VideoActivator*>  VideoActivator::m_sActivators;

/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
/*������쳣ֵ�Ķ���ο�Video.h�ļ���ERR_MSG�Ķ���*/
VideoActivator* VideoActivator::GetInstance(LPCTSTR lpszPluginPath)
{
	VideoActivator* ins = NULL;
	CString key(lpszPluginPath);

	if(!VideoActivator::m_sActivators.Lookup(key,ins))
	{
		ins = new VideoActivator(lpszPluginPath);
		try
		{			
			VideoActivator::m_sActivators[key] = ins;
		}
		catch(CException* e)
		{
			e->Delete();
			ins->ReleaseInstance();
			ins = NULL;

			throw ZW_ERR_NOT_DEFINED;
		}
	}

	if(ins != NULL)
		ins->m_nRefCount++;

	return ins;
}


void VideoActivator::ReleaseInstance()
{
	if(m_nRefCount != 0)
		m_nRefCount--;
	if(m_nRefCount == 0)
	{
		VideoActivator::m_sActivators.RemoveKey(m_strPluginPath);
		delete this;
	}
}

/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
/*������쳣ֵ�Ķ���ο�Video.h�ļ���ERR_MSG�Ķ���*/
VideoActivator::VideoActivator(LPCTSTR lpszPluginPath)
{
	m_nRefCount = 0;
	m_strPluginPath = lpszPluginPath;
	m_pModuleLoader = ModuleLoader::GetInstance(lpszPluginPath);
	m_hModule = m_pModuleLoader->GetModule();
	m_pfnInitialVideoSource = reinterpret_cast<INITIAL_VIDEO_SOURCE_PROC>(::GetProcAddress(m_hModule, "InitialVideoSource"));
	m_pfnGetVideoCodec = reinterpret_cast<GET_VIDEO_CODEC_PROC>(::GetProcAddress(m_hModule, "GetVideoCodec"));
	m_pfnCanReConnect = reinterpret_cast<CAN_RECONNECT_PROC>(::GetProcAddress(m_hModule, "CanReConnect"));
	m_pfnTriggerPeripheral = reinterpret_cast<TRIGGER_PERIPHERAL_PROC>(::GetProcAddress(m_hModule, "TriggerPeripheral"));
	m_pfnStartVideoSource = reinterpret_cast<START_VIDEO_SOURCE_PROC>(::GetProcAddress(m_hModule, "StartVideoSource"));
	m_pfnStopVideoSource = reinterpret_cast<STOP_VIDEO_SOURCE_PROC>(::GetProcAddress(m_hModule, "StopVideoSource"));
	m_pfnFinializeVideoSource = reinterpret_cast<FINIALIZE_VIDEO_SOURCE_PROC>(::GetProcAddress(m_hModule, "FinializeVideoSource"));

	if(m_pfnInitialVideoSource==NULL
		|| m_pfnGetVideoCodec==NULL
		|| m_pfnStartVideoSource==NULL
		|| m_pfnStopVideoSource==NULL
		|| m_pfnCanReConnect==NULL
		|| m_pfnTriggerPeripheral==NULL
		|| m_pfnFinializeVideoSource==NULL)
	{
		m_pModuleLoader->ReleaseInstance();
		throw ZW_ERR_PTZ_ADDIN_FAULT;
	}
}

VideoActivator::~VideoActivator(void)
{
	if (m_pModuleLoader != NULL)
		m_pModuleLoader->ReleaseInstance();
}
