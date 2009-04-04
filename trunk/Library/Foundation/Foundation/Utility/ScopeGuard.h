
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

#include <cassert>
#include <memory>
#include "Constrain.h"



class ScopeGuardImp : private NonCopyable
{
public:
		ScopeGuardImp() { };
		virtual ~ScopeGuardImp(){ }
};



template<class Type, class Fore, class Aft>					
class ScopeGuardMF : public ScopeGuardImp
{ 
private:
		typedef  void( Type::*MFun)(void);
private:
		Type	*m_type;
		Fore	m_fore;
		Aft		m_aft;
public:
		ScopeGuardMF(Type *ptype, Fore fore, Aft aft)
		{
				assert(ptype != NULL);
				m_type = ptype;
				m_fore = fore;
				m_aft = aft;
				(m_type->*m_fore)();
		}

		~ScopeGuardMF()
		{
				(m_type->*m_aft)();
		}
};


template<class Fore, class Aft>
class ScopeGuardFun : public ScopeGuardImp
{ 
private:
		Fore	m_fore;
		Aft		m_aft;
public:
		ScopeGuardFun(Fore fore, Aft aft)
		{
				m_fore = fore;
				m_aft = aft;
				m_fore();
		}
		
		~ScopeGuardFun()
		{
				m_aft();
		}

};


class ScopeGuard  : private NonCopyable
{
private:
		std::auto_ptr<ScopeGuardImp>   m_pimp;
public:
		template<typename Type, typename Fore, typename Aft>
		ScopeGuard(Type *ptype, Fore fore, Aft aft)
		{
				m_pimp.reset(new ScopeGuardMF<Type, Fore, Aft>(ptype, fore, aft));
		}
		
		template<typename Fore, typename Aft>
		ScopeGuard(Fore fore, Aft aft)
		{
				m_pimp.reset(new ScopeGuardFun<Fore, Aft>(fore, aft));
		}

		~ScopeGuard() { }

};


#define SCOPE_GUARD(pobj, fore, aft)\
		ScopeGuard guard_tmp(pobj, fore, aft)

#define SCOPE_GUARD2(pobj, fore, aft, name)\
		ScopeGuard name##tmp(pobj, fore, aft)

