
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
#pragma once

#include "TypeDef.h"
#include "Path.h"


namespace FileSysSpace {


class DirectoryIteratorImpl;


class FoundationAPI DirectoryIterator : private NonCopyable
{
private:
		DirectoryIteratorImpl			*m_pimpl;

public:
		DirectoryIterator(const std::wstring &path);

		DirectoryIterator(const std::string &path);

		~DirectoryIterator();

public:
		void GetPath(std::wstring &path)const;

		void GetPath(std::string &path) const;

		void First();

		void Next();

		bool IsDone()const;

		const Path& Current()const;

		void Reset(const std::wstring &path);

		void Reset(const std::string &path);
};











}