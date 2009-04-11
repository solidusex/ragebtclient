
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

#include <exception>
#include <cassert>
#include <string>

template<typename Type, typename BaseType>
Type* TypeCast(BaseType *pbase)
{
		Type *ptype = dynamic_cast<Type*>(pbase);
		if(ptype == 0)
		{
				std::string err_str = "invalid typecast : ";
				err_str += typeid(BaseType).name();
				err_str += " to ";
				err_str += typeid(Type).name();
				throw std::runtime_error(err_str);
		}else
		{
				return ptype;
		}

}