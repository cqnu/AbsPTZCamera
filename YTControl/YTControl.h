// YTControl.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CYTControlApp:
// �йش����ʵ�֣������ YTControl.cpp
//

class CYTControlApp : public CWinApp
{
public:
	CYTControlApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	CString  m_strStartUpPath;
};

extern CYTControlApp theApp;