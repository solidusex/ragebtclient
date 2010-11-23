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

#include "Event.h"
#include "SpinLock.h"

#include <queue>
#include <list>
#include <algorithm>


namespace ThreadSpace {

class FoundationAPI TSDataQueue : NonCopyable
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
		
		typedef std::list<void*>					DataQueue;

		typedef std::deque<WaitInfo*>					WaitQueue;
		
		typedef SpinLock								MutexType;
		
		typedef MutexType::ScopeLock					FMLock;
private:
		DataQueue				m_queue;
		
		WaitQueue				m_wait_queue;
		
		mutable MutexType		m_mutex;		
public:
		
		//void* PeekQueue()const;

		void Enqueue(const void *ptr);

		//最好在只有一个线程读的时候用，也就是说当调用此函数时候
		//不能存在任何线程在调用wait_dequeue函数,并非会出错误，但是void*的顺序会出现问题
		void EnqueueToFront(const void *ptr);

		bool WaitDequeue(void *&ptr, t_ulong milliseconds);

		bool PeekQueue(void *&ptr)const;			//注意，多线程共同读的时候不可以使用

		void WaitDequeue(void *&ptr);

		bool HasIdleThread()const;

		bool IsEmpty()const;

		size_t Size()const;

		void Clear();

		TSDataQueue();

		~TSDataQueue();
};

}

