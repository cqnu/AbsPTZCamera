// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// AbsDevice.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

template<> UINT AFXAPI HashKey(CString& string)
{
    LPCTSTR key = (LPCTSTR) string;
    UINT nHash = 0;
    while (*key)
        nHash = (nHash<<5) + nHash + *key++;
    return nHash;
}

