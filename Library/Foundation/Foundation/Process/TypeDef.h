
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

#include "../System/SysHdr.h"

#include <map>
#include <string>


namespace ProcessSpace {

typedef t_ulong		PID;


struct ProcessStartInfo
{
		std::map<std::wstring, std::wstring>		env;
		std::wstring								cur_dir;
};

}






