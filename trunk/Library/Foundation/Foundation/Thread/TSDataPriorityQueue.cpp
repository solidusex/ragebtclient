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
#include "TSDataPriorityQueue.h"

namespace ThreadSpace {

TSDataPriorityQueue::PriorityInfo::PriorityInfo() : m_priority(LOW), m_ptr(0)
{

}

bool TSDataPriorityQueue::PriorityInfo::operator<(const PriorityInfo &other)const
{
		return (m_priority < other.m_priority);
}




bool TSDataPriorityQueue::PeekQueue(void *&ptr)const
{
		FMLock lock(m_mutex);
		if(!m_queue.empty())
		{
				PriorityInfo info = m_queue.top();
				ptr = info.m_ptr;
				return true;
		}
		return false;
}




void TSDataPriorityQueue::Enqueue(const void *ptr, size_t level)
{
		FMLock lock(m_mutex);

		if(m_wait_queue.empty())
		{
				PriorityInfo info;
				info.m_priority = level;
				info.m_ptr = const_cast<void*>(ptr);
				m_queue.push(info);
		}else
		{
				WaitInfo *pwi = m_wait_queue.front();
				m_wait_queue.pop_front();
				pwi->pt = const_cast<void*>(ptr);
				pwi->m_event.Set();
		}
}

	


bool TSDataPriorityQueue::WaitDequeue(void *&ptr, t_ulong milliseconds)
{
		WaitInfo *pwi = 0;

		{
				FMLock lock(m_mutex);
				
				if(!m_queue.empty())
				{
						PriorityInfo info = m_queue.top();
						ptr = info.m_ptr;
						m_queue.pop();
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
						ptr = pwi->pt;
						res = true;
				}
		}

		delete pwi;
		return res;



}

void TSDataPriorityQueue::WaitDequeue(void *&ptr)
{
		WaitInfo *pwi = 0;

		{
				FMLock lock(m_mutex);
				
				if(!m_queue.empty())
				{
						PriorityInfo info = m_queue.top();
						ptr = info.m_ptr;
						m_queue.pop();
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


bool TSDataPriorityQueue::HasIdleThread()const
{
		FMLock lock(m_mutex);
		return (m_wait_queue.size() != 0);
}

bool TSDataPriorityQueue::IsEmpty()const
{
		FMLock lock(m_mutex);
		return m_queue.empty();

}

size_t TSDataPriorityQueue::Size()const
{
		FMLock lock(m_mutex);
		return m_queue.size();

}

void TSDataPriorityQueue::Clear()
{
		FMLock lock(m_mutex);
		while(!m_queue.empty())
		{
				m_queue.pop();
		}
}

TSDataPriorityQueue::TSDataPriorityQueue() { }

TSDataPriorityQueue::~TSDataPriorityQueue() 
{ 
		//FMLock lock(m_mutex);
		//assert(m_wait_queue.empty());
}







}




#if(0)


/*
void TSDataPriorityQueue::WakeUpAll()
{
		FMLock lock(m_mutex);

		
		for(WaitQueue::iterator it = m_wait_queue.begin(); it != m_wait_queue.end(); ++it)
		{
				(*it)->pt = 0;
				(*it)->m_event.Set();
		}

		m_wait_queue.clear();
		
}*/

/*
void* TSDataPriorityQueue::PeekQueue()const
{
		FMLock lock(m_mutex);
		void *ptr = 0;
		if(!m_queue.empty())
		{
				PriorityInfo info = m_queue.top();
				ptr = info.m_ptr;
				
		}

		return ptr;

}*/


/*
void* TSDataPriorityQueue::WaitDequeue(t_ulong milliseconds)
{
		void* ptr = 0;
		WaitInfo *pwi = 0;

		{
				FMLock lock(m_mutex);
				ptr = dequeue();
				if(ptr != NULL)
				{
						return ptr;
				}

				pwi = new WaitInfo();

				m_wait_queue.push_back(pwi);
		}

		//在这一步,如果超时，那么pwi并没有被清出m_wait_queue,则可能有
		//某个线程调用Enqueue()函数,则pt被赋值.
		
		if(pwi->m_event.Wait(milliseconds))
		{
				//如果没超时，则肯定被enqueue线程或者wakeupall线程从m_wai_queue中清除出去
				//并且pwi->pt 一定被赋值0或者enqueue所指的值
				//则无需在清除m_wait_queue中的pwi
				ptr = pwi->pt;

		}else
		{
				//这里如果上一步超时，并且pt没被某个Enqueue赋值，那么等于0
				//当然，也有相当微小的可能性是它在超时后，并获得锁之前被设置并赋值
				//所以必须先获得锁，然后赋值一次(注意，pwi->pt 初始即为0),然后清除在
				//m_wait_queue中的pwi;
				FMLock lock(m_mutex);
				ptr = pwi->pt;

				//这里可能检索不到，因为有相当微小的可能
				//会被某个线程调用的Enqueue函数pop出去。并赋值给pwi->pt
				//(上一步的锁操作会保证要么pt被赋值，并被弹出wait队列，要么没被
				//赋值并在队列中存在，只有这两种可能
				for(WaitQueue::iterator it = m_wait_queue.begin(); it != m_wait_queue.end(); ++it)
				{
						if(pwi == (*it))
								m_wait_queue.erase(it);
						break;
				}
				
		}

		delete pwi;
		return ptr;
}
*/

#endif

