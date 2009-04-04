
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

#include "Event.h"
#include "SpinLock.h"
#include <queue>
#include <list>
#include <algorithm>

#include <deque>



namespace ThreadSpace {


class FoundationAPI TSDataPriorityQueue : NonCopyable
{
private:
		struct WaitInfo
		{
				void				*pt;
				Event				m_event;
				
				WaitInfo() : m_event(false), pt(0)
				{

				}
		};
		class PriorityInfo
		{
		public:
				size_t			m_priority;
				void			*m_ptr;
		public:
				PriorityInfo();
				
				bool operator<(const PriorityInfo &other)const;
				
		};

		typedef std::priority_queue<PriorityInfo>		DataQueue;

		typedef std::deque<WaitInfo*>					WaitQueue;
		
		typedef SpinLock								MutexType;
		
		typedef MutexType::ScopeLock					FMLock;
private:
		DataQueue				m_queue;
		WaitQueue				m_wait_queue;
		mutable MutexType		m_mutex;		
public:
		void Enqueue(const void *ptr, size_t level);

		bool WaitDequeue(void *&ptr, t_ulong milliseconds);

		bool PeekQueue(void *&ptr)const;			//注意，多线程共同读的时候不可以使用

		void WaitDequeue(void *&ptr);

		bool HasIdleThread()const;

		bool IsEmpty()const;

		size_t Size()const;

		void Clear();

		TSDataPriorityQueue();

		~TSDataPriorityQueue();
};


}