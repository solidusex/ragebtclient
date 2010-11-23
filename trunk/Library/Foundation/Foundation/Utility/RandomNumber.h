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
#pragma once

#include "../Common/ComnHdr.h"
#include <ctime>



FoundationAPI t_uint64 rand64();

FoundationAPI t_uint32 rand32();


template<class T = int>
class RandomNumber
{
private:
		size_t m_range;

public:
		RandomNumber(size_t range = 256) : m_range(range)
		{
				srand(time(0));
		}

		T operator()()
		{
				return (T)(rand64()%m_range);
		}

};

