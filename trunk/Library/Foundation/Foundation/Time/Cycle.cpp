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
#include "Cycle.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/CycleImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif


namespace TimeSpace {


t_uint64 CycleTimer::GetCycleCount()
{
		return rdtsc();
		
}


void CycleTimer::Start()
{
		m_startcycle = GetCycleCount();
}

t_uint64 CycleTimer::Stop()
{
		return (GetCycleCount() - m_startcycle - m_overhead);
}

CycleTimer::~CycleTimer()
{

}

CycleTimer::CycleTimer() : m_overhead(0), m_startcycle(0)
{
		Start();
		t_uint64 count = Stop();

		for(size_t i = 0; i < 10; ++i)
		{
				Start();
				count = std::min(count,Stop());
		}
		m_overhead = count;
}


t_uint64 CycleTimer::GetOverHead()const
{
		return m_overhead;
}





}