
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

#include "../Common/ComnHdr.h"
#include "Semaphore.h"
#include "SpinLock.h"

namespace ThreadSpace {


class  FoundationAPI  ReaderWriterMutex  : private NonCopyable
{
private:
		typedef SpinLock		MutexType;
private:
		Semaphore		m_sema_readers;
		Semaphore		m_sema_writers;
		MutexType		m_cs;
		t_uint32		m_waiting_readers;
		t_uint32		m_waiting_writers;
		t_int32			m_active;
public:
		class  FoundationAPI ScopeLock : private NonCopyable
		{
		private:
				ReaderWriterMutex	 &m_mutex;
		public:
				ScopeLock(ReaderWriterMutex	 &mutex, bool lock_read = true);

				~ScopeLock();
		};
public:
		ReaderWriterMutex();
		~ReaderWriterMutex();
		void WaitToRead();
		void WaitToWrite();
		void Done();
};




}