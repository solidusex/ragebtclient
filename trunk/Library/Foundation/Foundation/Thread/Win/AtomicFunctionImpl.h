
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

#include "../AtomicFunction.h"
#include<windows.h>
#include<cassert>

namespace ThreadSpace {

namespace {

inline t_ulong GetProcessorCount()
{
		::SYSTEM_INFO sys_info;
		::GetSystemInfo(&sys_info);
		assert(sys_info.dwNumberOfProcessors != 0);
		return sys_info.dwNumberOfProcessors;
}

static const t_ulong ProcessorCount = GetProcessorCount();

}

#if(ARCH == ARCH_IA32)

#pragma warning(push)
#pragma warning(disable : 4100)

void F_STDCALL AtomicFunction::Add(volatile size_t *pnum, size_t val)
{
		if(ProcessorCount == 1)
		{
				__asm
				{
						mov eax, [esp + 8];
						mov ecx, [esp + 4];
						add dword ptr[ecx], eax;
				}
		}else
		{
				__asm
				{
						mov eax, [esp + 8];
						mov ecx, [esp + 4];
						lock add dword ptr[ecx], eax;
				}
		}

}


void F_STDCALL AtomicFunction::Sub(volatile size_t *pnum, size_t val)
{
		if(ProcessorCount == 1)
		{
				__asm
				{
						mov eax, [esp + 8];
						mov ecx, [esp + 4];
						sub dword ptr[ecx], eax;
				}
		}else
		{
				__asm
				{
						mov eax, [esp + 8];
						mov ecx, [esp + 4];
						lock sub dword ptr[ecx], eax;
				}
		}

}
		

void F_STDCALL AtomicFunction::Increment(volatile size_t *pnum)
{
		if(ProcessorCount == 1)
		{
				__asm
				{
						mov ecx, [esp + 4];
						add dword ptr[ecx], 1;
				}
		}else
		{
				__asm 
				{
						mov ecx, [esp + 4];
						lock add dword ptr[eax], 1;
				}
		}
}


void F_STDCALL AtomicFunction::Decrement(volatile size_t *pnum)
{
		if(ProcessorCount == 1)
		{
				__asm
				{
						mov ecx, [esp + 4];
						add dword ptr [ecx], -1;
				}
		}else
		{
				__asm 
				{
						mov ecx, [esp + 4];
						lock add dword ptr[ecx], -1;
				}
		}

}

void F_STDCALL AtomicFunction::Exchange(volatile size_t *pnum, size_t val)
{
		if(ProcessorCount == 1)
		{
				__asm
				{
						mov ecx, [esp + 8];
						mov edx, [esp + 4];
						xchg [edx], ecx;
				}
		}else
		{
				__asm 
				{
						mov ecx, [esp + 8];
						mov edx, [esp + 4];
						lock xchg [edx], ecx;
				}
		}
}


size_t F_STDCALL AtomicFunction::CompareExchange(volatile size_t *dest, size_t val, size_t cmpval)
{
		if(ProcessorCount == 1)
		{
				__asm{
						mov eax, [esp + 12];//cmpval;
						mov ecx, [esp + 8];//val;
						mov edx, [esp + 4];//dest;
						cmpxchg [edx], ecx;
						ret 12;			//eax作为返回值;
				}
		}else
		{
				__asm{
						mov eax, [esp + 12];//cmpval;
						mov ecx, [esp + 8];//val;
						mov edx, [esp + 4];//dest;
						lock cmpxchg [edx], ecx;
						ret 12;			//eax作为返回值;
				}
		}
}

#pragma warning(pop)

#else

#error NOT_SUPPORT_PLATFORM

#endif

}
