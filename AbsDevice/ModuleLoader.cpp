#include "StdAfx.h"
#include "ModuleLoader.h"
#include "PTZ.h"

CMap<CString, CString&,ModuleLoader*, ModuleLoader*>  ModuleLoader::m_sLoaders;

/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
/*捕获的异常值的定义参考PTZ.h文件中ERR_MSG的定义*/
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
	/*在调用结束接口后，某些设备接入sdk中的线程不能正常退出。*/
	/*卸载掉dll会导致程序崩溃，所以module一旦载入就不卸载了*/
	//if(m_nRefCount == 0)
	//{
	//	ModuleLoader::m_sLoaders.RemoveKey(m_strPluginPath);
	//	delete this;
	//}
}

/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
/*捕获的异常值的定义参考PTZ.h文件中ERR_MSG的定义*/
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
