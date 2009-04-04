
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

#include "TypeDef.h"


namespace NetSpace {


enum EventMask
{
		INPUT_MASK	= BIT_MARK(0),
		OUTPUT_MASK = BIT_MARK(1),
		EXPT_MASK   = BIT_MARK(2),
		TIMER_MASK = BIT_MARK(3),
};

#define ALL_EVENT_MASK (INPUT_MASK|OUTPUT_MASK|EXPT_MASK|TIMER_MASK)

class Selector;

class FoundationAPI NetEventHandler : public ::RefCountedObject
{
private:
		typedef ThreadSpace::SpinLock	MutexType;
		typedef MutexType::ScopeLock	LockType;
private:
		friend class Selector;
private:
		volatile t_ulong	m_mask;
		Selector	*m_selector;
public:
		t_ulong Mask()const//得到mask;
		{
				return m_mask;
		}

		void Mask(t_ulong mask) //设置mask;
		{
				m_mask = (mask & ALL_EVENT_MASK);
		}

		void AddMask(t_ulong mask)
		{
				t_ulong add_mask = (mask & ALL_EVENT_MASK);
				m_mask |= add_mask;

		}

		void ClrMask(t_ulong mask)
		{
				t_ulong clr_mask = (mask & ALL_EVENT_MASK);
				m_mask &= ~clr_mask;
		}

		bool IsMask(t_ulong mask)const
		{
				return ((m_mask & mask) != 0);
		}
public:
		Selector* GetSelector()const
		{
				return m_selector;
		}
public:
		virtual NetHandle GetHandle()const = 0
		{
				return NET_INVALID_SOCKET;
		}
		
		virtual void OnInput() { }

		virtual void OnOutput() { }

		virtual void OnException() { }

		virtual void OnTimer() { }
public:
		NetEventHandler() : m_mask(0), m_selector(0)
		{

		}

		//任何在此析构函数中涉及到GetSelector()的操作都会挂掉
		virtual ~NetEventHandler()
		{

		}

};





}