
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
#include "ReaderWriterMutex.h"



namespace ThreadSpace {


ReaderWriterMutex::ScopeLock::ScopeLock(ReaderWriterMutex &mutex, bool lock_read) :m_mutex(mutex)
{
		if(lock_read)
		{
				m_mutex.WaitToRead();
		}else
		{
				m_mutex.WaitToWrite();
		}
}

ReaderWriterMutex::ScopeLock::~ScopeLock()
{
		m_mutex.Done();

}

ReaderWriterMutex::ReaderWriterMutex() : m_sema_readers(0, MAX_LONG), 
						m_sema_writers(0, MAX_LONG), 
						m_cs(),
						m_waiting_readers(0),
						m_waiting_writers(0),
						m_active(0)
{

}


ReaderWriterMutex::~ReaderWriterMutex() 
{

}


void ReaderWriterMutex::WaitToRead()
{
		bool is_pending;
		{
				MutexType::ScopeLock lock(m_cs);

				is_pending = (m_waiting_writers || (m_active < 0));

				if(is_pending){
						++m_waiting_readers;
				}else {
						++m_active;
				}
		}

		if(is_pending){
				m_sema_readers.Acquire();
		}
}


void ReaderWriterMutex::WaitToWrite()
{
		bool resource_owned;
		{
				MutexType::ScopeLock lock(m_cs);

				resource_owned = (m_active != 0);

				if(resource_owned){
						++m_waiting_writers;
				}else {
						m_active = -1;
				}

		}
		
		if(resource_owned){
				m_sema_writers.Acquire();
		}
}


void ReaderWriterMutex::Done()
{
		Semaphore		*psema	=	0;
		t_ulong			n = 1;

		{
				MutexType::ScopeLock lock(m_cs);
		
				if(m_active > 0)
				{
						--m_active;
				}
				else 
				{
						++m_active;
				}

				if(m_active == 0)
				{
						if(m_waiting_writers > 0)
						{
								m_active = -1;
								--m_waiting_writers;
								psema = &m_sema_writers;
						}
						else if(m_waiting_readers > 0)
						{
								m_active = m_waiting_readers;
								m_waiting_readers = 0;
								psema = &m_sema_readers;
								n = m_active;
						}
						else 
						{

						}
				}
		}
		
		if(psema != 0){
				psema->Release(n);
		}
		
		
}

}