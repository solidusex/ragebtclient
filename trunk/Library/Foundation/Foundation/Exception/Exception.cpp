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

#include "Exception.h"


namespace ExceptionSpace{

Exception::Exception()
{

}

Exception::Exception(const std::string& msg) 
		: m_msg(msg), m_code(0)
{


}

Exception::Exception(const std::string& msg, t_ulong code) 
		: m_msg(msg), m_code(code)
{

}


Exception::Exception(const Exception &other)
{
		*this = other;
}

Exception::~Exception() throw()
{


}

Exception& Exception::operator=(const Exception& other)
{

		if(this != &other)
		{
				std::exception::operator=(other);
				m_msg = other.m_msg;
				m_code = other.m_code;
		}

		return *this;

}

const char* Exception::ClassName() const throw()
{
		return typeid(*this).name();

}

const char* Exception::what() const throw()
{
		return m_msg.c_str();
}


Exception* Exception::Clone() const
{
		return (new Exception(*this));
}
		
void Exception::ReThrow() const
{

		throw *this;
}

const std::string& Exception::Message() const
{
		return m_msg;
}

t_ulong Exception::Code()const
{
		return m_code;
}


#if(0)


LogicException::LogicException(const std::string &msg, t_long code)
{
		m_err_msg.msg = msg;
		m_err_msg.code = code;

}
		
LogicException::~LogicException()
{


}



RuntimeException::RuntimeException(const std::string &msg, t_long code)
{
		m_err_msg.msg = msg;
		m_err_msg.code = code;
}
		
RuntimeException::~RuntimeException()
{


}




#endif














}

