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
#include "TSDataFlow.h"

namespace ThreadSpace {


bool TSDataFlow::IsEmpty()const
{
		return Size() == 0;
}


void TSDataFlow::notify_dequeue()
{
		assert(m_lo_water_mark >= 1);
		assert(m_hi_water_mark >= m_lo_water_mark);
				
		if(m_queue.size() >= m_lo_water_mark)
		{
				m_lo_cond.Signal();
		}
}

void TSDataFlow::notify_enqueue()
{
		assert(m_lo_water_mark >= 1);
		assert(m_hi_water_mark >= m_lo_water_mark);
		if(m_queue.size() < m_hi_water_mark)
		{
				m_hi_cond.Signal();
		}
}


TSDataFlow::TSDataFlow(t_uint32 hi_mark, t_uint32 lo_mark)
{
		if(lo_mark == 0) lo_mark = 1;
		if(hi_mark < lo_mark) hi_mark = lo_mark;
		m_lo_water_mark = lo_mark;
		m_hi_water_mark = hi_mark;
}

TSDataFlow::~TSDataFlow()
{
		assert(m_queue.empty());
}

t_uint32 TSDataFlow::LowWaterMark()const
{
		LockType lock(m_mutex);
		return m_lo_water_mark;
}

void TSDataFlow::LowWaterMark(t_uint32 mark)
{
		LockType lock(m_mutex);
		if(mark < 1) mark = 1;
		m_lo_water_mark = mark;
		bool hi_is_modified = false;

		if(m_hi_water_mark < m_lo_water_mark)
		{
				m_hi_water_mark = m_lo_water_mark;
				hi_is_modified = true;
		}
		notify_dequeue();
		
		if(hi_is_modified)
		{
				notify_enqueue();
		}
		
}

t_uint32 TSDataFlow::HighWaterMark()const
{
		LockType lock(m_mutex);
		return m_hi_water_mark;
}

void TSDataFlow::HighWaterMark(t_uint32 mark)
{
		LockType lock(m_mutex);
		if(mark < m_lo_water_mark) mark = m_lo_water_mark;
		m_hi_water_mark = mark;
		notify_enqueue();
}

void TSDataFlow::Enqueue(void *pdata)
{
		LockType lock(m_mutex);
		for(;;)
		{
				assert(m_lo_water_mark >= 1);
				assert(m_hi_water_mark >= m_lo_water_mark);
				if(m_queue.size() < m_hi_water_mark)
				{
						m_queue.push_back(pdata);
						notify_dequeue();
				}else
				{
						m_hi_cond.Wait(m_mutex);
				}
		}
}

void TSDataFlow::Dequeue(void *& pres)
{
		LockType lock(m_mutex);
		for(;;)
		{
				assert(m_lo_water_mark >= 1);
				assert(m_hi_water_mark >= m_lo_water_mark);
				if(m_queue.size() >= m_lo_water_mark)
				{
						pres = m_queue.front();
						m_queue.pop_front();
						notify_enqueue();
				}else
				{
						m_lo_cond.Wait(m_mutex);
				}
		}
}

bool TSDataFlow::Enqueue(void *pdata, t_uint64 timeout)//ºÁÃë
{
		LockType lock(m_mutex);
		TimeSpace::TimeStamp	ts;
		t_uint64 time_left = timeout;

		for(;;)
		{
				assert(m_lo_water_mark >= 1);
				assert(m_hi_water_mark >= m_lo_water_mark);
				if(m_queue.size() < m_hi_water_mark)
				{
						m_queue.push_back(pdata);
						notify_dequeue();
						return true;
				}else
				{
						ts.Update();
						if(!m_hi_cond.Wait(m_mutex, time_left))
						{
								return false;
						}else
						{
								t_uint64 elapsed = ts.ElapsedMillisecond();
								if(elapsed >= time_left)
								{
										return false;
								}else
								{
										time_left -= elapsed;
								}
						}
				}
		}
}




bool TSDataFlow::Dequeue(void *& pres, t_ulong timeout)
{
		LockType lock(m_mutex);
		TimeSpace::TimeStamp	ts;
		t_uint64 time_left = timeout;

		for(;;)
		{
				assert(m_lo_water_mark >= 1);
				assert(m_hi_water_mark >= m_lo_water_mark);
				if(m_queue.size() >= m_lo_water_mark)
				{
						pres = m_queue.front();
						m_queue.pop_front();
						notify_enqueue();
						return true;
				}else
				{
						ts.Update();
						if(!m_lo_cond.Wait(m_mutex, time_left))
						{
								return false;
						}else
						{
								t_uint64 elapsed = ts.ElapsedMillisecond();
								if(elapsed >= time_left)
								{
										return false;
								}else
								{
										time_left -= elapsed;
								}
						}
				}
		}
}

void TSDataFlow::Clear()
{
		LockType lock(m_mutex);
		m_queue.clear();
		notify_enqueue();
}

size_t TSDataFlow::Size()const
{
		LockType lock(m_mutex);
		return m_queue.size();
}




}