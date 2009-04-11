
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


#include "TypeDef.h"




namespace SharedLibrarySpace {

class SharedLibraryImpl;


class SharedLibrary : NonCopyable
{
private:
		SharedLibraryImpl		*m_pimpl;
public:
		SharedLibrary(const std::string &path);

		SharedLibrary(const std::wstring &path);

		~SharedLibrary();

public:
		void* GetSymbolAddr(const std::string &name);
		
		void GetPath(std::wstring &path);

		void GetPath(std::string &path);
};




}