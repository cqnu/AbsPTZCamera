#include "StdAfx.h"
#include "ConfigManager.h"

ConfigManager ConfigManager::m_instance;

ConfigManager& ConfigManager::GetInstance()
{
	return ConfigManager::m_instance;
}

ConfigManager::ConfigManager(void)
{
	try
	{
		::GetModuleFileName(NULL,m_strModulePath.GetBuffer(MAX_PATH),MAX_PATH);
		m_strModulePath.ReleaseBuffer();
		::PathRemoveFileSpec(m_strModulePath.GetBuffer());
		m_strModulePath.ReleaseBuffer();
		m_strModulePath += _T("\\");

		CString configPath = m_strModulePath+_T("ptzv.ini");
		TCHAR buffer[MAX_PATH];

		memset(buffer,0,MAX_PATH);
		::GetPrivateProfileString(_T("addin"),_T("VideoPlugin"),_T(""),buffer,MAX_PATH,configPath);

		if(StrCmp(buffer,_T("")) != 0)
		{
			VideoPlugin = m_strModulePath + buffer;
		}

		memset(buffer,0,MAX_PATH);
		::GetPrivateProfileString(_T("addin"),_T("PTZPlugin"),_T(""),buffer,MAX_PATH,configPath);

		if(StrCmp(buffer,_T("")) != 0)
		{
			PTZPlugin = m_strModulePath + buffer;
		}

		memset(buffer,0,MAX_PATH);
		::GetPrivateProfileString(_T("addin"),_T("PTZPlugin2"),_T(""),buffer,MAX_PATH,configPath);

		if(StrCmp(buffer,_T("")) != 0)
		{
			PTZPlugin2 = m_strModulePath + buffer;
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
	}
}

ConfigManager::~ConfigManager(void)
{
}
