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

#include "DirectoryIterator.h"
#include "../Format/FormatHdr.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/DirectoryIteratorImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace FileSysSpace {

void DirectoryIterator::First()
{
		m_pimpl->First();
}

DirectoryIterator::DirectoryIterator(const std::wstring &path) : m_pimpl(0)
{
		m_pimpl = new DirectoryIteratorImpl(path);
}

DirectoryIterator::DirectoryIterator(const std::string &path)
{
		std::wstring wstr;
		m_pimpl = new DirectoryIteratorImpl(
				FormatSpace::AnsiConverter::ToUnicode(path)
				);

}

DirectoryIterator::~DirectoryIterator()
{
		delete m_pimpl;
}


void DirectoryIterator::GetPath(std::wstring &path)const
{
		path = m_pimpl->GetPath();
}

void DirectoryIterator::GetPath(std::string &path) const
{
		path = FormatSpace::UnicodeConverter::ToAnsi(m_pimpl->GetPath());
}

void DirectoryIterator::Next()
{
		m_pimpl->Next();
}

bool DirectoryIterator::IsDone()const
{
		return m_pimpl->IsDone();
}

const Path& DirectoryIterator::Current()const
{
		return m_pimpl->Current();
}

void DirectoryIterator::Reset(const std::wstring &path)
{
		assert(m_pimpl != 0);
		delete m_pimpl;
		m_pimpl = new DirectoryIteratorImpl(path);
}

void DirectoryIterator::Reset(const std::string &path)
{
		Reset(FormatSpace::AnsiConverter::ToUnicode(path));
}



}


