
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


namespace ProcessSpace {


class ProcessHandleImpl;

class ProcessImpl;

class Process;

class FoundationAPI ProcessHandle
{
private:
		ProcessHandleImpl		*m_pimpl;
private:
		ProcessHandle(ProcessHandleImpl *impl);

		friend class Process;

public:
		PID	Id()const;
		
		t_int32 Wait();
		
		ProcessHandle(const ProcessHandle &other);

		ProcessHandle& operator= (const ProcessHandle &other);

		~ProcessHandle();

		t_ulong GetPriority()const;

		bool SetPriority(t_ulong priority)const;

};


class  FoundationAPI Process
{
public:
		static ProcessHandle Launch(const std::wstring &path, const std::wstring &command, ProcessStartInfo &si);
		
		static bool Kill(PID pid);

		static void RequestTermination(PID pid);


};


}