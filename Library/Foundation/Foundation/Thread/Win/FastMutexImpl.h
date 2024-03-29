
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

#include "../TypeDef.h"



#include <windows.h>

namespace ThreadSpace {


class FastMutexImpl 
{
private:
		CRITICAL_SECTION		m_cs;
public:
		FastMutexImpl();
		virtual ~FastMutexImpl();
		void Lock();
		bool TryLock();
		void UnLock();
};





}


