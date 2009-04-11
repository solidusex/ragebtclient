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
#include "TSDataQueue.h"


namespace ThreadSpace {




bool TSDataQueue::PeekQueue(void *&ptr)const
{
		FMLock lock(m_mutex);
		if(!m_queue.empty())
		{
				ptr = m_queue.front();
				return true;
		}else
		{
				return false;
		}

}

void TSDataQueue::EnqueueToFront(const void *ptr)
{
		FMLock lock(m_mutex);
		
		if(m_wait_queue.empty())
		{
				m_queue.push_front(const_cast<void*>(ptr));
		}else
		{
				WaitInfo *pwi = m_wait_queue.front();
				m_wait_queue.pop_front();
				pwi->pt = const_cast<void*>(ptr);
				pwi->m_event.Set();
		}
}

void TSDataQueue::Enqueue(const void *ptr)
{
		FMLock lock(m_mutex);

		if(m_wait_queue.empty())
		{
				m_queue.push_back(const_cast<void*>(ptr));
		}else
		{
				WaitInfo *pwi = m_wait_queue.front();
				m_wait_queue.pop_front();
				pwi->pt = const_cast<void*>(ptr);
				pwi->m_event.Set();
		}
}




		//void WaitDequeue(void *&ptr);

bool TSDataQueue::WaitDequeue(void *&ptr, t_ulong milliseconds)
{
		WaitInfo *pwi = 0;

		{
				FMLock lock(m_mutex);
				
				if(!m_queue.empty())
				{
						ptr = m_queue.front();
						m_queue.pop_front();
						return true;
				}else
				{
						pwi = new WaitInfo();
						m_wait_queue.push_back(pwi);
				}
		}
		
		bool res = false;
		if(pwi->m_event.Wait(milliseconds))
		{
				ptr = pwi->pt;
				res = true;
		}else
		{
				FMLock lock(m_mutex);
				
				WaitQueue::iterator it = std::find(m_wait_queue.begin(), m_wait_queue.end(), pwi);
				if(it != m_wait_queue.end())
				{
						ptr = 0;
						m_wait_queue.erase(it);
						res = false;
				}else
				{
//如果我们获得锁之后，又发现m_wait_queue没有我们注册进去的pwi，那么说明此pwi中的pt已经被赋值了
						ptr = pwi->pt;
						res = true;
				}
		}

		delete pwi;
		return res;

}

void TSDataQueue::WaitDequeue(void *&ptr)
{
		WaitInfo *pwi = 0;

		{
				FMLock lock(m_mutex);
				
				if(!m_queue.empty())
				{
						ptr = m_queue.front();
						m_queue.pop_front();
						return;
				}else
				{
						pwi = new WaitInfo();
						m_wait_queue.push_back(pwi);
				}
		}
		
		
		pwi->m_event.Wait();
		ptr = pwi->pt;
		delete pwi;
}


bool TSDataQueue::HasIdleThread()const
{
		FMLock lock(m_mutex);
		return (m_wait_queue.size() != 0);
}

bool TSDataQueue::IsEmpty()const
{
		FMLock lock(m_mutex);
		return m_queue.empty();

}

size_t TSDataQueue::Size()const
{
		FMLock lock(m_mutex);
		return m_queue.size();
}



void TSDataQueue::Clear()
{
		FMLock lock(m_mutex);
		m_queue.clear();
}

TSDataQueue::TSDataQueue() { }

TSDataQueue::~TSDataQueue() 
{ 
		//FMLock lock(m_mutex);
		//assert(m_wait_queue.empty());

}


}





#if(0)
void TSDataQueue::WakeUpAll()
{
		FMLock lock(m_mutex);

		for(WaitQueue::iterator it = m_wait_queue.begin(); it != m_wait_queue.end(); ++it)
		{
				(*it)->pt = 0;
				(*it)->m_event.Set();
		}

		m_wait_queue.clear();
}

#endif