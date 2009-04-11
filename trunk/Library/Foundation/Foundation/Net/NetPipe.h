
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
#include "SockStream.h"



namespace NetSpace {



class FoundationAPI NetPipe : private NonCopyable
{
public:
		SockStream		m_reader;
		SockStream		m_writer;
public:
		
		NetPipe();

		~NetPipe();
		
		bool Open (size_t buf_size = 0*KB);

		bool IsOpen()const;

		void Close();

		NetHandle ReaderHandle()const;

		NetHandle WriterHandle()const;

		int Available();
		
		//都是阻塞操作，如果返回失败，则管道不可再用
		bool Read(t_byte* buf, size_t len);

		bool Write(const t_byte* buf, size_t len);
		
};

















}

