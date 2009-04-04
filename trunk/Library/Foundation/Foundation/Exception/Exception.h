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

#include "../Common/ComnHdr.h"
#include <stdexcept>
#include <typeinfo>
namespace  ExceptionSpace {


class FoundationAPI Exception: public std::exception
{
private:
		std::string		m_msg;
		t_ulong			m_code;
public:
		Exception();

		Exception(const std::string& msg);

		Exception(const std::string& msg, t_ulong code);
		
		Exception(const Exception &other);

		virtual ~Exception() throw();

		Exception& operator= (const Exception& other);
		
		virtual const char* ClassName() const throw();

		virtual const char* what() const throw();

		virtual Exception* Clone() const;
		
		virtual void ReThrow() const;

		const std::string& Message() const;

		t_ulong Code()const;
};












}