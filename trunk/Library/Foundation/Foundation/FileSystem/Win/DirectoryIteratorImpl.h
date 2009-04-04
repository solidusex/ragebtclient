

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
#include "../Path.h"

#define _WINSOCKAPI_
#include <windows.h>

namespace FileSysSpace {



class DirectoryIteratorImpl 
{
private:
		HANDLE					m_hdl;
		
		std::wstring			m_path;
		
		Path					m_curr;

		bool					m_is_done;
private:
		void set_curr(WIN32_FIND_DATAW		&data);
public:
		DirectoryIteratorImpl(const std::wstring &path);

		~DirectoryIteratorImpl();

public:
		std::wstring GetPath()const;

		void Next();

		bool IsDone()const;

		const Path& Current()const;

		void First();

		//bool Reset(const std::wstring &path);

		
};





}