
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
#pragma once
#include "TypeDef.h"
#include <ctime>


namespace TimeSpace {




class FoundationAPI TimeStamp
{
public:
		typedef t_uint64 TimeVal;    /// monotonic UTC time value in microsecond resolution
		typedef t_uint64 UtcTimeVal; /// monotonic UTC time value in 100 nanosecond resolution
		typedef t_uint64 TimeDiff;   /// difference between two TimeStamps in microseconds
private:
		TimeVal					m_ts;
public:
		TimeStamp();
		
		TimeStamp(TimeVal tv);
		
		TimeStamp(const TimeStamp& other);
		
		~TimeStamp();
		
		TimeStamp& operator = (const TimeStamp& other);

		TimeStamp& operator = (TimeVal tv);

		void Swap(TimeStamp& other);
		

		void Update();

		bool operator == (const TimeStamp& ts) const;
		bool operator != (const TimeStamp& ts) const;
		bool operator >  (const TimeStamp& ts) const;
		bool operator >= (const TimeStamp& ts) const;
		bool operator <  (const TimeStamp& ts) const;
		bool operator <= (const TimeStamp& ts) const;

		TimeStamp  operator+  (TimeDiff d) const;
		TimeStamp  operator-  (TimeDiff d) const;
		TimeDiff   operator-  (const TimeStamp& ts) const;
		TimeStamp& operator+= (TimeDiff d);
		TimeStamp& operator-= (TimeDiff d);

		time_t EpochTime() const;
		/// Returns the TimeStamp expressed in time_t.
		/// time_t base time is midnight, January 1, 1970.
		/// Resolution is one second.

		UtcTimeVal UtcTime() const;
		/// Returns the TimeStamp expressed in UTC-based
		/// time. UTC base time is midnight, October 15, 1582.
		/// Resolution is 100 nanoseconds.

		TimeVal EpochMicroseconds() const;
		/// Returns the TimeStamp expressed in microseconds
		/// since the Unix epoch, midnight, January 1, 1970.

		TimeDiff Elapsed() const;
		/// Returns the time elapsed since the time denoted by
		/// the TimeStamp. Equivalent to TimeStamp() - *this.

		TimeDiff ElapsedMillisecond()const;

		TimeDiff ElapsedSecond()const;

		bool IsElapsed(TimeDiff interval) const;
		/// Returns true iff the given interval has passed
		/// since the time denoted by the TimeStamp.

		//static TimeStamp FromEpochTime(time_t t);
		/// Creates a TimeStamp from a time_t.

		//static TimeStamp FromUtcTime(UtcTimeVal val);
		/// Creates a TimeStamp from a UTC time value.

		static TimeVal Resolution();
		/// Returns the resolution in units per second.
		/// Since the TimeStamp has microsecond resolution,
		/// the returned value is always 1000000.
};






}