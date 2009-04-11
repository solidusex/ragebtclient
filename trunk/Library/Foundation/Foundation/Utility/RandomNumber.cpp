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
#include "RandomNumber.h"
#include <cstdlib>

t_uint64 rand64()
{
		t_uint64 res = rand();
		res ^= (t_uint64)rand() << 15;
		res ^= (t_uint64)rand() << 30;
		res ^= (t_uint64)rand() << 45;
		res ^= (t_uint64)rand() << 60;
		return res;
}

t_uint32 rand32()
{
		t_uint32 res = rand();
		res ^= (t_uint32)rand() << 15;
		res ^= (t_uint32)rand() << 30;
		return res;
}

