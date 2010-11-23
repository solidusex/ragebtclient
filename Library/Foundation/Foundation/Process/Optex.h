
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
#include <string>

namespace ProcessSpace {

class OptexImpl;

class  FoundationAPI  Optex : private NonCopyable		//OPTEX是一种自己实现的
{														//最优化的跨进程mutex
private:
		OptexImpl		*pimpl;
public:
		class  FoundationAPI ScopeLock : private NonCopyable
		{
		private:
				Optex	 &m_optex;
		public:
				ScopeLock(Optex	 &optex);
				~ScopeLock();
		}; 
public:
		Optex();
		Optex(const std::string &name);
		~Optex();
		void Enter();
		bool TryEnter();
		void Leave();
		bool IsSingleProcessOptex()const;
		void SetSpinCount(t_ulong spin_count);
};




}

