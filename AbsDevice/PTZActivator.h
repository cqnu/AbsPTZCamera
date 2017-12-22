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
	/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
	/*捕获的异常值的定义参考PTZ.h文件中ERR_MSG的定义*/
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
	/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
	/*捕获的异常值的定义参考PTZ.h文件中ERR_MSG的定义*/
	PTZActivator(LPCTSTR lpszPluginPath);
	~PTZActivator(void);
};
