
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
#include "ProcessImpl.h"

#include <vector>

namespace ProcessSpace {

t_ulong ProcessHandleImpl::GetPriority()const
{
		return ::GetPriorityClass(m_hdl);
}

bool ProcessHandleImpl::SetPriority(t_ulong priority)const
{
		return (::SetPriorityClass(m_hdl, priority) == TRUE);
}

ProcessHandleImpl::ProcessHandleImpl(HANDLE hdl) : m_hdl(hdl)
{
		assert(m_hdl);
}

PID ProcessHandleImpl::Id()const
{
		return (PID)::GetProcessId(m_hdl);
}

ProcessHandleImpl::~ProcessHandleImpl()
{
		::CloseHandle(m_hdl);

}

t_int32 ProcessHandleImpl::Wait()
{
		DWORD rc = WaitForSingleObject(m_hdl, INFINITE);
		if (rc != WAIT_OBJECT_0)
		{
				throw ExceptionSpace::SystemException("Wait for Process Failed!", ::GetLastError());
		}
		

		DWORD ec = 0;
		
		if (GetExitCodeProcess(m_hdl, &ec) == 0)
		{
				throw ExceptionSpace::SystemException("Wait for Process Failed!", ::GetLastError());

		}
		
		return ec;
}






bool ProcessImpl::Kill(PID pid)
{
		HANDLE hdl = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
		if (hdl)
		{
				bool res = (TerminateProcess(hdl, 0) == 0);
				CloseHandle(hdl);
				return res;
		}else
		{
				return false;
		}
				


}



void ProcessImpl::RequestTermination(PID pid)
{
		Kill(pid);
}



ProcessHandleImpl* ProcessImpl::Launch(const std::wstring &path, const std::wstring &command, ProcessStartInfo &si)
{
		//std::auto_ptr<wchar_t> pbuf(new WCHAR[command.size() + 1]);
		
		std::vector<wchar_t> buf(command.size() + 1);
		std::copy(command.begin(), command.end(), buf.begin());

		std::map<std::wstring, std::wstring>  env_bak;

		SysSpace::Environment env;

		for(std::map<std::wstring, std::wstring>::iterator it = si.env.begin(); it != si.env.end(); ++it)
		{
				if(env.Has(it->first))
				{
						env_bak[it->first] = env.Get(it->first);
				}
				env.Set(it->first, it->second);
		}


		::PROCESS_INFORMATION						pi;
		memset((void*)&pi, 0, sizeof(pi));

		STARTUPINFOW startupInfo;
		GetStartupInfoW(&startupInfo);
		startupInfo.cb          = sizeof(STARTUPINFO);
		startupInfo.lpReserved  = NULL;
		startupInfo.lpDesktop   = NULL;
		startupInfo.lpTitle     = NULL;
		startupInfo.dwFlags     = STARTF_FORCEOFFFEEDBACK | STARTF_USESTDHANDLES;
		startupInfo.cbReserved2 = 0;
		startupInfo.lpReserved2 = NULL;


		BOOL success = ::CreateProcessW(path.c_str(), &buf[0], 0, 0, FALSE,0, 0, si.cur_dir.c_str(), &startupInfo, &pi);
		
		for(std::map<std::wstring, std::wstring>::iterator it = env_bak.begin(); it != env_bak.end(); ++it)
		{
				env.Set(it->first, it->second);
		}
		
		if(success)
		{
				ProcessHandleImpl *pimpl = new ProcessHandleImpl(pi.hProcess);
				return pimpl;
		}else
		{
				return 0;
		}

}



















}