
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
#include "NumberFormatter.h"

#include <cstdio>
#include <cctype>
#include <cassert>

#if defined(OS_FAMILY_WINDOWS)

#define I64_FMT "I64"

#endif

namespace FormatSpace {


std::string NumberFormatter::Format(t_int32 value)
{
		char buf[64];
		sprintf(buf, "%d", value);
		return std::string(buf);
}


std::string NumberFormatter::Format(t_int32 value, int width)
{
		assert((width > 0) && (width < 64));
		char buffer[64];
		sprintf(buffer, "%*d", width, value);
		return std::string(buffer);
}

std::string NumberFormatter::Format0(t_int32 value, int width)
{
		assert((width > 0) && (width < 64));
		char buffer[64];
		sprintf(buffer, "%0*d", width, value);
		return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_int32 value)
{
		char buffer[64];
		sprintf(buffer, "%X", value);
		return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_int32 value, int width)
{
		assert((width > 0) && (width < 64));
		char buffer[64];
		sprintf(buffer, "%0*X", width, value);
		return std::string(buffer);
}



std::string NumberFormatter::Format(t_uint32 value)
{
		char buffer[64];
		sprintf(buffer, "%u", value);
		return std::string(buffer);
}


std::string NumberFormatter::Format(t_uint32 value, int width)
{
		char buffer[64];
		sprintf(buffer, "%*u", width, value);
		return std::string(buffer);
}


std::string NumberFormatter::Format0(t_uint32 value, int width)
{
		char buffer[64];
		sprintf(buffer, "%0*u", width, value);
		return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_uint32 value)
{
		char buffer[64];
		sprintf(buffer, "%X", value);
		return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_uint32 value, int width)
{

		char buffer[64];
		sprintf(buffer, "%0*X", width, value);
		return std::string(buffer);
}


std::string NumberFormatter::Format(t_long value)
{
		char buffer[64];
		sprintf(buffer, "%ld", value);
		return std::string(buffer);
}


std::string NumberFormatter::Format(t_long value, int width)
{


		char buffer[64];
		sprintf(buffer, "%*ld", width, value);
		return std::string(buffer);
}


std::string NumberFormatter::Format0(t_long value, int width)
{


		char buffer[64];
		sprintf(buffer, "%0*ld", width, value);
		return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_long value)
{
		char buffer[64];
		sprintf(buffer, "%lX", value);
		return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_long value, int width)
{


		char buffer[64];
		sprintf(buffer, "%0*lX", width, value);
		return std::string(buffer);
}


std::string NumberFormatter::Format(t_ulong value)
{
		char buffer[64];
		sprintf(buffer, "%lu", value);
		return std::string(buffer);
}


std::string NumberFormatter::Format(t_ulong value, int width)
{

		char buffer[64];
		sprintf(buffer, "%*lu", width, value);
		return std::string(buffer);
}


std::string NumberFormatter::Format0(t_ulong value, int width)
{

		char buffer[64];
		sprintf(buffer, "%0*lu", width, value);
		return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_ulong value)
{
		char buffer[64];
		sprintf(buffer, "%lX", value);
		return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_ulong value, int width)
{
		char buffer[64];
		sprintf(buffer, "%0*lX", width, value);
		return std::string(buffer);
}



std::string NumberFormatter::Format(t_int64 value)
{
		char buffer[64];
		sprintf(buffer, "%"I64_FMT"d", value);
		return std::string(buffer);
}


std::string NumberFormatter::Format(t_int64 value, int width)
{


	char buffer[64];
	sprintf(buffer, "%*"I64_FMT"d", width, value);
	return std::string(buffer);
}


std::string NumberFormatter::Format0(t_int64 value, int width)
{
	

	char buffer[64];
	sprintf(buffer, "%0*"I64_FMT"d", width, value);
	return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_int64 value)
{
	char buffer[64];
	sprintf(buffer, "%"I64_FMT"X", value);
	return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_int64 value, int width)
{

	char buffer[64];
	sprintf(buffer, "%0*"I64_FMT"X", width, value);
	return std::string(buffer);
}


std::string NumberFormatter::Format(t_uint64 value)
{
	char buffer[64];
	sprintf(buffer, "%"I64_FMT"u", value);
	return std::string(buffer);
}


std::string NumberFormatter::Format(t_uint64 value, int width)
{
	

	char buffer[64];
	sprintf(buffer, "%*"I64_FMT"u", width, value);
	return std::string(buffer);
}


std::string NumberFormatter::Format0(t_uint64 value, int width)
{

	char buffer[64];
	sprintf(buffer, "%0*"I64_FMT"u", width, value);
	return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_uint64 value)
{
	char buffer[64];
	sprintf(buffer, "%"I64_FMT"X", value);
	return std::string(buffer);
}


std::string NumberFormatter::FormatHex(t_uint64 value, int width)
{

	char buffer[64];
	sprintf(buffer, "%0*"I64_FMT"X", width, value);
	return std::string(buffer);
}




std::string NumberFormatter::Format(float value)
{
	char buffer[64];
	sprintf(buffer, "%.*g", 10, (double) value);
	return std::string(buffer);
}


std::string NumberFormatter::Format(double value)
{
	char buffer[64];
	sprintf(buffer, "%.*g", 20, value);
	return std::string(buffer);
}


std::string NumberFormatter::Format(double value, int precision)
{


	char buffer[64];
	sprintf(buffer, "%.*f", precision, value);
	return std::string(buffer);
}


std::string NumberFormatter::Format(double value, int width, int precision)
{


	char buffer[64];
	sprintf(buffer, "%*.*f", width, precision, value);
	return std::string(buffer);
}

//#if defined(FOUNDATION_PALTFORM_IS_64BIT)
//		sprintf(buffer, "%016"I64_FMT"X", (t_uint64) ptr);
//#else
//#endif

std::string NumberFormatter::Format(const void* ptr)
{
		char buffer[24];
		sprintf(buffer, "%p", ptr);
		return std::string(buffer);
}



}