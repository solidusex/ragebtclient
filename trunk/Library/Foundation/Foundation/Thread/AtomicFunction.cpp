
/*
 * The Foundation Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. It is provided "as is" without express 
 * or implied warranty.
 */
#include "AtomicFunction.h"




#if defined(OS_FAMILY_WINDOWS)
		#include "Win/AtomicFunctionImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif














