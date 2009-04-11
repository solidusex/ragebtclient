
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

#include "OptexImpl.h"
#include <cassert>

namespace ProcessSpace{

#define EVENT_PREFIX_NAME		"OptexEvent_"
#define MEMFILE_PREFIX_NAME		"OptexMMF_"
#define DEF_SPIN_COUNT			4000

OptexImpl::OptexImpl() : m_name(""), m_psi(new SharedInfo()), m_event(0), m_mem_file(0)
{		
		m_event = ::CreateEvent(0, FALSE, FALSE, 0);
		assert(m_event != NULL);
		memset((void*)m_psi, 0, sizeof(SharedInfo));
		SetSpinCount(DEF_SPIN_COUNT);
}

OptexImpl::OptexImpl(const std::string &name) : m_name(name), m_psi(0), m_event(0), m_mem_file(0)
{
		char buf[MAX_LINE];
		memset((void*)buf, '\0', MAX_LINE);
		sprintf(buf, "%s%s", EVENT_PREFIX_NAME, m_name.c_str());
		m_event = ::CreateEventA(0, FALSE, FALSE, buf);
		assert(m_event != NULL);
		
		memset((void*)buf, '\0', MAX_LINE);
		sprintf(buf, "%s%s", MEMFILE_PREFIX_NAME, m_name.c_str());
		m_mem_file = ::CreateFileMappingA(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(SharedInfo), buf);
		assert(m_mem_file != 0);

		m_psi = (SharedInfo*)::MapViewOfFile(m_mem_file, FILE_MAP_WRITE, 0, 0, 0);
		assert(m_psi != 0);
		
		SetSpinCount(DEF_SPIN_COUNT);
}

OptexImpl::~OptexImpl()
{
#ifdef FOUNDATION_DEBUG

		if(IsSingleProcessOptex() && (m_psi->thread_id != 0)){
				::DebugBreak();
		}

		
		if(!IsSingleProcessOptex() &&  (m_psi->thread_id == ::GetCurrentThreadId())){
				::DebugBreak();
		}

#endif

		::CloseHandle(m_event);
		if(IsSingleProcessOptex())
		{
				delete m_psi;
		}else 
		{
				::UnmapViewOfFile(m_psi);
				::CloseHandle(m_mem_file);
		}
}


void OptexImpl::Enter()
{
		if(TryEnter()) return;

		t_ulong tid = ::GetCurrentThreadId();

		if(::InterlockedIncrement((long*)(&m_psi->lock_count)) == 1)
		{
				m_psi->thread_id = tid;
				m_psi->recurse_count = 1;
		}else 
		{
				if(m_psi->thread_id == tid)
				{
						++m_psi->recurse_count;
				}else {
						::WaitForSingleObject(m_event, INFINITE);
						m_psi->thread_id = tid;
						m_psi->recurse_count = 1;
				}
		}
}


void OptexImpl::SetSpinCount(t_ulong spin_count)
{
		InterlockedExchange((long*)&m_psi->spin_count, spin_count);
}


bool OptexImpl::TryEnter()
{

		t_ulong tid = ::GetCurrentThreadId();
		bool owns_the_optex = false;

		t_ulong s_count = m_psi->spin_count;
		do{
				owns_the_optex = (0 == ::InterlockedCompareExchange((long*)(&m_psi->lock_count), 1, 0));

				if(owns_the_optex)
				{
						m_psi->thread_id = tid;
						m_psi->recurse_count = 1;
				}else 
				{
						if(m_psi->thread_id == tid)
						{
								::InterlockedIncrement((long*)(&m_psi->lock_count));
								++m_psi->recurse_count;
								owns_the_optex = true;
						}
				}
		}while(!owns_the_optex && (s_count-- > 0));

		return owns_the_optex;
}


void OptexImpl::Leave()
{
#ifdef FOUNDATION_DEBUG
		if(m_psi->thread_id != ::GetCurrentThreadId())
		{
				::DebugBreak();
		}

#endif
		
		if(--m_psi->recurse_count > 0)
		{
				::InterlockedDecrement((long*)&m_psi->lock_count);
		}else 
		{
				m_psi->thread_id = 0;
				if(::InterlockedDecrement((long*)&m_psi->lock_count) > 0)
				{
						::SetEvent(m_event);
				}
		}
}

bool OptexImpl::IsSingleProcessOptex()const
{
		return (m_mem_file == 0);
}

}