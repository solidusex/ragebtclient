
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

#include "ThreadImpl.h"
#include <windows.h>
#include <process.h>



namespace ThreadSpace {

/***********************************ThreadImplBase***********************************/

t_ulong ThreadImpl::GetCurrentThreadId()
{
		return ::GetCurrentThreadId();
}

void* ThreadImpl::GetThreadHandle()const
{
		if(m_hdl == 0) return 0;
		return (void*)m_hdl;
}

void ThreadImpl::Sleep(t_ulong milliseconds)
{
		::Sleep(milliseconds);
}

void ThreadImpl::YieldThread()
{
		::Sleep(0);
}

#if(0)
t_ulong ThreadImpl::unique_id()
{
		static t_ulong id = 1;
		
		return (t_ulong)::InterlockedIncrement((LONG*)&id);
		
}
#endif

bool ThreadImpl::Join(t_ulong milliseconds)
{
		
		//if (m_hdl == 0) return false;
		assert(m_tid != ::GetCurrentThreadId());
		if(m_tid == ::GetCurrentThreadId())
		{
				return false;
		}
		
		DWORD res = ::WaitForSingleObject(m_hdl, milliseconds);
		
		switch(res)
		{
		case WAIT_TIMEOUT:
				return false;
		case WAIT_OBJECT_0:
				return true;
		default:
				throw ExceptionSpace::SystemException("can't join thread", ::GetLastError());
		}
		
}
		
void ThreadImpl::Join()
{
		Join(INFINITE);
}

void ThreadImpl::Terminate()
{
		::TerminateThread(m_hdl, 1);
}


Priority ThreadImpl::GetPriority()const
{
		int res = ::GetThreadPriority(m_hdl);
		if(res == THREAD_PRIORITY_ERROR_RETURN)
		{
				throw ExceptionSpace::SystemException("cannot get thread priority", ::GetLastError());
				
		}

		switch(res){
		case THREAD_PRIORITY_TIME_CRITICAL:
		case THREAD_PRIORITY_HIGHEST:
				return HIGH;
		case THREAD_PRIORITY_ABOVE_NORMAL:
		case THREAD_PRIORITY_NORMAL:
		case THREAD_PRIORITY_BELOW_NORMAL:
				return NORMAL;
		default:
				return LOW;				//case THREAD_PRIORITY_LOWEST:
										//case THREAD_PRIORITY_IDLE:
		}

}
		
bool ThreadImpl::SetPriority(Priority p)
{		
		switch(p){
		case HIGH:
				return (::SetThreadPriority(m_hdl, THREAD_PRIORITY_HIGHEST) != 0);
		case LOW:
				return (::SetThreadPriority(m_hdl, THREAD_PRIORITY_LOWEST) != 0);
		default:
				return (::SetThreadPriority(m_hdl, THREAD_PRIORITY_NORMAL) != 0);
		}

}
		
t_ulong ThreadImpl::Tid()const
{
		if(IsActive())
		{
				return m_tid;
		}else
		{
				return (t_ulong)~0;
		}

}

bool ThreadImpl::IsActive()const
{
		t_ulong ec = 0;
		::GetExitCodeThread(m_hdl, &ec);
		return (ec == STILL_ACTIVE);
}

void ThreadImpl::Detach()
{
		if(m_hdl != 0)
		{
				::CloseHandle(m_hdl);
				m_hdl = 0;
		}
}


ThreadImpl::~ThreadImpl()
{
		
		if(IsActive())
		{
				if(m_hdl != 0)
				{
						Terminate();
						Join(INFINITE);
						::CloseHandle(m_hdl);
				}
		}
}

ThreadImpl::ThreadImpl(ThreadCallBackAbs *ptcb)
{
		assert(ptcb != 0);
		m_ptcb = ptcb;

		m_hdl = 0;
		m_tid = 0;
		
		t_uint32 x;

		m_hdl = (HANDLE)_beginthreadex(0, 0, ThreadFuncInstance, (void*)m_ptcb, 0, &x);
		
		if(m_hdl == 0)
		{
				throw ExceptionSpace::SystemException("Can not create thread", ::GetLastError());
		}
		
		m_tid = x;
}



t_uint32 ThreadImpl::ThreadFuncInstance(void *param)
{
		
		try{
				ThreadCallBackAbs		*ptcb = (ThreadCallBackAbs*)param;

				ptcb->Run();

				delete ptcb;
		}catch(const std::exception &expt)
		{
				DEBUG_PRINT2("Thread %ld has unhandled exception %s\n", ::GetCurrentThreadId(), expt.what());
				assert(0);
		}

		return 0;
}







}