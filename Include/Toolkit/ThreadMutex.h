#ifndef __THREADMUTEX_H__
#define __THREADMUTEX_H__

#pragma once

#include "process.h"

class ThreadMutex
{
public:
	ThreadMutex()
	{
		InitializeCriticalSection(&m_cs);
	}

	~ThreadMutex()
	{
		DeleteCriticalSection(&m_cs);
	}

	void Lock()
	{
		EnterCriticalSection(&m_cs);		
	}

	void Unlock()
	{
		LeaveCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION m_cs;
};


#endif