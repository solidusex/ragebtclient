
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
#include <memory>
#include "../Utility/UtilityHdr.h"

namespace ThreadSpace {

class MutexImpl;

class  FoundationAPI  Mutex : private NonCopyable
{
private:
		MutexImpl	*m_pimpl;
public:
		class  FoundationAPI ScopeLock : private NonCopyable
		{
		private:
				Mutex	 &m_mutex;
		public:
				ScopeLock(Mutex	 &mutex);
				~ScopeLock();
		};

public:
		Mutex();
		~Mutex();
		void Lock();
		//bool TryLock(t_ulong timeout);
		bool TryLock();

		void UnLock();
};







}