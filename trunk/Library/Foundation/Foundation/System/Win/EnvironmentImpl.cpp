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
#include "EnvironmentImpl.h"



namespace SysSpace {


EnvVariable EnvironmentImpl::GetEnvVar()
{
		wchar_t *penv = ::GetEnvironmentStringsW();

		wchar_t *pwstr = penv;

		EnvVariable env_map;
		for(pwstr; *pwstr; ++pwstr)
		{
				std::wstring wtmp;
				while(*pwstr)
				{
						wtmp += (*pwstr++);
				}
				
				if(!wtmp.empty())
				{
						if(wtmp[0] == L'=')
						{
								continue;
						}else
						{
								size_t idx = wtmp.find(L'=');
								if(idx == std::wstring::npos)
								{
										continue;
								}else
								{
										std::wstring key = wtmp.substr(0, idx - 0);
										std::wstring val = wtmp.substr(idx+1);
										env_map[key] = val;
								}

						}
				}
				
		}

		::FreeEnvironmentStringsW(penv);
		
		return env_map;
}


bool EnvironmentImpl::Set(const std::wstring &name, const std::wstring &value)
{
		return (::SetEnvironmentVariableW(name.c_str(), value.c_str()) == TRUE);
}

bool EnvironmentImpl::Has(const std::wstring &name)
{
		return (::GetEnvironmentVariableW(name.c_str(), 0, 0) > 0);
}


std::wstring EnvironmentImpl::Get(const std::wstring &name)
{
		DWORD  len = ::GetEnvironmentVariableW(name.c_str(), 0, 0);
		
		if(len == 0) return L"";

		std::auto_ptr<wchar_t> pbuf(new wchar_t[len + 1]);
		
		len = ::GetEnvironmentVariableW(name.c_str(), pbuf.get(), len + 1);

		if(len == 0)
		{
				throw ExceptionSpace::RuntimeException( "Get Env Value Failed!", ::GetLastError());
		}
		return pbuf.get();
}


SystemInfo EnvironmentImpl::GetSystemInfo()
{
		SystemInfo info;
		::SYSTEM_INFO sys_info;
		::GetSystemInfo(&sys_info);
		info.allocation_granularity = sys_info.dwAllocationGranularity;
		info.min_app_addr = (t_uint64)sys_info.lpMinimumApplicationAddress;
		info.max_app_addr = (t_uint64)sys_info.lpMaximumApplicationAddress;
		info.page_size = sys_info.dwPageSize;
		info.processor_num = sys_info.dwNumberOfProcessors;
		return info;
}

std::wstring EnvironmentImpl::GetNodeName()
{
		wchar_t name[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
		if (GetComputerNameW(name, &size) == 0) 
		{
				throw ExceptionSpace::SystemException("Cannot get computer name", ::GetLastError());
		}
		
		return std::wstring(name);


}


}