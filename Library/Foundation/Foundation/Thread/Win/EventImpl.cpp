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
#include "EventImpl.h"
#include "../TypeDef.h"

namespace ThreadSpace {


EventImpl::EventImpl(bool is_auto)
{
		BOOL set_auto = (is_auto ? FALSE : TRUE);

		m_event_hdl = ::CreateEvent(0, set_auto, FALSE, 0);

		
		assert(m_event_hdl != NULL);
		if(m_event_hdl == NULL)
		{
				throw ExceptionSpace::SystemException("EventImpl::EventImpl(bool is_auto) Failed!", ::GetLastError());
		}

}


EventImpl::~EventImpl()
{
		::CloseHandle(m_event_hdl);
}

bool EventImpl::Wait(t_uint64 timeout)
{
		switch(::WaitForSingleObject(m_event_hdl, (DWORD)timeout)){
		case WAIT_OBJECT_0:
				return true;
		case WAIT_TIMEOUT:
				return false;
		default:
				assert(0);
				throw ExceptionSpace::SystemException("can not wait the event object", ::GetLastError());
		}
}

void EventImpl::Wait()
{
		switch(::WaitForSingleObject(m_event_hdl, INFINITE))
		{
		case WAIT_OBJECT_0:
				return;
		case WAIT_TIMEOUT:
		default:
				assert(0);
				throw ExceptionSpace::SystemException("can not wait the event object", ::GetLastError());
		}
}



void EventImpl::Set()
{
		::SetEvent(m_event_hdl);
}


void EventImpl::Reset()
{
		::ResetEvent(m_event_hdl);

}


bool EventImpl::TryWait()
{
		return Wait(0);
}

}



















#if(0)

EventHandle::EventHandle()
{
		m_hdl = ::CreateEvent(0, TRUE, FALSE, 0);
		assert(m_hdl);
		if(m_hdl == 0)
		{
				ThrowThreadException(::GetLastError(), "Create Event object failed");
		}
}

bool EventHandle::Wait(t_ulong timeout)
{
		switch(::WaitForSingleObject(m_hdl, timeout))
		{
		case WAIT_OBJECT_0:
				return true;
		case WAIT_TIMEOUT:
				return false;
		default:
				assert(0);
				ThrowThreadException(::GetLastError(), "can not wait the condition object");
		}
}

void EventHandle::Set()
{
		if(::SetEvent(m_hdl) == FALSE)
		{
				ThrowThreadException(::GetLastError(), "Can't set the event");
		}
}

void EventHandle::ReSet()
{
		if(::ResetEvent(m_hdl) == FALSE)
		{		
				ThrowThreadException(::GetLastError(), "Can't reset the event");
		}
}

EventHandle::~EventHandle()
{
		assert(m_hdl);
		::CloseHandle(m_hdl);
}


bool EventImpl::Wait(t_ulong timeout)
{
		DWORD tid = ::GetCurrentThreadId();

		EventHandle *peh = 0;

		try{
				peh = new EventHandle();

				{
						MutexType::ScopeLock lock(m_mutex);

#if defined(FOUNDATION_DEBUG)
						assert((m_set.count(tid)) == 0);
#endif

						m_set[tid] = peh;
				}

				bool is_ok =  peh->Wait(timeout);

				delete peh;
				return is_ok;
		}catch(...)
		{
				if(peh != 0)
				{
						delete peh;
				}

				ThrowThreadException(::GetLastError(), "Can't wait evnet");
		}
}



void EventImpl::Wait()
{
		Wait(INFINITE);
}
bool EventImpl::TryWait()
{
		return Wait(0);
}

void EventImpl::Signal()
{

		MutexType::ScopeLock lock(m_mutex);
		try{
				if(m_set.size() != 0)
				{
						EventHandle *peh = m_set.begin()->second;
						peh->Set();
						m_set.erase(m_set.begin());
				}
		}
		catch(ThreadException &expt)
		{
				m_set.erase(m_set.begin());
				throw;
		}
}

void EventImpl::Broadcast()
{
		MutexType::ScopeLock lock(m_mutex);
		
		for(EventSet::iterator it = m_set.begin(); it != m_set.end(); ++it)
		{
				try{
						it->second->Set();
				}catch(...)
				{
						//ThrowThreadException(::GetLastError(), "BroasdCast Failed");
						continue;
				}
		}

		m_set.clear();

}






#endif