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
#include "../Utility/UtilityHdr.h"
#include "../Exception/ExptHdr.h"

#include <map>
#include <string>


namespace SysSpace {

typedef std::map<std::wstring, std::wstring>	EnvVariable;


struct SystemInfo
{
		t_uint64		page_size;
		t_uint64		min_app_addr;
		t_uint64		max_app_addr;
		t_uint64		allocation_granularity;
		t_ulong			processor_num;
};


}