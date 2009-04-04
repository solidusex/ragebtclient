
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
#pragma once



#include "../TypeDef.h"

#include <windows.h>

#include <string>



namespace SysSpace {


class EnvironmentImpl
{
public:
		static std::wstring Get(const std::wstring &name);

		static bool Set(const std::wstring &name, const std::wstring &value);

		static bool Has(const std::wstring &name);

		static EnvVariable GetEnvVar();

		static SystemInfo GetSystemInfo();

		static std::wstring GetNodeName();

};








}