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




#if defined(_MSC_VER)

		#if defined(NDEBUG) && defined(_DEBUG)
				#error Inconsistent build settings (check for /MD[d])
		#endif

		#if !defined(_WIN32_WINNT)
				#define _WIN32_WINNT 0x0501
		#endif

		#if defined(NDEBUG)
				#define FOUNDATION_NDEBUG
		#else
				#define FOUNDATION_DEBUG
		#endif

		#if defined(FOUNDATION_SHARED_LIB)
				#if defined(FOUNDATION_EXPORTS)
						#define FoundationAPI __declspec(dllexport)
				#else
						#define FoundationAPI __declspec(dllimport)	
				#endif
		#else
				#define FoundationAPI
		#endif
		
		#define F_STDCALL __stdcall

		#define F_THISCALL __thiscall

		#define F_CCALL __cdecl 
		
		#define	F_NAKED __declspec(naked)


		#if !defined(_MT)
				#error Must compile with /MD, /MDd, /MT or /MTd
		#endif

		//#pragma warning(disable:4018) // signed/unsigned comparison
		#pragma warning(disable:4251) // ... needs to have dll-interface warning 
		#pragma warning(disable:4355) // 'this' : used in base member initializer list
		#pragma warning(disable:4996) // VC++ 8.0 deprecation warnings
		#pragma warning(disable:4002)
		#pragma warning(disable:4800)
		#pragma warning(disable : 4101)
		#pragma warning(disable : 4516)
		#pragma warning(disable : 4127)

#else

		#error "Unknow Compiler!"

#endif
