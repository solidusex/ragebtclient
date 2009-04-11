
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

#include "../TypeDef.h"

#include <windows.h>
#include <string>

namespace ProcessSpace {




class OptexImpl : private NonCopyable
{
private:

		struct SharedInfo
		{
				volatile t_long			lock_count;
				volatile t_uint32		thread_id;
				volatile t_long			recurse_count;
				volatile t_ulong		spin_count;
		};
private:
		const std::string		m_name;
		SharedInfo				*m_psi;
		HANDLE					m_event;
		HANDLE					m_mem_file;
public:
		OptexImpl();
		OptexImpl(const std::string &name);
		~OptexImpl();
		void Enter();
		bool TryEnter();
		void Leave();
		bool IsSingleProcessOptex()const;
		void SetSpinCount(t_ulong spin_count);
};



}