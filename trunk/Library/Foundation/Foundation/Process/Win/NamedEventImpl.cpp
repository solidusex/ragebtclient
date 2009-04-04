
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
#include "NamedEventImpl.h"


#include "../TypeDef.h"


namespace ProcessSpace {




std::string NamedEventImpl::Name()const
{
		return m_name;
}


void NamedEventImpl::Set()
{
		if(!::SetEvent(m_hdl))
		{
				throw ExceptionSpace::SystemException("Can't Set Event", ::GetLastError());
		}
}

void NamedEventImpl::Wait()
{
		switch (WaitForSingleObject(m_hdl, INFINITE))
		{
		case WAIT_OBJECT_0:
				return;
		default:
				throw ExceptionSpace::SystemException("Wait for Event Failed!", ::GetLastError());
		}
}


NamedEventImpl::~NamedEventImpl()
{
		::CloseHandle(m_hdl);
}


NamedEventImpl::NamedEventImpl(const std::string& name) : m_name(name)
{
		m_hdl = ::CreateEventA(0, FALSE,  FALSE, m_name.c_str());
		assert(m_hdl);
		
		if(m_hdl == 0)
		{
				throw ExceptionSpace::SystemException("Wait for Event Failed!", ::GetLastError());
		}
}




























}