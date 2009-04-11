
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


namespace FormatSpace {

class FoundationAPI NumberFormatter
{
public:
		static std::string Format(t_int32 value);


		static std::string Format(t_int32 value, int width);//返回指定宽度的字符串


		static std::string Format0(t_int32 value, int width);//用0填补缺少的,按十进制


		static std::string FormatHex(t_int32 value);//返回按16进制


		static std::string FormatHex(t_int32 value, int width);


		static std::string Format(t_uint32 value);


		static std::string Format(t_uint32 value, int width);


		static std::string Format0(t_uint32  value, int width);


		static std::string FormatHex(t_uint32 value);


		static std::string FormatHex(t_uint32 value, int width);


		static std::string Format(t_long value);


		static std::string Format(t_long value, int width);


		static std::string Format0(t_long value, int width);


		static std::string FormatHex(t_long value);


		static std::string FormatHex(t_long value, int width);


		static std::string Format(t_ulong value);


		static std::string Format(t_ulong value, int width);


		static std::string Format0(t_ulong value, int width);


		static std::string FormatHex(t_ulong value);


		static std::string FormatHex(t_ulong value, int width);




		static std::string Format(t_int64 value);


		static std::string Format(t_int64 value, int width);


		static std::string Format0(t_int64 value, int width);


		static std::string FormatHex(t_int64 value);


		static std::string FormatHex(t_int64 value, int width);


		static std::string Format(t_uint64 value);


		static std::string Format(t_uint64 value, int width);


		static std::string Format0(t_uint64 value, int width);


		static std::string FormatHex(t_uint64 value);


		static std::string FormatHex(t_uint64 value, int width);




		static std::string Format(float value);


		static std::string Format(double value);

		static std::string Format(double value, int precision);


		static std::string Format(double value, int width, int precision);


		static std::string Format(const void* ptr);

};



}