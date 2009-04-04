
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
#include "Semaphore.h"




#if defined(OS_FAMILY_WINDOWS)
		#include "Win/SemaphoreImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace ThreadSpace {

Semaphore::Semaphore(t_ulong count, t_ulong max)
{
		m_pimpl = new SemaphoreImpl(count, max);
}


Semaphore::~Semaphore() { delete m_pimpl; }

t_ulong Semaphore::Count() const { return m_pimpl->Count(); }

bool Semaphore::TryAcquire()
{
		return m_pimpl->TryAcquire();
}

bool Semaphore::Acquire(t_ulong timeout)
{
		return m_pimpl->Acquire(timeout);
}

void Semaphore::Acquire() { m_pimpl->Acquire(); }

void Semaphore::Release(t_ulong n) { m_pimpl->Release(n); }

}