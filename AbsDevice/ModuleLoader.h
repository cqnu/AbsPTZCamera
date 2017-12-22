#pragma once

class ModuleLoader
{
public:
	/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
	/*������쳣ֵ�Ķ���ο�PTZ.h�ļ���ERR_MSG�Ķ���*/
	static ModuleLoader* GetInstance(LPCTSTR lpszPluginPath);
    HMODULE GetModule();
	void   ReleaseInstance();

private:
	static CMap<CString, CString&,ModuleLoader*, ModuleLoader*> m_sLoaders;

	CString       m_strPluginPath;
	int           m_nRefCount;
	HMODULE       m_hModule;                ///< Win32 DLL handle

private:
	/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
	/*������쳣ֵ�Ķ���ο�PTZ.h�ļ���ERR_MSG�Ķ���*/
	ModuleLoader(LPCTSTR lpszPluginPath);
	~ModuleLoader(void);
};