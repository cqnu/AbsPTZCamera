#include "StdAfx.h"
#include "PTZActivator.h"

CMap<CString, CString&,PTZActivator*, PTZActivator*>  PTZActivator::m_sActivators;

/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
/*捕获的异常值的定义参考PTZ.h文件中ERR_MSG的定义*/
PTZActivator* PTZActivator::GetInstance(LPCTSTR lpszPluginPath)
{
	PTZActivator* ins = NULL;
	CString key(lpszPluginPath);

	if(!PTZActivator::m_sActivators.Lookup(key,ins))
	{
		ins = new PTZActivator(lpszPluginPath);
		try
		{			
			PTZActivator::m_sActivators[key] = ins;
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


void PTZActivator::ReleaseInstance()
{
	if(m_nRefCount != 0)
		m_nRefCount--;
	if(m_nRefCount == 0)
	{
		PTZActivator::m_sActivators.RemoveKey(m_strPluginPath);
		delete this;
	}
}

/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
/*捕获的异常值的定义参考PTZ.h文件中ERR_MSG的定义*/
PTZActivator::PTZActivator(LPCTSTR lpszPluginPath)
{
	m_nRefCount = 0;
	m_strPluginPath = lpszPluginPath;
	m_pModuleLoader = ModuleLoader::GetInstance(lpszPluginPath);
	m_hModule = m_pModuleLoader->GetModule();
	m_pfnInitialPTZ = reinterpret_cast<INITIAL_PTZ_PROC>(::GetProcAddress(m_hModule, "InitialPTZ"));
	m_pfnExecPTZ = reinterpret_cast<EXEC_PTZ_PROC>(::GetProcAddress(m_hModule, "ExecPTZ"));
	m_pfnCanDirectTo = reinterpret_cast<CAN_DIRECT_TO_PROC>(::GetProcAddress(m_hModule, "CanDirectTo"));
	m_pfnDirectTo = reinterpret_cast<DIRECT_TO_PROC>(::GetProcAddress(m_hModule, "DirectTo"));
	m_pfnFinializePTZ = reinterpret_cast<FINIALIZE_PTZ_PROC>(::GetProcAddress(m_hModule, "FinializePTZ"));

	if(m_pfnInitialPTZ==NULL || m_pfnExecPTZ==NULL || m_pfnCanDirectTo==NULL || m_pfnDirectTo==NULL || m_pfnFinializePTZ==NULL)
	{
		m_pModuleLoader->ReleaseInstance();
		throw ZW_ERR_PTZ_ADDIN_FAULT;
	}
}

PTZActivator::~PTZActivator(void)
{
	if (m_pModuleLoader != NULL)
		m_pModuleLoader->ReleaseInstance();
}
