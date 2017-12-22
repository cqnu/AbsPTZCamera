// stdafx.cpp : 只包括标准包含文件的源文件
// AbsDevice.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

template<> UINT AFXAPI HashKey(CString& string)
{
    LPCTSTR key = (LPCTSTR) string;
    UINT nHash = 0;
    while (*key)
        nHash = (nHash<<5) + nHash + *key++;
    return nHash;
}

