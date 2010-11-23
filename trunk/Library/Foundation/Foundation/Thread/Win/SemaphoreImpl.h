
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
#pragma once

#include "../../Common/ComnHdr.h"
#include <windows.h>

//Win32 Semaphore 
namespace ThreadSpace {


class SemaphoreImpl
{
private:
		HANDLE			m_sema_hdl;
		volatile t_ulong		m_curr_resources;
		volatile t_ulong		m_max_resources;
public:
		SemaphoreImpl(t_ulong count = 1, t_ulong max = 1);
		~SemaphoreImpl();
public:
		t_ulong Count()const;
public:
		bool Acquire(t_ulong timeout);
		void Acquire();
		void Release(t_ulong n);
		bool TryAcquire();
		
};







}






