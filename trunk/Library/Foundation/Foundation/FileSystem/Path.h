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


namespace FileSysSpace {

class PathImpl;
		
class  FoundationAPI  Path				//注意，Path类只根据传入的字符串最后是否有
{										//'\'（win32)or '/'(linux)来判定是否为dir

private:
		PathImpl		*m_pimpl;

public:
		const static char PATH_SP_A;
		
		const static wchar_t PATH_SP_W;
		
		const static t_ulong MAX_PATH_LEN;
public:
		Path();

		Path(const std::wstring &path);

		Path(const std::string &path);

		~Path();
public:
		Path(const Path &other);

		Path& operator= (const Path &other);
public:
		PathType GetPathType()const;

		bool IsExisted()const;
public:
		bool GetParent(std::wstring &parent)const;
		
		bool GetParent(std::string &parent)const;

		bool GetPath(std::wstring &path) const;

		bool GetPath(std::string &path) const;

		bool GetName(std::string &name)const;

		bool GetName(std::wstring &name)const;

		bool Rename(const std::string &name);

		bool Rename(const std::wstring &name);

		bool Make();

		bool Remove();
		
		void Reset(const std::string &path);

		void Reset(const std::wstring &path);

		bool operator==(const Path &other);

public:
		static void GetCurrentPath(std::wstring &path);

		static void GetCurrentPath(std::string &path);

		static bool SetCurrentPath(std::wstring &path);

		static bool SetCurrentPath(std::string &path);
};







}