
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


#include "../SocketSet.h"


namespace NetSpace {






void FdSetIterator::First()
{

		m_curr = 0;

}
		
void FdSetIterator::Next()
{
		m_curr++;
}

bool FdSetIterator::IsDone()const
{
		return (m_curr >= m_set.fd_count);
}

NetHandle FdSetIterator::Current()const
{
		assert(m_curr < m_set.fd_count);
		return m_set.fd_array[m_curr];

}

/////////////////////////////SocketSet/////////////////////////////////

NetHandle SocketSet::MaxSocket()const
{
		return 0;
}













}