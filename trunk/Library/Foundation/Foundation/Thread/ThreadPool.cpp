
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
#include "ThreadPool.h"


namespace ThreadSpace {

#define  TP_SHUT_DOWN 0

void ThreadPool::worker_thread()
{
		assert(IsStarted());

		while(true)
		{
				void *pcb = 0;
				m_queue.WaitDequeue(pcb);

				if((t_uint64)pcb == TP_SHUT_DOWN)
				{
						break;
				}else
				{
						((ThreadCallBack*)pcb)->Run();
				}
		}

}

bool ThreadPool::Start(size_t thd_num)
{
		if(IsStarted()) {assert(false); return false; }
		m_is_started = true;
		IncreaseThread(thd_num);
		return true;
}

void ThreadPool::Stop()
{
		if(!IsStarted()) {assert(false);}

		DecreaseThread(m_thd_num);

		for(size_t i = 0; i < m_thread_set.size(); ++i)
		{
				m_thread_set[i]->Join();
		}

		for(size_t i = 0; i < m_thread_set.size(); ++i)
		{
				delete m_thread_set[i];
		}
		m_thread_set.clear();

		m_is_started = false;

}

bool ThreadPool::IsStarted()const
{
		return m_is_started;

}

void ThreadPool::execute(ThreadCallBackAbs *ptcb)
{
		if(IsStarted())
		{
				m_queue.Enqueue(ptcb);
		}else
		{
				assert(false);
		}
}

void ThreadPool::IncreaseThread(size_t n)
{
		assert(this->IsStarted());

		if(IsStarted())
		{
				for(size_t i = 0; i < n; ++i)
				{
						m_thread_set.push_back(new Thread(this, &ThreadPool::worker_thread));
				}

				m_thd_num += n;
		}

}

void ThreadPool::DecreaseThread(size_t n)
{
		assert(this->IsStarted());
		if(IsStarted())
		{
				size_t dec_n = std::min(n, m_thd_num);

				for(size_t i = 0; i < dec_n; ++i)
				{
						m_queue.Enqueue(TP_SHUT_DOWN);
				}

				m_thd_num -= dec_n;
		}

}

void ThreadPool::Execute(ThreadFunction func, void *param)
{
		execute(new ThreadCallBack(func, param));
}

ThreadPool::ThreadPool() : m_thd_num(0), m_is_started(false)
{
		m_queue.Clear();
}


ThreadPool::~ThreadPool()
{
		try{
		if(IsStarted())
		{
				Stop();
		}
		}catch(...)
		{
				DEBUG_PRINT0("UnKnow Exception from ThreadPool::~ThreadPool()\n");
		}
}


size_t ThreadPool::ThreadCount()const
{
		return m_thd_num;
}











}