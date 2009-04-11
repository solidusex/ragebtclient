
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

#include "TypeDef.h"

namespace TimeSpace {


class FoundationAPI CycleTimer : NonCopyable
{
private:
		t_uint64 m_startcycle;
		t_uint64 m_overhead;
public:
		CycleTimer();

		~CycleTimer();
		

		void Start();

		t_uint64 Stop();

		t_uint64 GetOverHead()const;
public:
		static t_uint64 GetCycleCount();

};










}