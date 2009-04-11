
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


#include "ByteStream.h"
#include <vector>

namespace StreamSpace {


class FoundationAPI DefaultMemBufferImpl
{
private:
		DSSpace::SimpleBuffer	m_buffer;
public:
		DefaultMemBufferImpl();
		
		~DefaultMemBufferImpl();
public:
		const t_byte* GetContent()const;
		
		void Put(const t_byte *arr, size_t len);

		size_t Get(t_byte *arr, size_t n);

		size_t Skip(size_t n);
		
		t_uint64  Available()const ;

		void Clear();

		bool IsEmpty()const;
};


/*********************************MemStreamBuffer ******************/

template<class BufferType = DefaultMemBufferImpl>
class MemStreamBuffer : public AbstractByteStreamBuffer, private NonCopyable
{
private:
		BufferType		m_buf;
public:
		MemStreamBuffer()
		{

		}

		
		virtual ~MemStreamBuffer()
		{

		}
public:
		virtual const t_byte* GetContent()const
		{
				return m_buf.GetContent();
		}

		virtual void Put(const t_byte &b)
		{
				m_buf.Put(&b, 1);
		}
		
		virtual void Get(t_byte &b)
		{
				m_buf.Get(&b, 1);
		}
		
		virtual void Put(const t_byte *arr, size_t len)
		{
				m_buf.Put(arr, len);
		}

		virtual size_t Get(t_byte *arr, size_t n)
		{
				return m_buf.Get(arr, n);
		}

		virtual size_t Skip(size_t n)
		{
				return m_buf.Skip(n);
		}

		
		virtual t_uint64  Available()const 
		{
				return m_buf.Available();
		}

		virtual void Clear()
		{
				m_buf.Clear();
		}
};


typedef MemStreamBuffer<DefaultMemBufferImpl>  DefaultMemStreamBuffer;

}

