
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


#include "../../Common/ComnHdr.h"
#include <windows.h>



namespace ThreadSpace{


class MutexImpl
{
private:
		HANDLE			m_mutex_hdl;
public:
		MutexImpl();
		~MutexImpl();
		void Lock();
		//bool TryLock(t_ulong timeout);
		bool TryLock();
		void UnLock();
};




}