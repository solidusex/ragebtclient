
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

#include "Path.h"
#include <queue>
#include <cassert>

namespace FileSysSpace {




class FoundationAPI TreeIterator : private NonCopyable
{
private:
		typedef std::queue<Path>		PathQueue;
private:
		PathQueue						m_queue;
		Path							m_root;
		Path							m_curr;
		bool							m_ignore_error;
private:
		void clear()
		{
				m_curr.Reset(L"");
				while(!m_queue.empty())
				{
						m_queue.pop();
				}
		}
public:
		TreeIterator(const std::wstring &path, bool ignore_err = true);

		TreeIterator(const std::string &path, bool ignore_err = true);
		

		~TreeIterator()
		{
				
		}
public:
		void RootPath(std::wstring &path)const;

		void GetPath(std::string &path) const;

		void First();//¿ÉÄÜthrow FileSysException

		void Next();

		bool IsDone()const;

		const Path& Current()const;

		bool Reset(const std::wstring &path, bool ignore_err = true);

		bool Reset(const std::string &path, bool ignore_err = true);
};



















}