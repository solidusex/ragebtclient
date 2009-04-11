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
#include "Thread.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/ThreadImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace ThreadSpace {


t_ulong Thread::GetCurrentThreadId()
{
		return ThreadImpl::GetCurrentThreadId();
}

void* Thread::GetThreadHandle()const
{
		return m_pimpl->GetThreadHandle();
}

Thread::Thread(ThreadFunction func, void *context)
{
		init(new ThreadCallBack(func, context));


}

void Thread::init(ThreadCallBackAbs *ptcb)
{
		m_pimpl = new ThreadImpl(ptcb);

}


void Thread::Sleep(t_ulong milliseconds)
{
		ThreadImpl::Sleep(milliseconds);
}

		
void Thread::YieldThread()
{
		ThreadImpl::YieldThread();
}
/*
void Thread::Detach()
{
		m_pimpl->Detach();
}*/

		
bool Thread::Join(t_ulong milliseconds)
{
		return m_pimpl->Join(milliseconds);
}

void Thread::Join()
{
		m_pimpl->Join();
}
		
void Thread::Terminate()
{
		m_pimpl->Terminate();
}


Priority Thread::GetPriority()const
{
		return m_pimpl->GetPriority();
}
		
bool Thread::SetPriority(Priority p)
{
		return m_pimpl->SetPriority(p);
}
		
t_ulong Thread::Tid()const
{
		return m_pimpl->Tid();
}

bool Thread::IsActive()const
{
		return m_pimpl->IsActive();
}

Thread::~Thread()
{
		try{
				delete m_pimpl;
		}
		catch(const ExptSpace::Exception &expt)
		{
				DEBUG_PRINT2("Expt msg == %s, Error code == %ld : Thread::~Thread()\n", 
						expt.what(),expt.Code());
				assert(0);
		}
		catch(const std::exception &expt)
		{
				DEBUG_PRINT1("Unknow Exception %s : Thread::~Thread()\n", expt.what());
				assert(0);
		}
}



}

