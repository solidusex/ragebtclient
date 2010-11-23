
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
#include "Timer.h"
#include <algorithm>

namespace ThreadSpace {


void Timer::run()
{
		m_cb->Run();
		m_ts.Update();
}
		
bool Timer::is_one_off()const
{
		return m_is_one_off;
}

bool Timer::is_expired()const
{
		return m_ts.ElapsedMillisecond() >= m_interval;
}

void Timer::update()const
{
		m_ts.Update();
}

Timer::Timer(ThreadFunction proc, void *context, t_uint64 interval, bool is_one_off) 
		: m_cb(new ThreadCallBack(proc, context))
		, m_interval(interval) 
		, m_is_one_off(is_one_off)
{
		
		assert(m_cb != 0);
}

Timer::~Timer()
{
		delete m_cb;
}


void Timer::ResetInterval(t_uint64 interval)
{
		assert(interval != 0);
		m_interval = interval;
}


///////////////////////////////////////////////////////////////////////////////////




size_t TimerManager::Count()const
{
		LockType lock(m_mutex);
		return m_set.size();
}

TimerManager::TimerManager() : m_is_started(false), m_thread(0), m_resolution(20)
{


}

TimerManager::~TimerManager()
{
		assert(!m_is_started);
		assert(m_thread == 0);
		Clear();
}


bool TimerManager::Start(t_uint32 min_resolution)
{
		if(!m_is_started)
		{
				assert(m_thread == 0);
				m_resolution = min_resolution;
				m_is_started = true;
				m_thread = new Thread(this, &TimerManager::worker_thread);
				return true;
		}else
		{
				return false;
		}


}

void TimerManager::Stop()
{
		if(m_is_started)
		{
				assert(m_thread != 0);
				m_is_started = false;
				m_thread->Join();
				delete m_thread;
				m_thread = 0;
				m_resolution = 20;
		}
}


void TimerManager::Clear()
{
		LockType lock(m_mutex);
		
		for(TimerSet::iterator it = m_set.begin(); it != m_set.end(); ++it)
		{
				(*it)->Release();
		}
		m_set.clear();
}

void TimerManager::worker_thread()
{
		while(m_is_started)
		{
				Thread::Sleep(m_resolution);

				LockType lock(m_mutex);
				TimerSet::iterator beg = m_set.begin();
				TimerSet::iterator end = m_set.end();
				
				TimerSet::iterator it = beg;

				while(it != end)
				{
						if((*it)->is_expired())
						{
								(*it)->run();

								if((*it)->is_one_off())
								{
										(*it)->Release();
										it = m_set.erase(it);
										continue;
								}
						}
						++it;
				}
		}
}


bool TimerManager::RegisterTimer(Timer *ptimer)
{
		assert(ptimer != 0);

		LockType lock(m_mutex);
		
		TimerSet::iterator it = std::find(m_set.begin(), m_set.end(), ptimer);

		if(it == m_set.end())
		{
				ptimer->Duplicate();
				ptimer->update();
				m_set.push_back(ptimer);
				return true;
		}else
		{
				return false;
		}
}
		
bool TimerManager::RemoveTimer(Timer *ptimer)
{
		assert(ptimer != 0);
		LockType lock(m_mutex);

		TimerSet::iterator it = std::find(m_set.begin(), m_set.end(), ptimer);
		if(it != m_set.end())
		{
				m_set.erase(it);
				ptimer->Release();
				return true;
		}

		return false;
}







/*
#if(0)
size_t TimerManager::Count()const
{
		LockType lock(m_mutex);
		return m_set.size();
}

TimerManager::TimerManager() : m_is_started(false), m_thread(0), m_resolution(20)
{


}

TimerManager::~TimerManager()
{
		assert(!m_is_started);
		assert(m_thread == 0);
		Clear();
}


bool TimerManager::Start(t_uint32 min_resolution)
{
		if(!m_is_started)
		{
				assert(m_thread == 0);
				m_resolution = min_resolution;
				m_is_started = true;
				m_thread = new Thread(this, &TimerManager::worker_thread);
				return true;
		}else
		{
				return false;
		}


}
		
void TimerManager::Stop()
{
		if(m_is_started)
		{
				assert(m_thread != 0);
				m_is_started = false;
				m_thread->Join();
				delete m_thread;
				m_thread = 0;
				m_resolution = 20;
		}
}


void TimerManager::Clear()
{
		LockType lock(m_mutex);
		
		for(TimerSet::iterator it = m_set.begin(); it != m_set.end(); ++it)
		{
				(*it)->reset_remain_time();
				(*it)->Release();
		}
		m_set.clear();
}

void TimerManager::worker_thread()
{
		std::vector<Timer*> exec_timer;

		while(m_is_started)
		{
				Thread::Sleep(m_resolution);

				LockType lock(m_mutex);
				TimerSet::iterator beg = m_set.begin();
				TimerSet::iterator end = m_set.end();
				exec_timer.clear();
				
				t_uint64 remain_time = 0;

				for(TimerSet::iterator it = beg; it != end; )
				{
						remain_time = (*it)->get_remain_time();
						if(remain_time <= m_resolution)
						{
								exec_timer.push_back(*it);
								it = m_set.erase(it);
						}else
						{
								break;
						}
				}

				beg = m_set.begin();
				end = m_set.end();

				for(TimerSet::iterator it = beg; it != end; ++it)
				{
						remain_time = (*it)->get_remain_time();
						assert(remain_time > m_resolution);
						(*it)->set_remain_time(remain_time - m_resolution);
				}
				
				for(size_t i = 0; i < exec_timer.size(); ++i)
				{
						exec_timer[i]->run();

						if(exec_timer[i]->is_one_off())
						{
								exec_timer[i]->Release();
						}else
						{
								exec_timer[i]->reset_remain_time();
								assert(exec_timer[i]->get_remain_time() == exec_timer[i]->get_interval());
								insert_timer(exec_timer[i]);
						}
				}
		}

}


bool TimerManager::RegisterTimer(Timer *ptimer)
{
		assert(ptimer != 0);

		LockType lock(m_mutex);
		
		for(TimerSet::iterator it = m_set.begin(); it != m_set.end(); ++it)
		{
				if( ptimer == (*it))
				{
						return false;
				}else
				{
						if((*it)->get_remain_time() >= ptimer->get_remain_time())
						{
								ptimer->Duplicate();
								ptimer->reset_remain_time();
								m_set.insert(it, ptimer);
								return true;
						}
				}
		}
		ptimer->Duplicate();
		m_set.push_back(ptimer);
		return true;
}
		
bool TimerManager::RemoveTimer(Timer *ptimer)
{
		assert(ptimer != 0);
		LockType lock(m_mutex);
		for(TimerSet::iterator it = m_set.begin(); it != m_set.end(); ++it)
		{
				if((*it) == ptimer)
				{
						m_set.erase(it);
						ptimer->reset_remain_time();
						ptimer->Release();
						return true;
				}
		}

		return false;
}

void TimerManager::insert_timer(Timer *p_timer)
{
		assert(p_timer != 0 && p_timer->get_interval() == p_timer->get_remain_time());
		TimerSet::iterator beg = m_set.begin();
		TimerSet::iterator end = m_set.end();

		for(TimerSet::iterator it = beg; it != end; ++it)
		{
				if((*it)->get_remain_time() >= p_timer->get_remain_time())
				{
						m_set.insert(it, p_timer);
						return;
				}
		}
		m_set.push_back(p_timer);

}

#endif

*/












}