
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
#include <windows.h>
#include <string>
#include <memory>

namespace ProcessSpace {


class ProcessImpl;


class ProcessHandleImpl : public RefCountedObject
{
private:
		HANDLE			m_hdl;
private:
		ProcessHandleImpl();

		virtual ~ProcessHandleImpl();

		friend class ProcessImpl;
public:
		PID	Id()const;
		
		t_int32 Wait();

		ProcessHandleImpl(HANDLE hdl);
				
		t_ulong GetPriority()const;

		bool SetPriority(t_ulong priority)const;

};



class ProcessImpl
{
public:
		static ProcessHandleImpl* Launch(const std::wstring &path, const std::wstring &command, ProcessStartInfo &si);
		
		static bool Kill(PID pid);

		static void RequestTermination(PID pid);


};





}