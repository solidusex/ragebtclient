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

#ifndef __COMMON_DEFINE_H_
#define __COMMON_DEFINE_H_

#pragma once

#if defined(_WIN32)

#define MAX_LONG				0x7fffffff
#define MAX_SHORT				0x7fff
#define MAX_LINE				4096


//如果是gunc编译器，则会变成val##LLU,注意，这玩意只能是常量的替换而已，不能整个变量放上来
#define LARGE_NUM_SUFFIX(val)		(val)

#endif

#endif





