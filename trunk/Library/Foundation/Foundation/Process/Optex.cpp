
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
#include "Optex.h"



#if defined(OS_FAMILY_WINDOWS)
		#include "Win/OptexImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace ProcessSpace {

Optex::Optex() : pimpl(new OptexImpl()) { }

Optex::Optex(const std::string &name) : pimpl(new OptexImpl(name)) { }

Optex::~Optex() { delete pimpl; }

void Optex::Enter()
{
		pimpl->Enter();
}

bool Optex::TryEnter()
{
		return pimpl->TryEnter();
}

void Optex::Leave()
{
		pimpl->Leave();
}

bool Optex::IsSingleProcessOptex() const
{
		return pimpl->IsSingleProcessOptex();
}

void Optex::SetSpinCount(t_ulong spin_count)
{
		pimpl->SetSpinCount(spin_count);
}



Optex::ScopeLock::ScopeLock(Optex  &optex) : m_optex(optex)
{
		m_optex.Enter();
}

Optex::ScopeLock::~ScopeLock()
{
		m_optex.Leave();
}


}