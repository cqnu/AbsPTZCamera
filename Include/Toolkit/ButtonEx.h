#pragma once
#include "afxwin.h"

#define BN_LBUTTONDOWN  (WM_USER+10)  //按钮按下 
#define BN_LBUTTONUP    (WM_USER+11)  //弹起 

#define ON_BN_LBUTTONDOWN(id, memberFxn)  ON_CONTROL(BN_LBUTTONDOWN,id, memberFxn)  //消息处理宏 
#define ON_BN_LBUTTONUP(id, memberFxn)    ON_CONTROL(BN_LBUTTONUP,id, memberFxn)      //消息处理宏 

class CButtonEx :
	public CButton
{
	DECLARE_DYNAMIC(CButtonEx)
public:
	CButtonEx(void);
public:
	~CButtonEx(void);
protected:
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
