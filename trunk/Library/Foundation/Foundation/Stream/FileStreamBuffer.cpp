
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
#include "FileStreamBuffer.h"





namespace StreamSpace {

DefaultFileOPImpl::DefaultFileOPImpl()
{

}

DefaultFileOPImpl:: ~DefaultFileOPImpl()
{

}

void DefaultFileOPImpl::Flush()
{
		m_file.Flush();
}

bool DefaultFileOPImpl::Open(const std::string &fname)
{		using namespace FileSysSpace;
return m_file.Open(fname.c_str(), F_READ_WRITE|F_OPEN_ALWAYS|F_NOSHARED);

}

bool DefaultFileOPImpl::Open(const std::wstring &fname)
{
		using namespace FileSysSpace;
		return m_file.Open(fname.c_str(), F_READ_WRITE|F_OPEN_ALWAYS|F_NOSHARED);
}

void DefaultFileOPImpl::Close()
{
		m_file.Close();
}


void DefaultFileOPImpl::Write(const t_byte *arr, size_t len)
{
		m_file.Write(arr, len);
}

size_t DefaultFileOPImpl::Read(t_byte *arr, size_t len)
{
		return m_file.Read(arr, len);
}

t_uint64 DefaultFileOPImpl::Seek(const t_uint64 &offset, SEEK_POS method, bool forward)
{
		switch(method)
		{
		case StreamSpace::F_CUR:
				return m_file.Seek(offset, FileSysSpace::CURR, forward);
		case StreamSpace::F_END:
				return m_file.Seek(offset, FileSysSpace::END, forward);
		default:
				return m_file.Seek(offset, FileSysSpace::BEG, forward);
		}

}

t_uint64 DefaultFileOPImpl::Available()const
{
		return (GetLength() - GetPos());
}


t_uint64 DefaultFileOPImpl::GetLength()const
{
		return m_file.GetLength();
}

t_uint64 DefaultFileOPImpl::GetPos()const
{
		return m_file.GetPos();
}

}