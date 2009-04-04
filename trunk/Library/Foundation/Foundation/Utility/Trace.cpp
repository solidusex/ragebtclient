
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
#include "Trace.h"
#include <cstdio>

//typedef void (*TraceOutputFunc)(const char *pstr);



void F_STDCALL DefTraceFunc(const char *pstr)
{
		printf("%s", pstr);
}


static TraceOutputFunc g_trace_func = DefTraceFunc;


TraceOutputFunc SetTraceFunc(TraceOutputFunc func)
{

		TraceOutputFunc res = g_trace_func;

		if(func != 0)
		{
				g_trace_func = func;
		}

		return res;
}



void TracePrint(const char *pstr)
{
		if(pstr != 0)
		{
				g_trace_func(pstr);
		}

}