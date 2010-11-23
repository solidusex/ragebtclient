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
#include "SemaphoreImpl.h"
#include <cassert>
#include "../TypeDef.h"

namespace ThreadSpace {


SemaphoreImpl::SemaphoreImpl(t_ulong count, t_ulong max)
{
		assert(max > 0);
		assert(count <= max);
		m_sema_hdl = ::CreateSemaphore(0, count, max, 0);

		assert(max);
		assert(m_sema_hdl != NULL);

		m_curr_resources = count;
		m_max_resources = max;
}

SemaphoreImpl::~SemaphoreImpl()
{
		::CloseHandle(m_sema_hdl);
}

t_ulong SemaphoreImpl::Count() const
{
		return m_curr_resources;
}

void SemaphoreImpl::Acquire()
{
		//::WaitForSingleObject(m_sema_hdl, INFINITE);
		//::InterlockedDecrement((long*)&m_curr_resources);
		bool res = Acquire(INFINITE);
		assert(res);
}
void SemaphoreImpl::Release(t_ulong n)
{
		::ReleaseSemaphore(m_sema_hdl, n, 0);
		::InterlockedExchangeAdd((long*)&m_curr_resources, n);
}

bool SemaphoreImpl::TryAcquire()
{
		/*switch(::WaitForSingleObject(m_sema_hdl, 0))
		{
		case WAIT_OBJECT_0:
				::InterlockedDecrement((long*)&m_curr_resources);
				return true;
		case WAIT_TIMEOUT:
				return false;
		default:
				assert(0);
				throw ExceptionSpace::SystemException("try acquire the Semaphore failed", ::GetLastError());
		}*/

		return Acquire(0);
}

bool SemaphoreImpl::Acquire(t_ulong timeout)
{
		switch(::WaitForSingleObject(m_sema_hdl, timeout))
		{
		case WAIT_OBJECT_0:
				::InterlockedDecrement((long*)&m_curr_resources);
				return true;
		case WAIT_TIMEOUT:
				return false;
		default:
				assert(0);
				throw ExceptionSpace::SystemException("try acquire the Semaphore failed", ::GetLastError());
				//return false;
		}
}


}