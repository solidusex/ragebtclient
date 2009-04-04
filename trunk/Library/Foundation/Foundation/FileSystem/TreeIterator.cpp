
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
#include "TreeIterator.h"

#include "../Format/FormatHdr.h"
#include "DirectoryIterator.h"




namespace FileSysSpace {


bool TreeIterator::Reset(const std::wstring &path, bool ignore_err)
{
		Path new_path(path);
		if(new_path.GetPathType() == INVALID_PATH)
		{
				return false;
		}else
		{
				m_root = new_path;//Reset(path);
				m_ignore_error = ignore_err;
				clear();
				return true;
		}
}

bool TreeIterator::Reset(const std::string &path, bool ignore_err)
{
		std::wstring wstr = FormatSpace::AnsiConverter::ToUnicode(path);
		return Reset(wstr, ignore_err);
}

void TreeIterator::RootPath(std::wstring &path)const
{
		if(!m_root.GetPath(path))
		{
				path = L"";
		}
}

TreeIterator::TreeIterator(const std::string &path, bool ignore_err)
{
		clear();
		if(!Reset(path, ignore_err))
		{
				throw ExceptionSpace::PathSyntaxException(
						"Invalid path name : TreeIterator::TreeIterator(const std::wstring &path)"
						);
		}
}

TreeIterator::TreeIterator(const std::wstring &path, bool ignore_err)
{
		clear();
		if(!Reset(path, ignore_err))
		{
				throw ExceptionSpace::PathSyntaxException(
						"Invalid path name : TreeIterator::TreeIterator(const std::wstring &path)"
						);
		}
}



void TreeIterator::GetPath(std::string &path) const
{
		std::wstring wpath;
		bool res = m_root.GetPath(wpath);
		assert(res);

		path = FormatSpace::UnicodeConverter::ToAnsi(wpath);
}


bool TreeIterator::IsDone()const
{
		std::string name;
		m_curr.GetName(name);
		return (name == "");
}


const Path& TreeIterator::Current()const
{
		return m_curr;
		
}


void TreeIterator::First()
{
		clear();
		PathType pt = m_root.GetPathType();
		assert(pt != INVALID_PATH);
		
		
		if(pt == PATH_DIR || pt == PATH_ROOT)
		{
				std::wstring path;
				bool res = m_root.GetPath(path);
				assert(res);
				DirectoryIterator dir_iter(path);

				std::string name;
				for(dir_iter.First(); !dir_iter.IsDone(); dir_iter.Next())
				{
						name.clear();
						const Path &ref_path = dir_iter.Current();
						bool res = ref_path.GetName(name);
						assert(res);
						if(name != "." && name != "..")
						{
								m_queue.push(ref_path);
						}
				}
		}else
		{
				m_queue.push(m_root);
		}

		if(!m_queue.empty())
		{
				m_curr = m_queue.front();
				m_queue.pop();
		}else
		{
				m_curr.Reset("");
		}
}

void TreeIterator::Next()
{
		if(m_curr.GetPathType() == PATH_ROOT || m_curr.GetPathType() == PATH_DIR)
		{
				std::wstring path;
				bool res = m_curr.GetPath(path);
				assert(res);

				try{
						DirectoryIterator dir_iter(path);
						std::string name;
						for(dir_iter.First(); !dir_iter.IsDone(); dir_iter.Next())
						{
								name.clear();
								const Path &ref_path = dir_iter.Current();
								res = ref_path.GetName(name);
								assert(res);
								
								if((name != ".") && (name != ".."))
								{
										m_queue.push(ref_path);
								}
						}
				}catch(const ExceptionSpace::FileException &expt)
				{
						DEBUG_PRINT1("UnKnow exception == %s\n", expt.what());
						if(!m_ignore_error)
						{
								throw;
						}
				}
		}
		if(!m_queue.empty())
		{
				m_curr = m_queue.front();
				m_queue.pop();
		}else
		{
				m_curr.Reset("");
		}
}




}