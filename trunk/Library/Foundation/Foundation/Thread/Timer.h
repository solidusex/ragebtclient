
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
#include "SpinLock.h"
#include "Thread.h"
#include "../Time/TimeHdr.h"
#include <list>

namespace ThreadSpace {


class TimerManager;

class FoundationAPI Timer : public ::RefCountedObject
{
private:
		friend class TimerManager;
		typedef TimeSpace::TimeStamp	TimeStampT;
private:
		ThreadCallBackAbs				*m_cb;
		bool							m_is_one_off;
		t_uint64						m_interval;
		mutable TimeStampT				m_ts;

private:
		void run();
		void update()const;
		bool is_one_off()const;
		bool is_expired()const;
public:
		Timer(ThreadFunction proc, void *context, t_uint64 interval = 1000, bool is_one_off = false);

		template<class PointerType, class MFunc>
		Timer(const PointerType &pobj, MFunc mf, t_uint64 interval = 1000, bool is_one_off = false);

		~Timer();

		void ResetInterval(t_uint64 interval);
};





template<class PointerType, class MFunc>
Timer::Timer(const PointerType &pobj, MFunc mf, t_uint64 interval, bool is_one_off) 
		: m_cb(new ThreadCallBackMF<PointerType, MFunc>(pobj, mf))
		, m_interval(interval)
		, m_is_one_off(is_one_off)
{
		assert(m_interval != 0);
		assert(m_cb != 0);
}























class FoundationAPI TimerManager : private NonCopyable
{
private:
		typedef std::list<Timer*>		TimerSet;
		typedef SpinLock				MutexType;
		typedef MutexType::ScopeLock	LockType;
private:
		TimerSet						m_set;
		mutable MutexType				m_mutex;
		t_uint32						m_resolution;
		Thread							*m_thread;
		bool							m_is_started;
private:
		void worker_thread();
public:
		TimerManager();
		~TimerManager();
public:
		bool Start(t_uint32 min_resolution);
		
		void Stop();

		bool RegisterTimer(Timer *ptimer);
		
		bool RemoveTimer(Timer *ptimer);

		void Clear();

		size_t Count()const;
};

#if(0)

class TimerManager;

class FoundationAPI Timer : public ::RefCountedObject
{
private:
		friend class TimerManager;
private:
		ThreadCallBackAbs				*m_cb;
		bool							m_is_one_off;
		
		t_uint64						m_interval;

		mutable t_uint64				m_remain_time;
private:
		__forceinline void run();
		__forceinline bool is_one_off()const;
		__forceinline t_uint64 get_remain_time()const;
		__forceinline t_uint64 get_interval()const;
		__forceinline void set_remain_time(t_uint64 remain_time);
		__forceinline void reset_remain_time();
public:
		Timer(ThreadFunction proc, void *context, t_uint64 interval = 1000, bool is_one_off = false);

		template<class PointerType, class MFunc>
		Timer(const PointerType &pobj, MFunc mf, t_uint64 interval = 1000, bool is_one_off = false);

		~Timer();

		void ResetInterval(t_uint64 interval);
};



__forceinline void Timer::run()
{
		m_cb->Run();
}

__forceinline void Timer::reset_remain_time()
{
		assert(m_interval != 0);
		m_remain_time = m_interval;
}

__forceinline bool Timer::is_one_off()const
{
		return m_is_one_off;
}

__forceinline t_uint64 Timer::get_remain_time()const
{
		return m_remain_time;
}

__forceinline t_uint64 Timer::get_interval()const
{
		return m_interval;
}

__forceinline void Timer::set_remain_time(t_uint64 remain_time)
{
		m_remain_time = remain_time;
}


template<class PointerType, class MFunc>
Timer::Timer(const PointerType &pobj, MFunc mf, t_uint64 interval, bool is_one_off) 
		: m_cb(new ThreadCallBackMF<PointerType, MFunc>(pobj, mf))
		, m_interval(interval)
		, m_is_one_off(is_one_off)
{
		assert(m_interval != 0);
		m_remain_time = m_interval;
		assert(m_cb != 0);
}























class FoundationAPI TimerManager : private NonCopyable
{
private:
		typedef std::list<Timer*>		TimerSet;
		typedef SpinLock				MutexType;
		typedef MutexType::ScopeLock	LockType;
private:
		TimerSet						m_set;
		mutable MutexType				m_mutex;
		t_uint32						m_resolution;
		Thread							*m_thread;
		bool							m_is_started;
private:
		void insert_timer(Timer *p_timer);

		void worker_thread();
public:
		TimerManager();
		~TimerManager();
public:
		bool Start(t_uint32 min_resolution);
		
		void Stop();

		bool RegisterTimer(Timer *ptimer);
		
		bool RemoveTimer(Timer *ptimer);

		void Clear();

		size_t Count()const;
};

#endif


}