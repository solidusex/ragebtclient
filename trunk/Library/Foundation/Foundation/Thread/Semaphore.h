
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

#include "../Common/ComnHdr.h"
#include "../Utility/UtilityHdr.h"
#include <limits>


namespace ThreadSpace {




class SemaphoreImpl;

class  FoundationAPI  Semaphore : private NonCopyable
{
private:
		SemaphoreImpl  	*m_pimpl;
public:
		Semaphore(t_ulong count = 0, t_ulong max = MAX_LONG);
		~Semaphore();
public:
		t_ulong Count() const; 
		bool TryAcquire();
		bool Acquire(t_ulong timeout);
		void Acquire();
		void Release(t_ulong n = 1);
};



}