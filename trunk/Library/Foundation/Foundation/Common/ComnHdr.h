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

#ifndef __COMMON_COMNHDR_H_
#define __COMMON_COMNHDR_H_

#pragma once

#include "Platform.h"


//如果要编译成dll则需要在preprocessor中加入FOUNDATION_EXPORTS和FOUNDATION_SHARED_LIB

//被编译成dll之后，在引用此DLL的工程中要加入#define FOUNDATION_SHARED_LIB


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/Config.h"
		#include "Win/Type.h"
		#include "Win/Define.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif


#endif