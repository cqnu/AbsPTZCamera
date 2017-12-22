#pragma once

class ConfigManager
{
public:
	static ConfigManager& GetInstance();

public:
	CString PTZPlugin;
	CString PTZPlugin2;
	CString VideoPlugin;

private:
	CString m_strModulePath;
private:
	static ConfigManager m_instance;

private:
	ConfigManager(void);
	~ConfigManager(void);
};
