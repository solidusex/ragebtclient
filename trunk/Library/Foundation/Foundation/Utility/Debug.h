
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
#include "Trace.h"
#include <cstdio>

#if defined(FOUNDATION_NDEBUG)

/*#define DEBUG_PRINT0(arg)					
#define DEBUG_PRINT1(arg, a1)				
#define DEBUG_PRINT2(arg, a1, a2)			
#define DEBUG_PRINT3(arg, a1, a2, a3)		
#define DEBUG_PRINT4(arg, a1, a2, a3, a4)	
#define DEBUG_PRINT5(arg, a1, a2, a3, a4, a5)*/


#define DEBUG_PRINT0(str)						
#define DEBUG_PRINT1(fmt, s1)					
#define DEBUG_PRINT2(fmt, s1, s2)				
#define DEBUG_PRINT3(fmt, s1, s2, s3)			
#define DEBUG_PRINT4(fmt, s1, s2, s3, s4)		
#define DEBUG_PRINT5(fmt, s1, s2, s3, s4, s5)	



#else
/*
#define DEBUG_PRINT0(arg)						printf((arg))
#define DEBUG_PRINT1(arg, a1)					printf((arg), (a1))
#define DEBUG_PRINT2(arg, a1, a2)				printf((arg), (a1), (a2))
#define DEBUG_PRINT3(arg, a1, a2, a3)			printf((arg), (a1), (a2), (a3))
#define DEBUG_PRINT4(arg, a1, a2, a3, a4)		printf((arg), (a1), (a2), (a3), (a4))
#define DEBUG_PRINT5(arg, a1, a2, a3, a4, a5)	printf((arg), (a1), (a2), (a3), (a4), (a5))		*/

#define DEBUG_PRINT0(str)						do{ TracePrint(str); }while(0)
#define DEBUG_PRINT1(fmt, s1)					do{ char buf[1024]; ::sprintf(buf, fmt, s1); TracePrint(buf);}while(0)
#define DEBUG_PRINT2(fmt, s1, s2)				do{ char buf[1024]; ::sprintf(buf, fmt, s1, s2); TracePrint(buf);}while(0)
#define DEBUG_PRINT3(fmt, s1, s2, s3)			do{ char buf[1024]; ::sprintf(buf, fmt, s1, s2, s3); TracePrint(buf);}while(0)
#define DEBUG_PRINT4(fmt, s1, s2, s3, s4)		do{ char buf[1024]; ::sprintf(buf, fmt, s1, s2, s3, s4); TracePrint(buf);}while(0)
#define DEBUG_PRINT5(fmt, s1, s2, s3, s4, s5)	do{ char buf[1024]; ::sprintf(buf, fmt, s1, s2, s3, s4, s5); TracePrint(buf);}while(0)


#endif



#if defined(FOUNDATION_NDEBUG)

#define STATIC_ASSERT(exp) ((void)(exp))

#else

#define STATIC_ASSERT(exp)		\
		do{						\
				int x[exp];		\
		}while(0)				
								


#endif

template<class Expr, class ExptType>
void AssertEx(Expr expr, ExptType &expt)
{
		if(!expr)
		{
				assert(false);
				throw expt;
		}
}


#if defined(FOUNDATION_NDEBUG)

#define FOR_DEBUG(code)

#else

#define FOR_DEBUG(code)

#endif