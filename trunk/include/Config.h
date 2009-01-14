/*
 * The Rage Library
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
 */#pragma once


#if defined(_MSC_VER)


#if defined(RAGE_SHARED_LIB)
		#if defined(RAGE_EXPORTS)
				#define RageAPI __declspec(dllexport)
		#else
				#define RageAPI __declspec(dllimport)
		#endif
#else
		#define RageAPI
#endif


#if defined(RAGE_SHARED_LIB)
		#define FOUNDATION_SHARED_LIB
#endif

#if defined(FOUNDATION_NDEBUG)
		#define RAGE_NDEBUG
#else
		#define RAGE_DEBUG
#endif

#include "Foundation.h"
#pragma comment(lib, "Foundation.lib")

#if defined(min)
#undef min

#endif


#pragma warning(disable : 4267)
#pragma warning(disable : 4505)
#pragma warning(disable : 4310)

#endif