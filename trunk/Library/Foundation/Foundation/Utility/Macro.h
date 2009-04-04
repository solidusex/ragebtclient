
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

#include "../Common/ComnHdr.h"


#define GET_ADDRESS_BY_MEM(address, type, field)\
((type*)((t_byte*)(address) - (t_byte*) (&((type*)0)->field) ) )


#define MAKE_INT64(num64, high, low)					\
		do{												\
		(num64 = ((((t_uint64) high) << 32) | low));	\
		}while(0)

#define SPLIT_INT64(num64, high, low)															\
		do{																						\
				high = (t_uint32)((num64 & LARGE_NUM_SUFFIX(0xFFFFFFFF00000000)) >> 32);		\
				low  = (t_uint32)(num64 & LARGE_NUM_SUFFIX(0x00000000FFFFFFFF));				\
		}while(0)


////////////////////////////// chINRANGE Macro ////////////////////////////////


// This macro returns TRUE if a number is between two others
#define INRANGE(low, Num, High) (((low) <= (Num)) && ((Num) <= (High)))


//////////////////////////////// chDIMOF Macro ////////////////////////////////


// This macro evaluates to the number of elements in an array. 
#define DIMOF(Array) (sizeof(Array) / sizeof(Array[0]))


#define KB (1024)

#define MB  (1024 * KB)

#define GB  ((t_uint64)1024 * (t_uint64)MB)

