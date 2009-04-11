
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
#ifndef __THREAD_THREAD_H_
#define __THREAD_THREAD_H_

#include "TypeDef.h"
#include "ThreadCallBack.h"




namespace ThreadSpace {

class ThreadImpl;

class  FoundationAPI  Thread  : private NonCopyable
{
private:
		ThreadImpl		*m_pimpl;
public:
		static void Sleep(t_ulong milliseconds);
		
		static void YieldThread();

		static t_ulong GetCurrentThreadId();

		void init(ThreadCallBackAbs *ptcb);
public:
		void* GetThreadHandle()const;
		
		bool Join(t_ulong milliseconds);
		void Join();
		
		void Terminate();

		Priority GetPriority()const;
		
		bool SetPriority(Priority p);
		
		t_ulong Tid()const;

		bool IsActive()const;

		//void Detach();
		
		~Thread();
public:
		Thread(ThreadFunction func, void *context);
		
		template<class PointerType, class MFun>
		Thread(const PointerType &ptobj, MFun mfun)
		{
				assert(ptobj != 0);
				init(new ThreadCallBackMF<PointerType, MFun>(ptobj, mfun));
		}

		template<class PointerType, class MFun, class ArgType>
		Thread(const PointerType &ptobj, MFun mfun, ArgType arg)
		{
				assert(ptobj != 0);
				init(new ThreadCallBackMFWithArg<PointerType, MFun, ArgType>(ptobj, mfun, arg));
		}
		
		template<class PointerType, class MFunc, class ArgType1, class ArgType2>
		Thread(const PointerType &ptobj, MFunc mf, ArgType1 arg1, ArgType2 arg2)
		{
				assert(ptobj != 0);
				init(new ThreadCallBackMFWith2Arg<PointerType, MFunc, ArgType1, ArgType2>(ptobj, mf, arg1,arg2));

		}

		
		template<class PointerType, class MFunc, class ArgType1, class ArgType2, class ArgType3>
		Thread(const PointerType &ptobj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3)
		{
				assert(ptobj != 0);
				init(new ThreadCallBackMFWith3Arg<PointerType, MFunc, ArgType1, ArgType2, ArgType3>
						(ptobj, mf, arg1,arg2, arg3));

		}

		
		template<class PointerType, class MFunc, class ArgType1, class ArgType2, class ArgType3, class ArgType4>
		Thread(const PointerType &ptobj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3, ArgType4 arg4)
		{
				assert(ptobj != 0);
				init(new ThreadCallBackMFWith4Arg<PointerType, MFunc, ArgType1, ArgType2, ArgType3, ArgType4>
						(ptobj, mf, arg1,arg2, arg3, arg4));

		}

		
	



};




}

#endif

#if(0)

template<class Type, class MFun>
		Thread(Type &type, MFun mfun)
		{
				init(new ThreadCallBackMF<Type, MFun>(&type, mfun));
		}

		template<class Type, class MFun, class ArgType>
		Thread(Type *ptype, MFun mfun, ArgType arg)
		{
				assert(ptype != 0);
				init(new ThreadCallBackMFWithArg<Type, MFun, ArgType>(ptype, mfun, arg));
		}
		
		template<class Type, class MFun, class ArgType>
		Thread(Type &type, MFun mfun, ArgType arg)
		{
				init(new ThreadCallBackMFWithArg<Type, MFun, ArgType>(&type, mfun, arg));
		}


		template<class Type, class MFunc, class ArgType1, class ArgType2>
		Thread(Type *pobj, MFunc mf, ArgType1 arg1, ArgType2 arg2)
		{
				assert(pobj != 0);
				init(new ThreadCallBackMFWith2Arg<Type, MFunc, ArgType1, ArgType2>(pobj, mf, arg1,arg2));

		}

		template<class Type, class MFunc, class ArgType1, class ArgType2>
		Thread(Type &obj, MFunc mf, ArgType1 arg1, ArgType2 arg2)
		{
				init(new ThreadCallBackMFWith2Arg<Type, MFunc, ArgType1, ArgType2>(&obj, mf, arg1,arg2));

		}


		template<class Type, class MFunc, class ArgType1, class ArgType2, class ArgType3>
		Thread(Type *pobj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3)
		{
				assert(pobj != 0);
				init(new ThreadCallBackMFWith3Arg<Type, MFunc, ArgType1, ArgType2, ArgType3>
						(pobj, mf, arg1,arg2, arg3));

		}

		template<class Type, class MFunc, class ArgType1, class ArgType2, class ArgType3>
		Thread(Type &obj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3)
		{
				init(new ThreadCallBackMFWith3Arg<Type, MFunc, ArgType1, ArgType2, ArgType3>
						(&obj, mf, arg1,arg2, arg3));

		}

		template<class Type, class MFunc, class ArgType1, class ArgType2, class ArgType3, class ArgType4>
		Thread(Type *pobj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3, ArgType4 arg4)
		{
				assert(pobj != 0);
				init(new ThreadCallBackMFWith4Arg<Type, MFunc, ArgType1, ArgType2, ArgType3, ArgType4>
						(pobj, mf, arg1,arg2, arg3, arg4));

		}

		template<class Type, class MFunc, class ArgType1, class ArgType2, class ArgType3, class ArgType4>
		Thread(Type &obj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3, ArgType4 arg4)
		{
				init(new ThreadCallBackMFWith4Arg<Type, MFunc, ArgType1, ArgType2, ArgType3, ArgType4>
						(&obj, mf, arg1,arg2, arg3, arg4));

		}

#endif

	/*
		template<class Type, class MFun>
		Thread(Type *ptype, MFun mfun, void* param)
		{
				init(new ThreadCallBackMFWithArg<Type, MFun>(ptype, mfun, param));
		}
		
		
		template<class Type, class MFun>
		Thread(Type &type, MFun mfun, void *param)
		{
				init(new ThreadCallBackMFWithArg<Type, MFun>(&type, mfun, param));
		}*/