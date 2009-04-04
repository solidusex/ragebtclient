
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

#include "TypeDef.h"


namespace ThreadSpace {

class FoundationAPI AtomicFunction
{
public:
		static void F_STDCALL Increment(volatile size_t *pnum);

		static void F_STDCALL Decrement(volatile size_t *pnum);
		
		static void F_STDCALL Exchange(volatile size_t *pnum, size_t val);

		static size_t F_STDCALL CompareExchange(volatile size_t *dest, size_t val, size_t cmpval);

		static void F_STDCALL Add(volatile size_t *pnum, size_t val);

		static void F_STDCALL Sub(volatile size_t *pnum, size_t val);

};


}