
/*
 * The Foundation Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */
#include "mutex.h"

#if defined(OS_FAMILY_WINDOWS)
		#include "Win/MutexImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace ThreadSpace {



Mutex::Mutex() : m_pimpl(new MutexImpl()) { }

Mutex::~Mutex() { delete m_pimpl; }

void Mutex::Lock()
{
		m_pimpl->Lock();
}

//bool Mutex::TryLock(t_ulong timeout)
bool Mutex::TryLock()
{
		return m_pimpl->TryLock();
}

void Mutex::UnLock()
{
		m_pimpl->UnLock();
}



Mutex::ScopeLock::ScopeLock(Mutex  &mutex) : m_mutex(mutex)
{
		m_mutex.Lock();
}

Mutex::ScopeLock::~ScopeLock()
{
		m_mutex.UnLock();
}



}