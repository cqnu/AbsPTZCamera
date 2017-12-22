#pragma once

class ModuleLoader
{
public:
	/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
	/*捕获的异常值的定义参考PTZ.h文件中ERR_MSG的定义*/
	static ModuleLoader* GetInstance(LPCTSTR lpszPluginPath);
    HMODULE GetModule();
	void   ReleaseInstance();

private:
	static CMap<CString, CString&,ModuleLoader*, ModuleLoader*> m_sLoaders;

	CString       m_strPluginPath;
	int           m_nRefCount;
	HMODULE       m_hModule;                ///< Win32 DLL handle

private:
	/*如果创建失败将会抛出int类型异常，需要使用try catch块进行捕获。*/
	/*捕获的异常值的定义参考PTZ.h文件中ERR_MSG的定义*/
	ModuleLoader(LPCTSTR lpszPluginPath);
	~ModuleLoader(void);
};