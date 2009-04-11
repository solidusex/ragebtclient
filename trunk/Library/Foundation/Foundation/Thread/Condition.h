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
#include <list>

namespace ThreadSpace {


class FoundationAPI Condition : NonCopyable
{
private:
		typedef std::list<Event*>		WaitQueue;
		typedef RecursiveSpinLock		MutexType;
		typedef MutexType::ScopeLock	LockType;
private:
		WaitQueue						m_queue;
		MutexType						m_mutex;

private:
		void dequeue();
		void dequeue(Event &event);
		void enqueue(Event &event);
public:
		Condition();
		
		~Condition();

public:
		template<typename Mtx>
		bool Wait(Mtx &mtx, t_uint64 timeout)//∫¡√Î
		{
				Event event(false);
				m_mutex.Lock();
				ScopeGuard unlock_and_lock(&mtx, &Mtx::UnLock, &Mtx::Lock);
				enqueue(event);
				m_mutex.UnLock();

				if(!event.Wait(timeout))
				{
						LockType lock(m_mutex);
						dequeue(event);
						return false;
				}
				return true;

		}
		
		template<typename Mtx>
		void Wait(Mtx &mtx)
		{
				Event event(false);
				m_mutex.Lock();
				ScopeGuard unlock_and_lock(&mtx, &Mtx::UnLock, &Mtx::Lock);
				enqueue(event);
				m_mutex.UnLock();
				event.Wait();
		}

		void Signal();

		void BroadCast();
};



















}