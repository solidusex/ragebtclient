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
#include "TimeStamp.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/TimeStampImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif


namespace TimeSpace {


TimeStamp::TimeDiff TimeStamp::ElapsedMillisecond()const
{
		return (Elapsed() / 1000);
}


TimeStamp::TimeDiff TimeStamp::ElapsedSecond()const
{
		return ElapsedMillisecond() / 1000;

}

TimeStamp::TimeVal TimeStamp::Resolution()
{
		return 1000000;
}


void TimeStamp::Update()
{
		m_ts = GetTime();
}

TimeStamp::TimeStamp()
{
		Update();
}

TimeStamp::TimeStamp(TimeVal tv) : m_ts(tv)
{

}


TimeStamp::TimeStamp(const TimeStamp& other)
{
		m_ts = other.m_ts;
}

TimeStamp::~TimeStamp()
{

}

TimeStamp& TimeStamp::operator = (const TimeStamp& other)
{
		if(this != &other)
		{
				m_ts = other.m_ts;
		}
		return *this;
}

TimeStamp& TimeStamp::operator = (TimeVal tv)
{
		m_ts = tv;
		return *this;
}

void TimeStamp::Swap(TimeStamp &other)
{
		TimeVal tmp = other.m_ts;
		other.m_ts = m_ts;
		m_ts = tmp;
}



bool TimeStamp::operator == (const TimeStamp& other) const
{
		return m_ts == other.m_ts;
}


bool TimeStamp::operator != (const TimeStamp& ts) const
{
		return m_ts != ts.m_ts;
}


bool TimeStamp::operator >  (const TimeStamp& ts) const
{
		return m_ts > ts.m_ts;
}


bool TimeStamp::operator >= (const TimeStamp& ts) const
{
		return m_ts >= ts.m_ts;
}


bool TimeStamp::operator <  (const TimeStamp& ts) const
{
		return m_ts < ts.m_ts;
}


bool TimeStamp::operator <= (const TimeStamp& ts) const
{
		return m_ts <= ts.m_ts;
}


TimeStamp TimeStamp::operator + (TimeStamp::TimeDiff d) const
{
		return TimeStamp(m_ts + d);
}


TimeStamp TimeStamp::operator - (TimeStamp::TimeDiff d) const
{
		return TimeStamp(m_ts - d);
}


TimeStamp::TimeDiff TimeStamp::operator - (const TimeStamp& ts) const
{
		return m_ts - ts.m_ts;
}


TimeStamp& TimeStamp::operator += (TimeStamp::TimeDiff d)
{
		m_ts += d;
		return *this;
}


TimeStamp& TimeStamp::operator -= (TimeStamp::TimeDiff d)
{
		m_ts -= d;
		return *this;
}



time_t TimeStamp::EpochTime() const
{
		time_t tv = m_ts/Resolution();
		return tv;
}


TimeStamp::UtcTimeVal TimeStamp::UtcTime() const
{
		return m_ts*10 + (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
}


TimeStamp::TimeVal TimeStamp::EpochMicroseconds()const
{
		return m_ts;
}


TimeStamp::TimeDiff TimeStamp::Elapsed() const
{
		TimeStamp now;
		return now - *this;
}



bool TimeStamp::IsElapsed(TimeStamp::TimeDiff interval) const
{
		TimeStamp now;
		TimeStamp::TimeDiff diff = now - *this;
		return diff >= interval;
}




















}