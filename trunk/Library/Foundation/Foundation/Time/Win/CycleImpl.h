
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
#include "../Utility/UtilityHdr.h"

namespace TimeSpace{


#if(ARCH == ARCH_IA32 )//|| ARCH == ARCH_X64)

inline t_uint64 rdtsc()
{
		t_uint64 res;
		__asm
		{
			pushad	;
			CPUID	;
			RDTSC	;
			lea ebx, res;
			mov	[ebx], eax;
			mov	[ebx + 4], edx;
			popad;
		}
		return res;

}

#else

		#error NOT_SUPPORT_PLATFORM

#endif















}