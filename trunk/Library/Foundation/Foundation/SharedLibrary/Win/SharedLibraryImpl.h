
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
#include "../TypeDef.h"
#include "SharedLibraryImpl.h"
#include <windows.h>
#include <string>

namespace SharedLibrarySpace {





class SharedLibraryImpl
{
private:
		std::wstring			m_path;
		HINSTANCE				m_hdl;
public:
		SharedLibraryImpl(const std::wstring &path);
		
		~SharedLibraryImpl();
public:
		void* GetSymbolAddr(const std::string &name);
		
		std::wstring GetPath();
};




}