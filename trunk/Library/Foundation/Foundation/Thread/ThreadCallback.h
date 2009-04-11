
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

namespace ThreadSpace {



class ThreadCallBackAbs : NonCopyable
{
public:
		virtual void Run() = 0;

		virtual ~ThreadCallBackAbs() {   }
};

class ThreadCallBack : public ThreadCallBackAbs
{
private:
		ThreadFunction	 m_func;
		void			 *m_context;
		
public:
		ThreadCallBack(ThreadFunction func, void *context)
		{
				m_func = func;
				m_context = context;
		}
		

		virtual ~ThreadCallBack() {  }

		virtual void Run()
		{
				m_func(m_context);
		}
};

template<class PointerType, class MFunc>
class ThreadCallBackMF : public ThreadCallBackAbs
{
private:
		PointerType		m_ptobj;

		MFunc			m_mf;
public:
		ThreadCallBackMF(const PointerType &ptobj, MFunc mf) : m_ptobj(ptobj), m_mf(mf)
		{

		}

		virtual ~ThreadCallBackMF() { }

		virtual void Run()
		{
				(m_ptobj->*m_mf)();
		}
};


template<class PointerType, class MFun, class ArgType>
class ThreadCallBackMFWithArg : public ThreadCallBackAbs
{
private:
		PointerType				m_ptobj;
		MFun					m_mf;
		ArgType					m_arg;
public:
		ThreadCallBackMFWithArg(const PointerType &ptobj, MFun mf, ArgType arg):
				m_ptobj(ptobj), m_mf(mf), m_arg(arg)
		{

		}

		virtual ~ThreadCallBackMFWithArg() { }

		virtual void Run()
		{
				(m_ptobj->*m_mf)(m_arg);
		}

};

template<class PointerType, class MFun, class ArgType1, class ArgType2>
class ThreadCallBackMFWith2Arg : public ThreadCallBackAbs
{
private:
		PointerType		m_ptobj;
		MFun			m_mf;
		ArgType1		m_arg1;
		ArgType2		m_arg2;

public:
		ThreadCallBackMFWith2Arg(const PointerType &ptobj, MFun mf, ArgType1 arg1, ArgType2 arg2):
				m_ptobj(ptobj), m_mf(mf), m_arg1(arg1), m_arg2(arg2)
		{

		}

		virtual ~ThreadCallBackMFWith2Arg() { }

		virtual void Run()
		{
				(m_ptobj->*m_mf)(m_arg1, m_arg2);
		}

};


template<class PointerType, class MFun, 
		class ArgType1, class ArgType2, class ArgType3
>
class ThreadCallBackMFWith3Arg : public ThreadCallBackAbs
{
private:
		PointerType		m_ptobj;

		MFun			m_mf;

		ArgType1		m_arg1;
		ArgType2		m_arg2;
		ArgType3		m_arg3;

public:
		ThreadCallBackMFWith3Arg(const PointerType &ptobj, MFun mf, 
				ArgType1 arg1, ArgType2 arg2, ArgType3 arg3) 
				:
				m_ptobj(ptobj), m_mf(mf), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3)
		{

		}

		virtual ~ThreadCallBackMFWith3Arg() { }

		virtual void Run()
		{
				(m_ptobj->*m_mf)(m_arg1, m_arg2, m_arg3);
		}

};


template<class PointerType, class MFun, 
		class ArgType1, class ArgType2, class ArgType3, class ArgType4
>
class ThreadCallBackMFWith4Arg : public ThreadCallBackAbs
{
private:
		PointerType		m_ptobj;
		MFun			m_mf;
		ArgType1		m_arg1;
		ArgType2		m_arg2;
		ArgType3		m_arg3;
		ArgType4		m_arg4;

public:
		ThreadCallBackMFWith4Arg(const PointerType &ptobj, MFun mf, 
				ArgType1 arg1, ArgType2 arg2, ArgType3 arg3, ArgType4 arg4) 
				:
				m_ptobj(ptobj), m_mf(mf), 
						m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4)
		{

		}

		virtual ~ThreadCallBackMFWith4Arg() { }

		virtual void Run()
		{
				(m_ptobj->*m_mf)(m_arg1, m_arg2, m_arg3, m_arg4);
		}

};



}


#if(0)


template<class Type, class MFunc>
class ThreadCallBackMF : public ThreadCallBackAbs
{
private:
		Type	*m_ptype;

		MFunc	m_mf;
public:
		ThreadCallBackMF(Type *ptype, MFunc mf) : m_ptype(ptype), m_mf(mf)
		{

		}

		virtual ~ThreadCallBackMF() { }

		virtual void Run()
		{
				(m_ptype->*m_mf)();
		}
};


template<class Type, class MFun, class ArgType>
class ThreadCallBackMFWithArg : public ThreadCallBackAbs
{
private:
		Type			*m_ptype;
		MFun			m_mf;
		ArgType			m_arg;
public:
		ThreadCallBackMFWithArg(Type* ptype, MFun mf, ArgType arg):
				m_ptype(ptype), m_mf(mf), m_arg(arg)
		{

		}

		virtual ~ThreadCallBackMFWithArg() { }

		virtual void Run()
		{
				(m_ptype->*m_mf)(m_arg);
		}

};

template<class Type, class MFun, class ArgType1, class ArgType2>
class ThreadCallBackMFWith2Arg : public ThreadCallBackAbs
{
private:
		Type			*m_ptype;
		MFun			m_mf;
		ArgType1		m_arg1;
		ArgType2		m_arg2;

public:
		ThreadCallBackMFWith2Arg(Type* ptype, MFun mf, ArgType1 arg1, ArgType2 arg2):
				m_ptype(ptype), m_mf(mf), m_arg1(arg1), m_arg2(arg2)
		{

		}

		virtual ~ThreadCallBackMFWith2Arg() { }

		virtual void Run()
		{
				(m_ptype->*m_mf)(m_arg1, m_arg2);
		}

};


template<class Type, class MFun, 
		class ArgType1, class ArgType2, class ArgType3
>
class ThreadCallBackMFWith3Arg : public ThreadCallBackAbs
{
private:
		Type			*m_ptype;

		MFun			m_mf;

		ArgType1		m_arg1;
		ArgType2		m_arg2;
		ArgType3		m_arg3;

public:
		ThreadCallBackMFWith3Arg(Type* ptype, MFun mf, 
				ArgType1 arg1, ArgType2 arg2, ArgType3 arg3) 
				:
				m_ptype(ptype), m_mf(mf), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3)
		{

		}

		virtual ~ThreadCallBackMFWith3Arg() { }

		virtual void Run()
		{
				(m_ptype->*m_mf)(m_arg1, m_arg2, m_arg3);
		}

};


template<class Type, class MFun, 
		class ArgType1, class ArgType2, class ArgType3, class ArgType4
>
class ThreadCallBackMFWith4Arg : public ThreadCallBackAbs
{
private:
		Type			*m_ptype;

		MFun			m_mf;

		ArgType1		m_arg1;
		ArgType2		m_arg2;
		ArgType3		m_arg3;
		ArgType4		m_arg4;

public:
		ThreadCallBackMFWith4Arg(Type* ptype, MFun mf, 
				ArgType1 arg1, ArgType2 arg2, ArgType3 arg3, ArgType4 arg4) 
				:
				m_ptype(ptype), m_mf(mf), 
						m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4)
		{

		}

		virtual ~ThreadCallBackMFWith4Arg() { }

		virtual void Run()
		{
				(m_ptype->*m_mf)(m_arg1, m_arg2, m_arg3, m_arg4);
		}

};


#endif
