// Copyright (c) 2011 2012 唐锐(also tr0217)
// Mailto:tr0217@163.com
// The earliest release time: 2011-11-08
// Last modification time: 2012-02-26
// Accompanying files of necessity:
//        * ThreadTimer.inl
//
// This file and the accompanying files of this project may be freely used provided the following
// conditions are met:
//        * This copyright statement is not removed or modified.
//        * The code is not sold in uncompiled form.  (Release as a compiled binary which is part
//          of an application is fine)
//        * The design, code, or compiled binaries are not "Re-branded".
//        
// Optional:
//        * Redistributions in binary form must reproduce the above copyright notice.
//        * I receive a fully licensed copy of the product (regardless of whether the product is
//          is free, shrinkwrap, or commercial).  This is optional, though if you release products
//          which use code I've contributed to, I would appreciate a fully licensed copy.
//
// In addition, you may not:
//        * Publicly release modified versions of the code or publicly release works derived from
//          the code without express written authorization.
//
// NO WARRANTY:
//        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
//        ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
//        WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
//        IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
//        INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
//        NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
//        OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
//        WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
//        ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
//        OF SUCH DAMAGE.
//

#ifndef __THREADTIMER_H__
#define __THREADTIMER_H__

#pragma once

#if !defined(__AFX_H__) || !defined(__AFXMT_H__)
#include <windows.h>
#include <process.h>
#endif
#include <assert.h>

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

#ifdef _INC_MMSYSTEM
#define _GetCurTime()       timeGetTime()
#define _StartPeriod(x)     timeBeginPeriod(x)
#define _StopPeriod(x)      timeEndPeriod(x)
#else
#define _GetCurTime()       GetTickCount()
#define _StartPeriod(x)
#define _StopPeriod(x)
#endif


#ifndef TIME_PERIODIC
#define TIME_ONESHOT    0x0000
#define TIME_PERIODIC   0x0001 
#endif

typedef VOID (CALLBACK* THREADTIMERPROC)(LPVOID);

#if defined(__AFX_H__) || defined(__AFXMT_H__)
class CThreadTimer : public CObject
#else
class CThreadTimer
#endif
{
public:
	CThreadTimer(
		const UINT uDelay = 500,//调用回调方法的时间间隔，单位毫秒
		const UINT uResolution = 200,//计时的精度，单位毫秒，此数指定的越小越消耗CPU
		const DWORD dwDueTime = 0,//每次调用Start后延迟多长时间调用回调方法，如果fuEvent不是TIME_PERIODIC此参数无效
		const UINT fuEvent = TIME_PERIODIC);//只调用一次，还是周期性调用

	CThreadTimer(
		const THREADTIMERPROC lpTimerFunc,//回调方法的入口
		const LPVOID lpParam,//回调方法的参数
		const UINT uDelay,
		const UINT uResolution = 200,
		const DWORD dwDueTime = 0,
		const UINT fuEvent = TIME_PERIODIC);		

	BOOL Start();

	BOOL Start(
		const THREADTIMERPROC lpTimerFunc,
		const LPVOID lpParam,
		const UINT uDelay,
		const UINT uResolution = 200,
		const DWORD dwDueTime = 0,
		const UINT fuEvent = TIME_PERIODIC);

	VOID Stop();

	BOOL ReBegin(
		const UINT uDelay = 0,
		const UINT uResolution = 0);

	BOOL IsStarted();

	VOID Change(
		const UINT uDelay,
		const UINT uResolution = 0);//如果uResolution保持默认值则不修改uResolution

	virtual ~CThreadTimer(void);

protected:
	virtual VOID OnTick();

private:
	UINT  m_fuEvent;
	UINT  m_uDelay;
	UINT  m_uResolution;
	DWORD m_dwDueTime;
	BOOL  m_bStarted;
	DWORD m_dwStart;

private:
	THREADTIMERPROC   m_lpTimeProc;
	HANDLE            m_hTimeThread;
	CRITICAL_SECTION  m_csStarted;
	BOOL              m_bCanRebegin;
	BOOL              m_bRun;

#if !defined(__AFX_H__) || !defined(__AFXMT_H__)
	static UINT __stdcall TimeThreadFunc(LPVOID lpParam);
#else
	static UINT TimeThreadFunc(LPVOID lpParam);
#endif

protected:
	LPVOID            m_lpParam;

private:
	CThreadTimer(const CThreadTimer&) {};
	CThreadTimer& operator=(const CThreadTimer&) {};
};

#include "ThreadTimer.inl"

#endif
