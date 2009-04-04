
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
#pragma once

#include "TypeDef.h"
#include "SpinLock.h"
#include "Condition.h"

namespace ThreadSpace {


class FoundationAPI Barrier : private NonCopyable
{
private:
		typedef RecursiveSpinLock		MutexType;
		typedef MutexType::ScopeLock	LockType;
private:
		mutable MutexType				m_mutex;
		mutable Condition				m_cond;
		
		size_t							m_waiting_n;
		size_t							m_max_n;
public:
		Barrier(size_t	max_n);
		~Barrier();
public:
		bool Acquire(t_ulong timeout);

		void Acquire();
		
		size_t MaxNumber()const;

		size_t WaitingNumber()const;

		void Reset();
};






















}