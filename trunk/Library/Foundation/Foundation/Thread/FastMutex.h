
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
#include "../Utility/UtilityHdr.h"
#include <memory>

namespace ThreadSpace {

class FastMutexImpl;
//此锁必须支持递归互斥，支持线程挂起语意
class  FoundationAPI  FastMutex : private NonCopyable
{
private:
		FastMutexImpl	*m_pimpl;
public:
		class  FoundationAPI ScopeLock : private NonCopyable
		{
		private:
				FastMutex	 &m_mutex;
		public:
				ScopeLock(FastMutex	 &mutex);
				~ScopeLock();
		};
public:
		FastMutex();
		~FastMutex();
		void Lock();
		bool TryLock();
		void UnLock();
};

}