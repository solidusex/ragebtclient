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

#include "DirectoryIteratorImpl.h"




namespace FileSysSpace {










DirectoryIteratorImpl::~DirectoryIteratorImpl()
{
		
}

std::wstring DirectoryIteratorImpl::GetPath()const
{
		return m_path;
}

const Path& DirectoryIteratorImpl::Current()const
{
		return m_curr;
}

bool DirectoryIteratorImpl::IsDone()const
{
		return m_is_done;
}

void DirectoryIteratorImpl::set_curr(WIN32_FIND_DATAW	&data)
{

		std::wstring pn = (m_path + data.cFileName);
		
		if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
				pn += L"\\";
		}
		m_curr.Reset(pn);

}


DirectoryIteratorImpl::DirectoryIteratorImpl(const std::wstring &path) 
		: m_is_done(false), m_hdl(INVALID_HANDLE_VALUE)
{
		Path check(path);
		if(check.GetPathType() == PATH_ROOT || check.GetPathType() == PATH_DIR)
		{
				m_path = path;
				
		}else
		{
				throw ExceptionSpace::PathSyntaxException("Path invalid");
		}

}

void DirectoryIteratorImpl::First()
{
		assert(m_hdl == INVALID_HANDLE_VALUE);

		WIN32_FIND_DATAW		data;
		std::wstring tmp = m_path + L"*";
		m_hdl = FindFirstFileW(tmp.c_str(), &data);
		if(m_hdl == INVALID_HANDLE_VALUE)
		{
				throw ExceptionSpace::FileException(" Iterate Directory Failed : DirectoryIteratorImpl::First()", ::GetLastError());
		
		}
				
		set_curr(data);
}



void DirectoryIteratorImpl::Next()
{
		assert(m_hdl != INVALID_HANDLE_VALUE);
		WIN32_FIND_DATAW		data;

		if(::FindNextFileW(m_hdl, &data))
		{
				set_curr(data);
		}else
		{
				m_is_done = true;
				::FindClose(m_hdl);
				m_hdl = INVALID_HANDLE_VALUE;
		}
}


}