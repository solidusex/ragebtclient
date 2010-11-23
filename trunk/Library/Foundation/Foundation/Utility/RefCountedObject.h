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

#include "../Common/ComnHdr.h"
#include "NonCopyable.h"


class FoundationAPI RefCountedObject : private NonCopyable
{
public:
		RefCountedObject();
		
		bool Duplicate()const throw();
		
		void Release()const throw();

		size_t Count()const;
protected:
		virtual ~RefCountedObject();
private:
		volatile mutable size_t		m_rc;
};


