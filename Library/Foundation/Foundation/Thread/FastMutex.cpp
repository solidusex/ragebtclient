
/*
 * The Foundation Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  It is provided "as is" without express
 * or implied warranty.
 */
#include "FastMutex.h"

#if defined(OS_FAMILY_WINDOWS)
		#include "Win/FastMutexImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif



namespace ThreadSpace {

FastMutex::FastMutex() : m_pimpl(new FastMutexImpl()) { }

FastMutex::~FastMutex() { delete m_pimpl; }



FastMutex::ScopeLock::ScopeLock(FastMutex &mutex) : m_mutex(mutex)
{
		m_mutex.Lock();
}

FastMutex::ScopeLock::~ScopeLock()
{
		m_mutex.UnLock();
}

void FastMutex::Lock()
{
		m_pimpl->Lock();
}

bool FastMutex::TryLock()
{
		return m_pimpl->TryLock();
}

void FastMutex::UnLock()
{
		m_pimpl->UnLock();
}


#if(0)
void FastMutex::Acquire()
{
		m_pimpl->Acquire();
}

bool FastMutex::TryAcquire(t_ulong timeout)
{
		return m_pimpl->TryAcquire(timeout);
}

void FastMutex::Release()
{
		m_pimpl->Release();
}
#endif

}