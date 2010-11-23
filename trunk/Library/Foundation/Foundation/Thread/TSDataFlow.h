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
#pragma once

#include "TypeDef.h"
#include "SpinLock.h"
#include "Condition.h"
#include "../Time/TimeHdr.h"





namespace ThreadSpace {




class FoundationAPI TSDataFlow : NonCopyable
{
private:
		typedef std::list<void*>		DataQueue;
		typedef RecursiveSpinLock		MutexType;
		typedef MutexType::ScopeLock	LockType;
private:
		t_uint32						m_hi_water_mark;
		t_uint32						m_lo_water_mark;
		DataQueue						m_queue;
		
		mutable MutexType				m_mutex;
		mutable Condition				m_hi_cond;
		mutable Condition				m_lo_cond;

private:
		void notify_dequeue();
		void notify_enqueue();
public:
		TSDataFlow(t_uint32 hi_mark, t_uint32 lo_mark = 1);

		~TSDataFlow();

		t_uint32 LowWaterMark()const;

		void LowWaterMark(t_uint32 mark);

		t_uint32 HighWaterMark()const;

		void HighWaterMark(t_uint32 mark);

		bool Enqueue(void *pdata, t_uint64 timeout);//∫¡√Î

		bool Dequeue(void *& pres, t_ulong timeout);
		
		void Enqueue(void *pdata);

		void Dequeue(void *& pres);

		void Clear();

		size_t Size()const;

		bool IsEmpty()const;


};























}