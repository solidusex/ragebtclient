
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
#include "MemStreamBuffer.h"



namespace StreamSpace {

#if defined(OS_FAMILY_WINDOWS)

#pragma warning(disable : 4244)

#endif



const t_byte* DefaultMemBufferImpl::GetContent()const
{
		return const_cast<DefaultMemBufferImpl*>(this)->m_buffer.Data();
}


DefaultMemBufferImpl::~DefaultMemBufferImpl()
{

}

void DefaultMemBufferImpl::Clear()
{
		m_buffer.Clear();
}

t_uint64 DefaultMemBufferImpl::Available()const
{
		return m_buffer.Size();
}

bool DefaultMemBufferImpl::IsEmpty()const
{
		return m_buffer.IsEmpty();
		
}

DefaultMemBufferImpl::DefaultMemBufferImpl()
{

}


void DefaultMemBufferImpl::Put(const t_byte *arr, size_t len)
{
		assert(arr != 0 && len != 0);
		m_buffer.Insert(arr, len);
}

size_t DefaultMemBufferImpl::Get(t_byte *arr, size_t n)
{
		assert(arr != 0 && n != 0);
		assert(n <= m_buffer.Size());
		if(n > m_buffer.Size())
		{
				n = m_buffer.Size();
		}

		memcpy(arr,m_buffer.Data(), n);
		m_buffer.Erase(n);
		return n;
}

size_t DefaultMemBufferImpl::Skip(size_t n)
{
		if(n > m_buffer.Size())
		{
				n = m_buffer.Size();
		}

		m_buffer.Erase(n);
		return n;
}


}

