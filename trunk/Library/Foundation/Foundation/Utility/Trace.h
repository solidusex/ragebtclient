
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


typedef void (F_STDCALL *TraceOutputFunc)(const char *pstr);


//如果传入的fun == NULL 则不做改动,直接返回当前func的值
//此函数不可在main之前调用

FoundationAPI TraceOutputFunc SetTraceFunc(TraceOutputFunc func);


FoundationAPI void TracePrint(const char *pstr);

