// AbsDevice.h : AbsDevice DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAbsDeviceApp
// �йش���ʵ�ֵ���Ϣ������� AbsDevice.cpp
//

class CAbsDeviceApp : public CWinApp
{
public:
	CAbsDeviceApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
