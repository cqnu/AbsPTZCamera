#pragma once

#include "PTZ.h"
#include "ModuleLoader.h"

class PTZService;
class PtzCamera;

class PTZActivator
{
	friend class PTZService;
	friend class PtzCamera;
public:
	/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
	/*������쳣ֵ�Ķ���ο�PTZ.h�ļ���ERR_MSG�Ķ���*/
	static PTZActivator* GetInstance(LPCTSTR lpszPluginPath);

	void   ReleaseInstance();

private:
	static CMap<CString, CString&,PTZActivator*, PTZActivator*> m_sActivators;

	CString       m_strPluginPath;
	int           m_nRefCount;
	HMODULE       m_hModule;                ///< Win32 DLL handle
	ModuleLoader *m_pModuleLoader;
private:
	INITIAL_PTZ_PROC     m_pfnInitialPTZ;
	EXEC_PTZ_PROC        m_pfnExecPTZ;
	CAN_DIRECT_TO_PROC   m_pfnCanDirectTo;
	DIRECT_TO_PROC       m_pfnDirectTo;
	FINIALIZE_PTZ_PROC   m_pfnFinializePTZ;

private:
	/*�������ʧ�ܽ����׳�int�����쳣����Ҫʹ��try catch����в���*/
	/*������쳣ֵ�Ķ���ο�PTZ.h�ļ���ERR_MSG�Ķ���*/
	PTZActivator(LPCTSTR lpszPluginPath);
	~PTZActivator(void);
};
