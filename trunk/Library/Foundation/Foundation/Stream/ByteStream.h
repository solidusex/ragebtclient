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


#include "AbstractStream.h"


namespace StreamSpace {



typedef AbstractStreamBuffer<t_byte> AbstractByteStreamBuffer;

typedef AbstractInputStream<t_byte>   AbstractByteInputStream;

typedef AbstractOutputStream<t_byte>  AbstractByteOutputStream;



/////////////////////////////ByteArrayOutputStream//////////////////////////////


class FoundationAPI ByteOutputStream : public AbstractByteOutputStream, private NonCopyable
{
private:
		AbstractByteStreamBuffer	*m_pbuffer;
public:
		ByteOutputStream(AbstractByteStreamBuffer* pbuf);

		virtual ~ByteOutputStream();
public:
		virtual void Flush();
       
		virtual void Write(const t_byte &b);

		virtual void Write(const t_byte  *arr, size_t n);

		virtual AbstractByteStreamBuffer* Buffer(AbstractByteStreamBuffer* pbuf);

		virtual AbstractByteStreamBuffer* Buffer();

};



/////////////////////////////ByteArrayInputStream//////////////////////////////




class FoundationAPI ByteInputStream : public AbstractByteInputStream, private NonCopyable
{
private:
		AbstractByteStreamBuffer	*m_pbuffer;
public:
		ByteInputStream(AbstractByteStreamBuffer *pbuf);
		
		virtual ~ByteInputStream();

public:
		virtual t_uint64 Available()const;
		
		virtual void Read(t_byte &b);

		virtual size_t Read(t_byte *arr, size_t size);
		
		virtual size_t Skip(size_t n);

		virtual AbstractByteStreamBuffer* Buffer(AbstractByteStreamBuffer* pbuf);

		virtual AbstractByteStreamBuffer* Buffer();

};


}









#if(0)
class ByteArrayStreamBuffer : public AbstractByteStreamBuffer, private virtual NonCopyable
{
private:
		std::vector<t_byte>		m_buf;
		t_long					m_head;
		t_long					m_tail;
private:
		void increase_capability(t_ulong len);
public:
		ByteArrayStreamBuffer();

		virtual ~ByteArrayStreamBuffer();
public:
		virtual const t_byte* GetContent()const;

		virtual void Put(const t_byte &b);		//增加到buffer中
		
		virtual void Get(t_byte &b);			//从buf中取出，之后buf不再存在
		
		virtual void Put(const t_byte *arr, t_ulong len);

		virtual t_ulong Get(t_byte *arr, t_ulong n);

		virtual void Skip(t_ulong n);
		
		virtual t_ulong  Available()const ;

		bool IsEmpty()const;

		virtual void Clear();
};

#endif