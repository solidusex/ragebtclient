
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
#include "SpinLock.h"
#include "Event.h"
#include "Thread.h"
#include <list>

namespace ThreadSpace {


class FoundationAPI Token : private NonCopyable
{
private:
		typedef RecursiveSpinLock		MutexType;
		typedef MutexType::ScopeLock	LockType;

		struct WaitInfo
		{
				Event			event;
				t_uint32		wait_tid;

				WaitInfo();
		};
private:
		volatile t_uint32		m_owner_id;
		volatile t_uint32		m_recu_count;
		MutexType				m_mutex;
		std::list<WaitInfo*>	m_wait_queue;
		bool					m_is_fifo;
private:
		bool try_acquire();
public:
		Token(bool is_fifo = true);//如果为ture则为fifo策略，否则为lifo模式

		~Token();
public:

		bool TryAcquire();

		void Acquire();

		void Release();

		bool Acquire(t_uint64 timeout);
};


}