
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
#include "StopWatch.h"

namespace TimeSpace {




StopWatch::StopWatch(): m_elapsed(0), m_running(false)
{
}


StopWatch::~StopWatch()
{
}


TimeStamp::TimeDiff StopWatch::Elapsed() const
{
		if(m_running)
		{
				TimeStamp current;
				return m_elapsed + (current - m_start);
		}
		else
		{
				return m_elapsed;
		}
}


void StopWatch::Reset()
{
		m_elapsed = 0;
		m_running = false;
}


void StopWatch::Restart()
{
		m_elapsed = 0;
		m_start.Update();
		m_running = true;
}


void StopWatch::Start()
{
		m_start.Update();
		m_running = true;
}


void StopWatch::Stop()
{
		TimeStamp current;
		m_elapsed += current - m_start;
		m_running = false;
}


int StopWatch::ElapsedSeconds() const
{
		return int(Elapsed()/Resolution());
}


TimeStamp::TimeVal StopWatch::Resolution()
{
		return TimeStamp::Resolution();
}



TimeStamp::TimeDiff StopWatch::ElapsedMillisecond()const
{
		return (Elapsed()/1000);
}
























}