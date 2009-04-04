
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

#include <string>
#include <vector>

namespace FormatSpace {
//unicode为本机的采用的unicode格式
class  FoundationAPI  AnsiConverter : private NonCopyable
{
public:
		static std::wstring		 ToUnicode(const std::string &sour);
		static std::string		 ToUTF8(const std::string &sour);
};


class  FoundationAPI  UnicodeConverter : private NonCopyable
{
public:
		static std::string		ToAnsi(const std::wstring &sour);
		static std::string		ToUTF8(const std::wstring &sour);
};



class  FoundationAPI  UTF8Converter : private NonCopyable
{
public:
		static std::string ToAnsi(const std::string &sour);
		static std::wstring ToUnicode(const std::string &sour);
};













}