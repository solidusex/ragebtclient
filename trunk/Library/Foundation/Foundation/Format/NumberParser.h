
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


namespace FormatSpace {

class FoundationAPI NumberParser
{
public:
		static t_int32 ParseInt32(const std::string& s);
		
		static bool TryParseInt32(const std::string& s, t_int32& value);

		static t_uint32 ParseUInt32(const std::string& s);

		static bool TryParseUInt32(const std::string& s, t_uint32& value);

		
		static t_uint32 ParseHex32(const std::string& s);

		static bool TryParseHex32(const std::string& s, t_uint32& value);
		
		static t_int64 ParseInt64(const std::string& s);
		
		static bool TryParseInt64(const std::string& s, t_int64& value);

		static t_uint64 ParseUInt64(const std::string& s);

		static bool TryParseUInt64(const std::string& s, t_uint64& value);

		static t_uint64 ParseHex64(const std::string& s);
		
		static bool TryParseHex64(const std::string& s, t_uint64 &value);



		static double ParseFloat(const std::string& s);
		
		static bool TryParseFloat(const std::string& s, double& value);
};


}