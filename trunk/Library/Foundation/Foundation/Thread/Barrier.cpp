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
#include "Barrier.h"
#include "Thread.h"
namespace ThreadSpace {





Barrier::Barrier(size_t max_n)
{
		if(max_n < 1)
		{
				throw ExceptionSpace::InvalidArgumentException("max_n not equal to zero!");
		}
		m_max_n = max_n;
		m_waiting_n = 0;
}

Barrier::~Barrier()
{
		assert(m_waiting_n == 0);
}

size_t Barrier::MaxNumber()const
{
		return m_max_n;
}


size_t Barrier::WaitingNumber()const
{
		return m_waiting_n;
}


bool Barrier::Acquire(t_ulong timeout)
{
		LockType lock(m_mutex);

		if(m_waiting_n == m_max_n - 1)
		{
				m_cond.BroadCast();
				return true;
		}else
		{
				m_waiting_n++;
				bool res = m_cond.Wait(m_mutex, timeout);
				m_waiting_n--;
				return res;
		}
}

void Barrier::Acquire()
{
		LockType lock(m_mutex);

		if(m_waiting_n == m_max_n - 1)
		{
				m_cond.BroadCast();
		}else
		{
				m_waiting_n++;
				m_cond.Wait(m_mutex);
				m_waiting_n--;
		}
}


void Barrier::Reset()
{
		while(true)
		{
				Thread::Sleep(0);
				LockType lock(m_mutex);
				if(m_waiting_n > 0)
				{
						m_cond.BroadCast();
				}else
				{
						break;
				}
		}
}


















}