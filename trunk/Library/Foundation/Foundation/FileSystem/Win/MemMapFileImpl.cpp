
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
#include "MemMapFileImpl.h"

namespace FileSysSpace{






bool MemMapFileImpl::Flush(t_byte *addr, t_ulong len)
{
		assert(m_hdl != 0);
		assert(addr != 0);
		assert(len != 0);
		return (::FlushViewOfFile((void*)addr, len) == TRUE);
}

void MemMapFileImpl::UnmapViewOfFile(t_byte* addr)
{
		assert(addr != 0);
		if(::UnmapViewOfFile((const void*)addr) != TRUE)
		{
				throw ExceptionSpace::SystemException("Unmap File view Failed!", ::GetLastError());
		}
}

t_byte* MemMapFileImpl::MapViewOfFile(t_uint64 offset, size_t len, t_ulong mode)
{
		assert(m_hdl != 0);
		assert(len != 0);
		assert(mode != 0);

		DWORD desired = 0;
		switch(mode)
		{
		case MF_MAP_READ_WRITE:
				desired = FILE_MAP_WRITE;
				break;
		case MF_MAP_COPY_ON_WRITE:
				desired = FILE_MAP_COPY;
				break;
		case MF_MAP_READ_ONLY:
				desired = FILE_MAP_READ;
				break;
		default:
				assert(0);
				break;
		}
		t_uint32 hi, lo;
		
		SPLIT_INT64(offset, hi, lo);
		t_byte *addr = (t_byte*)::MapViewOfFile(m_hdl, desired, hi, lo, len);
		return addr;
}

MemMapFileImpl::~MemMapFileImpl()
{
		::CloseHandle(m_hdl);
}

MemMapFileImpl::MemMapFileImpl(void *fhdl, t_uint64 fsize, t_ulong mode)
{
		assert(fhdl != 0);
		assert(fhdl != INVALID_HANDLE_VALUE);
		if(fhdl == INVALID_HANDLE_VALUE)
		{
				throw ExceptionSpace::InvalidArgumentException("File handle invalid!", 0);
		}

		DWORD protect = 0;
		switch(mode)
		{
		case MF_READ_WRITE:
				protect = PAGE_READWRITE;
				break;
		case MF_WRITE_COPY:
				protect = PAGE_WRITECOPY;
				break;
		case MF_READ_ONLY:
				protect = PAGE_READONLY;
				break;
		default:
				assert(0);
				break;
		}

		m_hdl = ::CreateFileMappingW((HANDLE)fhdl, 0, protect, 0, 0, 0);
		assert(m_hdl != 0);
		if(m_hdl == 0)
		{
				throw ExceptionSpace::SystemException("Create Mapping File Failed!", ::GetLastError());
		}

}















}