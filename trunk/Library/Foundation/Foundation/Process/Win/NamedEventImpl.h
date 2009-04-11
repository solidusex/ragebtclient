
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

#include <windows.h>
#include <string>


namespace ProcessSpace {

class NamedEventImpl
{
private:
		HANDLE			m_hdl;
		std::string		m_name;

public:
		NamedEventImpl(const std::string& name);

		~NamedEventImpl();

		void Set();

		void Wait();

		std::string Name()const;
};




}