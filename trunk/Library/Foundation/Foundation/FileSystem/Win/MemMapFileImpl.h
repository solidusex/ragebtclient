
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

#include "../TypeDef.h"
#include <windows.h>

namespace FileSysSpace{


#define MEM_BLOCK_SIZE (65536)

class MemMapFileImpl
{
private:
		HANDLE			m_hdl;
public:
		MemMapFileImpl(void *fhdl, t_uint64 fsize, t_ulong mode);

		~MemMapFileImpl();
		
		t_byte* MapViewOfFile(t_uint64 offset, size_t len, t_ulong mode);//±ØÐë±ß½ç¶ÔÆë

		void UnmapViewOfFile(t_byte* addr);

		bool Flush(t_byte *addr, t_ulong len);
};


}