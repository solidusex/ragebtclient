
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
#include "Process.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/ProcessImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace ProcessSpace {


t_ulong ProcessHandle::GetPriority()const
{
		return m_pimpl->GetPriority();
}



bool ProcessHandle::SetPriority(t_ulong priority)const
{
		return m_pimpl->SetPriority(priority);
}




PID	ProcessHandle::Id()const
{
		return m_pimpl->Id();
}
		

t_int32 ProcessHandle::Wait()
{
		return m_pimpl->Wait();
}
		

ProcessHandle::ProcessHandle(const ProcessHandle &other) : m_pimpl(0)
{
		*this = other;
}


ProcessHandle& ProcessHandle::operator= (const ProcessHandle &other)
{
		if(this != &other)
		{
				assert(m_pimpl);
				assert(other.m_pimpl);
				m_pimpl->Release();
				m_pimpl = 0;
				other.m_pimpl->Duplicate();
				m_pimpl = other.m_pimpl;
		}
		
		return *this;
}


ProcessHandle::ProcessHandle(ProcessHandleImpl *impl) : m_pimpl(impl)
{
		assert(impl);
}


ProcessHandle::~ProcessHandle()
{
		m_pimpl->Release();
}




ProcessHandle Process::Launch(const std::wstring &path, const std::wstring &command, ProcessStartInfo &si)
{
		ProcessHandleImpl *pimpl = ProcessImpl::Launch(path, command, si);
		
		assert(pimpl);

		if(pimpl == 0)
		{

				throw ExceptionSpace::RuntimeException("Can't Execute Application~!", ::GetLastError());
		}
		
		return ProcessHandle(pimpl);
}
		
bool Process::Kill(PID pid)
{
		return ProcessImpl::Kill(pid);
}

void RequestTermination(PID pid)
{
		ProcessImpl::RequestTermination(pid);
}




}