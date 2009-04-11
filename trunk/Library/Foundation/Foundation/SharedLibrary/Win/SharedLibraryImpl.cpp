
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

#include "SharedLibraryImpl.h"


namespace SharedLibrarySpace {



void* SharedLibraryImpl::GetSymbolAddr(const std::string &name)
{
		return (void*)::GetProcAddress(m_hdl, name.c_str());
}

std::wstring SharedLibraryImpl::GetPath()
{
		return m_path;
}

SharedLibraryImpl::~SharedLibraryImpl()
{
		::FreeLibrary(m_hdl);
}

SharedLibraryImpl::SharedLibraryImpl(const std::wstring &path)
{
		m_path = path;
		m_hdl = ::LoadLibraryW(path.c_str());
		if(m_hdl == NULL)
		{
				std::string tmp = FormatSpace::UnicodeConverter::ToAnsi(path);
				throw ExceptionSpace::SystemException("Load Library " + tmp + " Failed!", ::GetLastError());
		}
		
}



}