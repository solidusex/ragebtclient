
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
#include "MutexImpl.h"
#include "../TypeDef.h"
#include <cassert>

namespace ThreadSpace {

MutexImpl::MutexImpl()
{
		m_mutex_hdl = ::CreateMutex(0, FALSE, 0);
		assert(m_mutex_hdl != NULL);
}

MutexImpl::~MutexImpl() { ::CloseHandle(m_mutex_hdl); }


void MutexImpl::Lock()
{
		DWORD res = ::WaitForSingleObject(m_mutex_hdl, INFINITE);
		if(res == WAIT_ABANDONED || res == WAIT_FAILED)
		{
				assert(0);
				throw ExceptionSpace::SystemException("Lock the mutex failed", ::GetLastError());
		}
}

//bool MutexImpl::TryLock(t_ulong timeout)
bool MutexImpl::TryLock()
{
		switch(::WaitForSingleObject(m_mutex_hdl, 0)){
		case WAIT_OBJECT_0:
				return true;
		case WAIT_TIMEOUT:
				return false;
		default:
				assert(0);
				throw ExceptionSpace::SystemException(
						"try acquire the mutex failed",
						::GetLastError()
						);
		}

		
		
}

void MutexImpl::UnLock()
{
		::ReleaseMutex(m_mutex_hdl);
}


}