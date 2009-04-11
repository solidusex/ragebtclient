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

		//�����ֻ��һ���̶߳���ʱ���ã�Ҳ����˵�����ô˺���ʱ��
		//���ܴ����κ��߳��ڵ���wait_dequeue����,���ǻ�����󣬵���void*��˳����������
		void EnqueueToFront(const void *ptr);

		bool WaitDequeue(void *&ptr, t_ulong milliseconds);

		bool PeekQueue(void *&ptr)const;			//ע�⣬���̹߳�ͬ����ʱ�򲻿���ʹ��

		void WaitDequeue(void *&ptr);

		bool HasIdleThread()const;

		bool IsEmpty()const;

		size_t Size()const;

		void Clear();

		TSDataQueue();

		~TSDataQueue();
};

}

