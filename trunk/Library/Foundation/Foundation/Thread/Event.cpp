
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
#include "Event.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/EventImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace ThreadSpace {

Event::Event(bool is_auto) : m_pimpl(new EventImpl(is_auto)) { }

Event::~Event() { delete m_pimpl; }

void Event::Set()
{
		m_pimpl->Set();
}

void Event::Wait() { m_pimpl->Wait(); }

bool Event::Wait(t_uint64 timeout)
{
		return m_pimpl->Wait(timeout);
}

bool Event::TryWait()
{
		return m_pimpl->TryWait();
}

void Event::Reset()
{
		m_pimpl->Reset();
}

}