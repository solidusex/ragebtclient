
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
#include "Token.h"
#include <algorithm>

namespace ThreadSpace {

Token::WaitInfo::WaitInfo() : event(false), wait_tid(Thread::GetCurrentThreadId())
{

}

Token::Token(bool is_fifo ) : m_owner_id(0), m_recu_count(0), m_is_fifo(is_fifo)
{


}

Token::~Token()
{
		assert(m_owner_id == 0);
		assert(m_recu_count == 0);
}


bool Token::try_acquire()
{
		t_uint32 curr_tid = Thread::GetCurrentThreadId();

		if(m_owner_id == curr_tid)
		{
				assert(m_recu_count >= 1);
				m_recu_count++;
				return true;
		}

		if(m_owner_id == 0)
		{
				assert(m_recu_count == 0);
				m_owner_id = curr_tid;
				m_recu_count = 1;
				return true;
		}

		return false;
}



bool Token::TryAcquire()
{
		LockType lock(m_mutex);
		return try_acquire();
}

void Token::Acquire()
{
		m_mutex.Lock();

		if(try_acquire())
		{
				m_mutex.UnLock();
		}else
		{
				WaitInfo wait_info;
				assert(wait_info.wait_tid != 0);
				
				m_wait_queue.push_back(&wait_info);
				m_mutex.UnLock();

				wait_info.event.Wait();
				
				assert(m_owner_id == wait_info.wait_tid);
				assert(m_recu_count == 1);
		}
}



void Token::Release()
{
		t_uint32 curr_tid = Thread::GetCurrentThreadId();
		LockType lock(m_mutex);
		assert(m_owner_id == curr_tid);
		assert(m_recu_count >= 1);

		if(m_recu_count == 1)
		{
				m_recu_count = 0;
				m_owner_id = 0;

				if(!m_wait_queue.empty())
				{
						WaitInfo *pwi = 0;
						if(m_is_fifo)
						{
								pwi = m_wait_queue.front();
								m_wait_queue.pop_front();
						}else
						{
								pwi = m_wait_queue.back();
								m_wait_queue.pop_back();
						}

						assert(pwi != 0);
						assert(pwi->wait_tid != 0);
						m_owner_id = pwi->wait_tid;
						m_recu_count = 1;
						pwi->event.Set();
				}
		}else
		{
				m_recu_count--;
		}
}


bool Token::Acquire(t_uint64 timeout)
{
		m_mutex.Lock();

		if(try_acquire())
		{
				m_mutex.UnLock();
				return true;
		}else
		{
				WaitInfo wait_info;
				assert(wait_info.wait_tid != 0);
				
				m_wait_queue.push_back(&wait_info);
				m_mutex.UnLock();
				
				bool res = wait_info.event.Wait(timeout);


				LockType lock(m_mutex);

				if(!res)
				{
						std::list<WaitInfo*>::iterator it = 
								std::find(m_wait_queue.begin(), m_wait_queue.end(), &wait_info);

						if(it != m_wait_queue.end())
						{
								m_wait_queue.erase(it);
								return false;
						}
				}

				assert(m_owner_id == wait_info.wait_tid);
				assert(m_recu_count == 1);
				return true;
		}
}











}