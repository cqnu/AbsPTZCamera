#include "StdAfx.h"
#include "ModuleLoader.h"
#include "PTZ.h"

CMap<CString, CString&,ModuleLoader*, ModuleLoader*>  ModuleLoader::m_sLoaders;

/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
/*������쳣ֵ�Ķ���ο�PTZ.h�ļ���ERR_MSG�Ķ���*/
ModuleLoader* ModuleLoader::GetInstance(LPCTSTR lpszPluginPath)
{
	ModuleLoader* ins = NULL;
	CString key(lpszPluginPath);

	if(!ModuleLoader::m_sLoaders.Lookup(key,ins))
	{
		ins = new ModuleLoader(lpszPluginPath);
		try
		{			
			ModuleLoader::m_sLoaders[key] = ins;
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

HMODULE ModuleLoader::GetModule()
{
	return m_hModule;
}

void ModuleLoader::ReleaseInstance()
{
	if(m_nRefCount != 0)
		m_nRefCount--;
	/*�ڵ��ý����ӿں�ĳЩ�豸����sdk�е��̲߳��������˳���*/
	/*ж�ص�dll�ᵼ�³������������moduleһ������Ͳ�ж����*/
	//if(m_nRefCount == 0)
	//{
	//	ModuleLoader::m_sLoaders.RemoveKey(m_strPluginPath);
	//	delete this;
	//}
}

/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
/*������쳣ֵ�Ķ���ο�PTZ.h�ļ���ERR_MSG�Ķ���*/
ModuleLoader::ModuleLoader(LPCTSTR lpszPluginPath)
{
	m_nRefCount = 0;
	m_strPluginPath = lpszPluginPath;
	m_strPluginPath.Trim();

	if(m_strPluginPath.IsEmpty())
		throw ZW_ERR_PATH_IS_EMPTY;

	if(!::PathFileExists(lpszPluginPath))
		throw ZW_ERR_PATH_NOT_EXIST;

	m_hModule = LoadLibraryEx(lpszPluginPath,NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
	if(m_hModule == NULL)
		throw ZW_ERR_LOAD_DLL_FAIL;
}

ModuleLoader::~ModuleLoader(void)
{
	if (m_hModule != NULL)
		FreeLibrary(m_hModule);
}
