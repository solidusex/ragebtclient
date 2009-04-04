
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
#include "Path.h"

#if defined(OS_FAMILY_WINDOWS)
		#include "Win/PathImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif


namespace FileSysSpace {


const char Path::PATH_SP_A = PathImpl::PATH_SP_A;
		
const wchar_t Path::PATH_SP_W = PathImpl::PATH_SP_W;

const t_ulong Path::MAX_PATH_LEN =  PathImpl::MAX_PATH_LEN;



Path::Path(const Path &other) : m_pimpl(0)
{
		m_pimpl = new PathImpl();
		*this = other;
}

Path& Path::operator= (const Path &other)
{
		if(this != &other)
		{
				*m_pimpl = *(other.m_pimpl);
		}
		return *this;
}
						



Path::Path(const std::wstring &path)
{
		m_pimpl = new PathImpl(path);
}

Path::~Path()
{
		delete m_pimpl;
}


void Path::Reset(const std::wstring &path)
{
		m_pimpl->Reset(path);
}


void Path::Reset(const std::string &path)
{		
		m_pimpl->Reset(path);
}


bool Path::operator==(const Path &other)
{
		return *m_pimpl == *(other.m_pimpl);
}

		
PathType Path::GetPathType()const
{
		return 	m_pimpl->GetPathType();
}

bool Path::IsExisted()const
{
		return m_pimpl->IsExisted();
}


bool Path::GetParent(std::wstring &parent)const
{
		return m_pimpl->GetParent(parent);
}

		
bool Path::GetName(std::wstring &name)const
{
		return m_pimpl->GetName(name);
}

bool Path::GetPath(std::wstring &path) const
{
		return m_pimpl->GetPath(path);
}


bool Path::Make()
{
		return m_pimpl->Make();
}


bool Path::Remove()
{
		return m_pimpl->Remove();
}
		
bool Path::Rename(const std::wstring &name)
{
		return m_pimpl->Rename(name);
}

Path::Path(const std::string &path)
{
		m_pimpl = new PathImpl(path);
}


bool Path::GetParent(std::string &parent)const
{
		return m_pimpl->GetParent(parent);
}

		
bool Path::GetName(std::string &name)const
{
		return m_pimpl->GetName(name);
}


bool Path::GetPath(std::string &path) const
{
		return m_pimpl->GetPath(path);
}


bool Path::Rename(const std::string &name)
{
		return m_pimpl->Rename(name);
}





Path::Path()
{
		m_pimpl = new PathImpl();
}



void Path::GetCurrentPath(std::wstring &path)
{
		PathImpl::GetCurrentPath(path);
}

void Path::GetCurrentPath(std::string &path)
{
		PathImpl::GetCurrentPath(path);
}



bool Path::SetCurrentPath(std::wstring &path)
{
		return PathImpl::SetCurrentPath(path);
}

bool SetCurrentPath(std::string &path)
{
		return PathImpl::SetCurrentPath(path);
}



}