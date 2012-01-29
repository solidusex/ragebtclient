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

#ifndef __COMMON_TYPE_H__
#define __COMMON_TYPE_H__

#pragma once

#if defined(_WIN32)

		typedef char					t_char;
		typedef unsigned char			t_uchar;

		typedef wchar_t					t_wchar;

		typedef unsigned short			t_uint16;
		typedef short					t_int16;
		typedef unsigned int			t_uint32;
		typedef int						t_int32;
	
		typedef unsigned long			t_ulong;
		typedef long					t_long;
		
		typedef unsigned __int64		t_uint64;
		typedef __int64					t_int64;
		typedef unsigned __int8			t_uint8;
		typedef __int8					t_int8;

		typedef t_uint8					t_byte;

		//typedef void*					t_ptr;
		
		
		//typedef t_uint16				t_word;
		//typedef t_ulong				t_dword;
		
#else



#endif



#endif  //__COMMON_TYPE_H__

