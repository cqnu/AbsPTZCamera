#include "StdAfx.h"
#include "ButtonEx.h"

IMPLEMENT_DYNAMIC(CButtonEx, CButton)

CButtonEx::CButtonEx(void)
{
}

CButtonEx::~CButtonEx(void)
{
}

BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CButtonEx::OnLButtonDown(UINT nFlags, CPoint point) 
{ 
	HWND hwParent = GetParent()->m_hWnd;  
	UINT uID = GetDlgCtrlID(); 
	::SendMessage(hwParent,WM_COMMAND,MAKEWPARAM(uID,BN_LBUTTONDOWN), (LPARAM)m_hWnd); //发送给父窗口 

	CButton::OnLButtonDown(nFlags, point); 
}

void CButtonEx::OnLButtonUp(UINT nFlags, CPoint point) 
{ 
	HWND hwParent = GetParent()->m_hWnd;  
	UINT uID = GetDlgCtrlID(); 
	::SendMessage(hwParent,WM_COMMAND,MAKEWPARAM(uID,BN_LBUTTONUP), (LPARAM)m_hWnd); //发送给父窗口 

	CButton::OnLButtonDown(nFlags, point); 
}