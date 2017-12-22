// Copyright (c) 2011 2012 唐锐(also tr0217)
// Mailto:tr0217@163.com
// The earliest release time: 2011-11-08
// Last modification time: 2012-02-26
// Accompanying files of necessity:
//        * ThreadTimer.h
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

inline CThreadTimer::CThreadTimer(const UINT uDelay /*= 500*/,
						   const UINT uResolution /*= 200*/, 
						   const DWORD dwDueTime /*= 0*/,
						   const UINT fuEvent /*= TIME_PERIODIC*/)
{
	m_bCanRebegin = TRUE;
	m_bRun = TRUE;
	m_bStarted = FALSE;
	m_uDelay = uDelay;
	m_uResolution = uResolution;
	m_dwDueTime = dwDueTime;
	m_fuEvent = fuEvent;

	assert(uDelay >= uResolution);

	InitializeCriticalSection(&m_csStarted);
	m_hTimeThread = NULL;
}

inline CThreadTimer::CThreadTimer(const THREADTIMERPROC lpTimerFunc,
						   const LPVOID lpParam,
						   const UINT uDelay ,
						   const UINT uResolution /*= 200*/,
						   const DWORD dwDueTime /*= 0*/,
						   const UINT fuEvent /*= TIME_PERIODIC*/)

{
	m_bCanRebegin = TRUE;
	m_bRun = TRUE;
	m_bStarted = FALSE;
	m_uResolution = uResolution;
	m_dwDueTime = dwDueTime;
	m_fuEvent = fuEvent;

	m_hTimeThread = NULL;
	m_lpTimeProc = lpTimerFunc;
	m_lpParam = lpParam;
	m_uDelay = uDelay;

	assert(m_uDelay > m_uResolution);
	InitializeCriticalSection(&m_csStarted);
}

inline CThreadTimer::~CThreadTimer(void)
{
	if (m_hTimeThread != NULL)
	{
		m_bRun = FALSE;
		if (!this->IsStarted())
		{
			ResumeThread(m_hTimeThread);
		}
		WaitForSingleObject(m_hTimeThread,100);
		
		CloseHandle(m_hTimeThread);
	}
	DeleteCriticalSection(&m_csStarted);
}

inline BOOL CThreadTimer::Start()
{
	if (m_hTimeThread == NULL)
	{
#if !defined(__AFX_H__) || !defined(__AFXMT_H__)
		m_hTimeThread = (HANDLE)_beginthreadex(NULL,0,CThreadTimer::TimeThreadFunc,
			(LPVOID)this,CREATE_SUSPENDED,NULL);
		if(m_hTimeThread == 0)
		{
			return FALSE;
		}
#else
		CWinThread* pThread = AfxBeginThread(CThreadTimer::TimeThreadFunc,
			(LPVOID)this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		if (pThread == NULL)
		{
			AfxThrowResourceException();
		}
		::DuplicateHandle(GetCurrentProcess (), pThread->m_hThread, 
			GetCurrentProcess (), &m_hTimeThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
#endif
	}

	BOOL ret = FALSE;

	EnterCriticalSection(&m_csStarted);

	if (!m_bStarted)
	{
		if(m_fuEvent == TIME_PERIODIC)
			m_dwStart = _GetCurTime() + m_dwDueTime - m_uDelay - 10;
		else
			m_dwStart = _GetCurTime();
		m_bStarted = TRUE;
		m_bCanRebegin = TRUE;
		ResumeThread(m_hTimeThread);
		ret = TRUE;
	}

	LeaveCriticalSection(&m_csStarted);

	return ret;
}

inline BOOL CThreadTimer::Start(const THREADTIMERPROC lpTimerFunc,
						 const LPVOID lpParam,
						 const UINT uDelay,
						 const UINT uResolution /*= 200*/,
						 const DWORD dwDueTime /*= 0*/,
						 const UINT fuEvent /*= TIME_PERIODIC*/)
{
	BOOL ret = FALSE;
	EnterCriticalSection(&m_csStarted);

	if (!m_bStarted)
	{
		m_lpTimeProc = lpTimerFunc;
		m_lpParam = lpParam;
		m_uDelay = uDelay;
		m_uResolution = uResolution;
		m_dwDueTime = dwDueTime;
		m_fuEvent = fuEvent;

		if (m_hTimeThread == NULL)
		{
#if !defined(__AFX_H__) || !defined(__AFXMT_H__)
			m_hTimeThread = (HANDLE)_beginthreadex(NULL,0,CThreadTimer::TimeThreadFunc,
				(LPVOID)this,CREATE_SUSPENDED,NULL);
			if(m_hTimeThread == 0)
			{
				return FALSE;
			}
#else
			CWinThread* pThread = AfxBeginThread(CThreadTimer::TimeThreadFunc,
				(LPVOID)this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);

			if (pThread == NULL)
			{
				AfxThrowResourceException();
			}

			::DuplicateHandle(GetCurrentProcess (), pThread->m_hThread, 
				GetCurrentProcess (), &m_hTimeThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
#endif
		}

		if(m_fuEvent == TIME_PERIODIC)
			m_dwStart = _GetCurTime() + m_dwDueTime - m_uDelay - 10;
		else
			m_dwStart = _GetCurTime();
		m_bStarted = TRUE;
		m_bCanRebegin = TRUE;
		ResumeThread(m_hTimeThread);
		ret = TRUE;
	}

	LeaveCriticalSection(&m_csStarted);

	return ret;
}

inline VOID CThreadTimer::Stop()
{
	EnterCriticalSection(&m_csStarted);

	m_bCanRebegin = FALSE;

	if (m_bStarted)
	{
		SuspendThread(m_hTimeThread);
		m_dwStart = 0;
		m_bStarted = FALSE;
	}

	LeaveCriticalSection(&m_csStarted);
}

inline BOOL CThreadTimer::ReBegin( const UINT uDelay /*= 0*/, const UINT uResolution /*= 0*/ )
{
	BOOL ret = FALSE;
	EnterCriticalSection(&m_csStarted);

	if(m_bCanRebegin)
	{
		m_dwStart = _GetCurTime();
		if (uDelay != 0)
		{
			m_uDelay = uDelay;
		}
		if (uResolution != 0)
		{
			m_uResolution = uResolution;
		}
		assert(m_uDelay > m_uResolution);
		ret = TRUE;
	}

	LeaveCriticalSection(&m_csStarted);

	return ret;
}

inline BOOL CThreadTimer::IsStarted()
{
	BOOL ret;

	EnterCriticalSection(&m_csStarted);
	ret = m_bStarted;
	LeaveCriticalSection(&m_csStarted);

	return ret;
}

inline VOID CThreadTimer::Change( const UINT uDelay, const UINT uResolution /*= 0*/ )
{
	EnterCriticalSection(&m_csStarted);
	m_uDelay = uDelay;
	if(uResolution != 0)
		m_uResolution = uResolution;
	LeaveCriticalSection(&m_csStarted);
}

inline UINT CThreadTimer::TimeThreadFunc( LPVOID lpParam )
{
	CThreadTimer* pTimer = (CThreadTimer*)lpParam;

	while(pTimer->m_bRun)
	{
		EnterCriticalSection(&(pTimer->m_csStarted));
		_StartPeriod(1); 
		Sleep(pTimer->m_uResolution);
		_StopPeriod(1);
		DWORD now = _GetCurTime();
		if ((now - pTimer->m_dwStart) >= pTimer->m_uDelay)
		{
			pTimer->m_dwStart += pTimer->m_uDelay;

			if (pTimer->m_bStarted)
			{
				pTimer->OnTick();

				if (pTimer->m_fuEvent == TIME_ONESHOT)
				{
					pTimer->m_bStarted = FALSE;
					pTimer->m_bCanRebegin = FALSE;
					pTimer->m_dwStart = 0;
					LeaveCriticalSection(&(pTimer->m_csStarted));
					SuspendThread(pTimer->m_hTimeThread);
					EnterCriticalSection(&(pTimer->m_csStarted));
				}	
			}	
		}
		LeaveCriticalSection(&(pTimer->m_csStarted));
	}

	return 0;
}

inline VOID CThreadTimer::OnTick()
{
	if (m_lpTimeProc != NULL)
	{
		try
		{
			m_lpTimeProc(m_lpParam);
		}
#if defined(__AFX_H__) || defined(__AFXMT_H__)
		catch (CException* e)
		{
#ifdef DEBUG
			CString str;
			e->GetErrorMessage(str.GetBuffer(100),100,NULL);
			str.ReleaseBuffer();
			MessageBox(NULL,_T("捕获到一个异常！\n")+str+_T("......"),_T("警告"),MB_RETRYCANCEL|MB_ICONERROR);
#endif
			e->Delete();
		}
#endif 
		catch (...)
		{
#ifdef DEBUG
			assert(false);
#endif
		}

	}
}