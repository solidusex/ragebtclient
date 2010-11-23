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

#include "AtomicFunction.h"
#include "../Utility/NonCopyable.h"

namespace ThreadSpace {


class FoundationAPI SpinLock : NonCopyable
{
private:
		enum { LOCK = 0x20, UNLOCK };

		typedef AtomicFunction AtomicF;
private:
		volatile size_t			m_state;
public:
		

		class FoundationAPI ScopeLock : private NonCopyable
		{
		private:
				SpinLock	&m_lock;
		public:
				ScopeLock(SpinLock &lock);

				~ScopeLock();
		};
public:
		SpinLock();

		~SpinLock();
private:
		void Lock();
		
		void UnLock();
};


class FoundationAPI RecursiveSpinLock : NonCopyable
{
private:
		enum { LOCK = 0x20, UNLOCK };

		typedef ThreadSpace::AtomicFunction AtomicF;
private:
		volatile size_t			m_state;
		volatile t_ulong		m_recu_count;
		volatile t_ulong		m_tid;
public:
		class FoundationAPI ScopeLock : private NonCopyable
		{
		private:
				RecursiveSpinLock	&m_lock;
		public:
				ScopeLock(RecursiveSpinLock &lock);

				~ScopeLock();
		};
public:
		RecursiveSpinLock();

		~RecursiveSpinLock();
public:
		void Lock();

		void UnLock();

		bool TryLock();

		bool IsOwner()const;
};



}























#if(0)
class FoundationAPI SpinLock : NonCopyable
{
private:
		volatile t_ulong			m_owner_tid;

		volatile t_ulong			m_locked_count;
private:
		typedef ThreadSpace::AtomicFunction AtomicF;
public:
		class FoundationAPI ScopeLock
		{
		private:
				SpinLock		&m_lock;
		public:
				ScopeLock(SpinLock &lock);
				~ScopeLock();
		};

		SpinLock();

		~SpinLock();

		bool TryLock(size_t count);
		
		void Lock();
		
		void UnLock();
		
};

#endif