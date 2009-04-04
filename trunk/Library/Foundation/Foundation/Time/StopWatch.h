
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
#include "TimeStamp.h"

namespace TimeSpace {


// A simple facility to measure time intervals
// with microsecond resolution.

class FoundationAPI StopWatch : NonCopyable
{
private:
		TimeStamp					m_start;
		TimeStamp::TimeDiff			m_elapsed;
		bool						m_running;
public:
		StopWatch();
		
		~StopWatch();

		void Start();
		/// Starts (or restarts) the StopWatch.

		void Stop();
		/// Stops or pauses the StopWatch.

		void Reset();
		/// Resets the StopWatch.

		void Restart();
		/// Resets and starts the StopWatch.

		TimeStamp::TimeDiff Elapsed() const;
		/// Returns the elapsed time in microseconds
		/// since the StopWatch started.

		int ElapsedSeconds() const;
		/// Returns the number of seconds elapsed
		/// since the StopWatch started.

		TimeStamp::TimeDiff ElapsedMillisecond()const;

		static TimeStamp::TimeVal Resolution();
		/// Returns the resolution of the StopWatch.
};


}