
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
#include "NumberParser.h"

#include <cstdio>
#include <cctype>

namespace FormatSpace {

#if defined(OS_FAMILY_WINDOWS)
	#define I64_FMT "I64"
#endif

#define ThrowException(message) throw ExceptionSpace::InvalidArgumentException(message)
		
		

t_int32 NumberParser::ParseInt32(const std::string& s)
{
		t_int32 res = 0;
		if(TryParseInt32(s, res))
		{
				return res;
		}else
		{
				ThrowException("invalid input str") ;
		}
}
		
bool NumberParser::TryParseInt32(const std::string& s, t_int32 &value)
{
		return (::sscanf(s.c_str(), "%d", &value) == 1);
}

t_uint32 NumberParser::ParseUInt32(const std::string& s)
{
		t_uint32 res = 0;
		if(TryParseUInt32(s, res))
		{
				return res;
		}else
		{
				ThrowException("invalid input str");
		}
}

bool NumberParser::TryParseUInt32(const std::string& s, t_uint32 &value)
{
		return (::sscanf(s.c_str(), "%u", &value) == 1);
}



t_uint32 NumberParser::ParseHex32(const std::string& s)
{
		t_uint32 res = 0;
		if(TryParseHex32(s, res))
		{
				return res;
		}else
		{
				ThrowException("invalid input str");
		}

}

bool NumberParser::TryParseHex32(const std::string& s, t_uint32 &value)
{
		return (sscanf(s.c_str(), "%x", &value) == 1);
}

t_int64 NumberParser::ParseInt64(const std::string& s)
{
		t_int64 res = 0;
		if(TryParseInt64(s, res))
		{
				return res;
		}else
		{
				ThrowException("invalid input format");
		}
}

bool NumberParser::TryParseInt64(const std::string& s, t_int64 &value)
{
		return (sscanf(s.c_str(), "%"I64_FMT"d", &value) == 1);
}


t_uint64 NumberParser::ParseUInt64(const std::string& s)
{
		t_uint64 res = 0;
		if(TryParseUInt64(s, res))
		{
				return res;
		}else
		{
				ThrowException("invalid input format");
		}
}


bool NumberParser::TryParseUInt64(const std::string& s, t_uint64& value)
{
		return (sscanf(s.c_str(), "%"I64_FMT"u", &value) == 1);
}

t_uint64 NumberParser::ParseHex64(const std::string& s)
{
		t_uint64 res = 0;
		if(TryParseHex64(s, res))
		{
				return res;
		}else
		{
				ThrowException("invalid input format");
		}
}

bool NumberParser::TryParseHex64(const std::string& s, t_uint64 &value)
{
		return (sscanf(s.c_str(), "%"I64_FMT"x", &value) == 1);
}

double NumberParser::ParseFloat(const std::string& s)
{
		double res = 0.0;
		if(TryParseFloat(s, res))
		{
				return res;
		}else
		{
				ThrowException("invalid input format");
		}
}

bool NumberParser::TryParseFloat(const std::string& s, double& value)
{
		return (sscanf(s.c_str(), "%lf", &value) == 1);
}



}
