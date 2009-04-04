
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

#include "../../Common/ComnHdr.h"
#include "../FastMutex.h"
#include <cassert>
#include <windows.h>
#include <map>
namespace ThreadSpace {


class EventImpl 
{
private:
		HANDLE	m_event_hdl;
public:
		EventImpl(bool is_auto);
		
		~EventImpl();
		
		void Wait();
		
		bool Wait(t_uint64 timeout);
		
		bool TryWait();

		void Set();

		void Reset();
		
};

}












#if(0)
class EventHandle 
{
private:
		HANDLE			m_hdl;
public:
		EventHandle();

		bool Wait(t_ulong timeout);
		
		void Set();

		void ReSet();

		~EventHandle();
};


class ConditionImpl 
{
private:
		
private:
		typedef std::map<DWORD, EventHandle*>	 EventSet;

		typedef	FastMutex		MutexType;
private:
		MutexType				m_mutex;

		EventSet				m_set;
public:
		bool TryWait();

		bool Wait(t_ulong timeout);
		
		
		void Wait();

		void Signal();
		
		void Broadcast();

};

#endif