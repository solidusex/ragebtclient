
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
#include "SharedLibrary.h"

#if defined(OS_FAMILY_WINDOWS)
		#include "Win/SharedLibraryImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace SharedLibrarySpace {

SharedLibrary::SharedLibrary(const std::string &path)
{
		std::wstring tmp = FormatSpace::AnsiConverter::ToUnicode(path);
		m_pimpl = new SharedLibraryImpl(tmp);
}

SharedLibrary::SharedLibrary(const std::wstring &path)
{
		m_pimpl = new SharedLibraryImpl(path);
}


SharedLibrary::~SharedLibrary()
{
		delete m_pimpl;
}


void* SharedLibrary::GetSymbolAddr(const std::string &name)
{
		return m_pimpl->GetSymbolAddr(name);
}
		
void SharedLibrary::GetPath(std::wstring &path)
{
		path = m_pimpl->GetPath();
}

void SharedLibrary::GetPath(std::string &path)
{
		std::wstring tmp;
		GetPath(tmp);
		path = FormatSpace::UnicodeConverter::ToAnsi(tmp);
}












}