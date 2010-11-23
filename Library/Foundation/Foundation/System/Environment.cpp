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
#include "Environment.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/EnvironmentImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace SysSpace {


		EnvVariable Environment::GetEnvVar()
		{
				return EnvironmentImpl::GetEnvVar();
		}


		std::wstring Environment::Get(const std::wstring &name)
		{
				return EnvironmentImpl::Get(name);
		}


		bool Environment::Set(const std::wstring &name, const std::wstring &value)
		{
				return EnvironmentImpl::Set(name, value);
		}

		bool Environment::Has(const std::wstring &name)
		{
				return EnvironmentImpl::Has(name);
		}

		SystemInfo Environment::GetSystemInfo()
		{
				return EnvironmentImpl::GetSystemInfo();
		}

		std::wstring Environment::GetNodeName()
		{
				return EnvironmentImpl::GetNodeName();
		}

}