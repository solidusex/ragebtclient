
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
#include "FastMutexImpl.h"
//#include <windowsx.h>

namespace ThreadSpace {





FastMutexImpl::FastMutexImpl()
{
		::InitializeCriticalSection(&m_cs);
}

FastMutexImpl::~FastMutexImpl()
{
		::DeleteCriticalSection(&m_cs);
}


void FastMutexImpl::Lock()
{
		::EnterCriticalSection(&m_cs);
}

bool FastMutexImpl::TryLock()
{
		return (TryEnterCriticalSection(&m_cs) == TRUE);
}

void FastMutexImpl::UnLock()
{
		::LeaveCriticalSection(&m_cs);
}













}
