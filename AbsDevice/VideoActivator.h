#pragma once

#include "Video.h"
#include "ModuleLoader.h"

class VideoService;
class PtzCamera;

class VideoActivator
{
	friend class VideoService;
	friend class PtzCamera;
public:
	/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
	/*捕获的异常值的定义参考Video.h文件中ERR_MSG的定义*/
	static VideoActivator* GetInstance(LPCTSTR lpszPluginPath);

	void   ReleaseInstance();

private:
	static CMap<CString, CString&,VideoActivator*, VideoActivator*> m_sActivators;

	CString         m_strPluginPath;
	int             m_nRefCount;
	HMODULE         m_hModule;                ///< Win32 DLL handle
    ModuleLoader   *m_pModuleLoader;
private:
	INITIAL_VIDEO_SOURCE_PROC    m_pfnInitialVideoSource;
	GET_VIDEO_CODEC_PROC         m_pfnGetVideoCodec;
	CAN_RECONNECT_PROC           m_pfnCanReConnect;
	TRIGGER_PERIPHERAL_PROC      m_pfnTriggerPeripheral;
	START_VIDEO_SOURCE_PROC      m_pfnStartVideoSource;
	STOP_VIDEO_SOURCE_PROC       m_pfnStopVideoSource;
	FINIALIZE_VIDEO_SOURCE_PROC  m_pfnFinializeVideoSource;

private:
	/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
	/*捕获的异常值的定义参考Video.h文件中ERR_MSG的定义*/
	VideoActivator(LPCTSTR lpszPluginPath);
	~VideoActivator(void);
};
