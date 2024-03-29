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

namespace ProcessSpace {


class NamedEventImpl;

class FoundationAPI NamedEvent : private NonCopyable
{
private:
		NamedEventImpl			*m_pimpl;
public:
		NamedEvent(const std::string& name);

		~NamedEvent();

		void Set();

		void Wait();

		std::string Name()const;
};




















}