
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


#include "../TypeDef.h"



namespace FileSysSpace {


class PathImpl
{

public:
		const static char PATH_SP_A = '\\';
		
		const static wchar_t PATH_SP_W = L'\\';
		
		const static t_ulong MAX_PATH_LEN = 1024;
private:
		std::wstring	m_parent;
		std::wstring	m_name;
		std::wstring	m_path;
private:
		PathType		m_type;
private:
		bool is_valid_path(const wchar_t *pattern, const std::wstring &path);
		
		PathType detect_path_type(const std::wstring &path);
		
		void separate_path(const std::wstring &path);

public:
		PathImpl();

		PathImpl(const std::wstring &path);
		
		PathImpl(const std::string &path);

		~PathImpl() { }

public:
		PathImpl(const PathImpl &other);

		PathImpl& operator=(const PathImpl &other);

public:
		bool GetParent(std::wstring &parent)const;
		
		bool GetName(std::wstring &name)const;

		bool GetPath(std::wstring &path) const;

		bool GetParent(std::string &parent)const;
		
		bool GetName(std::string &name)const;

		bool GetPath(std::string &path) const;

		bool Rename(const std::wstring &name);

		bool Rename(const std::string &name);

		void Reset(const std::wstring &path);

		void Reset(const std::string &path);

		bool operator==(const PathImpl &other);
public:
		bool Remove();

		bool Make();

		PathType GetPathType()const; 

		bool IsExisted()const;

public:
		static void GetCurrentPath(std::wstring &path);

		static void GetCurrentPath(std::string &path);

		static bool SetCurrentPath(std::wstring &path);

		static bool SetCurrentPath(std::string &path);
};


		







}