
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
#include "NamedEvent.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/NamedEventImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace ProcessSpace {





NamedEvent::NamedEvent(const std::string &name)
{
		m_pimpl = new NamedEventImpl(name);
		assert(m_pimpl);
}

NamedEvent::~NamedEvent()
{
		delete m_pimpl;
}


void NamedEvent::Set()
{
		m_pimpl->Set();
}

void NamedEvent::Wait()
{
		m_pimpl->Wait();
}

std::string NamedEvent::Name()const
{
		return m_pimpl->Name();
}






















}