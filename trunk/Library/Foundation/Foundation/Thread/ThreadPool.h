
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
#include "ThreadCallback.h"
#include "TSDataQueue.h"
#include "Thread.h"

namespace ThreadSpace {


class  FoundationAPI  ThreadPool : NonCopyable
{
private:
		TSDataQueue				m_queue;
		std::vector<Thread*>	m_thread_set;
		bool					m_is_started;
		size_t					m_thd_num;
private:
		void execute(ThreadCallBackAbs *ptcb);

		void worker_thread();
public:
		bool Start(size_t thd_num = 3);

		void Stop();
public:
		void IncreaseThread(size_t n);

		void DecreaseThread(size_t n);
		
		size_t ThreadCount()const;

		bool IsStarted()const;
public:
		ThreadPool();
		
		~ThreadPool();

public:
		void Execute(ThreadFunction func, void *param);

		template<class PointerType, class MFunc>
		void Execute(const PointerType &pobj, MFunc mf)
		{
				execute(new ThreadCallBackMF<PointerType, MFunc>(pobj, mf));
		}

		template<class PointerType, class MFunc, class ArgType>
		void Execute(const PointerType &pobj, MFunc mf, ArgType arg)
		{
				execute(new ThreadCallBackMFWithArg<PointerType, MFunc, ArgType>(pobj, mf, arg));
		}

		template<class PointerType, class MFunc, class ArgType1, class ArgType2>
		void Execute(const PointerType &pobj, MFunc mf, ArgType1 arg1, ArgType2 arg2)
		{
				execute(new ThreadCallBackMFWith2Arg<PointerType, MFunc, ArgType1, ArgType2>(pobj, mf, arg1,arg2));

		}

		template<class PointerType, class MFunc, class ArgType1, class ArgType2, class ArgType3>
		void Execute(const PointerType &pobj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3)
		{
				execute(new ThreadCallBackMFWith3Arg<PointerType, MFunc, ArgType1, ArgType2, ArgType3>
						(pobj, mf, arg1,arg2, arg3));

		}

		template<class PointerType, class MFunc, class ArgType1, class ArgType2, class ArgType3, class ArgType4>
		void Execute(const PointerType &pobj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3, ArgType4 arg4)
		{
				execute(new ThreadCallBackMFWith4Arg<PointerType, MFunc, ArgType1, ArgType2, ArgType3, ArgType4>
						(pobj, mf, arg1,arg2, arg3, arg4));

		}
};



		
}

#if(0)
		template<class Type, class MFunc>
		void Execute(Type *pobj, MFunc mf)
		{
				execute(new ThreadCallBackMF<Type, MFunc>(pobj, mf));
		}

		template<class Type, class MFunc>
		void Execute(Type &obj, MFunc mf)
		{
				execute(new ThreadCallBackMF<Type, MFunc>(&obj, mf));
		}

		template<class Type, class MFunc, class ArgType>
		void Execute(Type *pobj, MFunc mf, ArgType arg)
		{
				execute(new ThreadCallBackMFWithArg<Type, MFunc, ArgType>(pobj, mf, arg));
		}

		template<class Type, class MFunc, class ArgType>
		void Execute(Type &obj, MFunc mf, ArgType arg)
		{
				execute(new ThreadCallBackMFWithArg<Type, MFunc, ArgType>(&obj, mf, arg));
		}
		
		template<class Type, class MFunc, class ArgType1, class ArgType2>
		void Execute(Type *pobj, MFunc mf, ArgType1 arg1, ArgType2 arg2)
		{
				execute(new ThreadCallBackMFWith2Arg<Type, MFunc, ArgType1, ArgType2>(pobj, mf, arg1,arg2));

		}

		template<class Type, class MFunc, class ArgType1, class ArgType2>
		void Execute(Type &obj, MFunc mf, ArgType1 arg1, ArgType2 arg2)
		{
				execute(new ThreadCallBackMFWith2Arg<Type, MFunc, ArgType1, ArgType2>(&obj, mf, arg1,arg2));

		}


		template<class Type, class MFunc, class ArgType1, class ArgType2, class ArgType3>
		void Execute(Type *pobj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3)
		{
				execute(new ThreadCallBackMFWith3Arg<Type, MFunc, ArgType1, ArgType2, ArgType3>
						(pobj, mf, arg1,arg2, arg3));

		}

		template<class Type, class MFunc, class ArgType1, class ArgType2, class ArgType3>
		void Execute(Type &obj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3)
		{
				execute(new ThreadCallBackMFWith3Arg<Type, MFunc, ArgType1, ArgType2, ArgType3>
						(&obj, mf, arg1,arg2, arg3));

		}

		template<class Type, class MFunc, class ArgType1, class ArgType2, class ArgType3, class ArgType4>
		void Execute(Type *pobj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3, ArgType4 arg4)
		{
				execute(new ThreadCallBackMFWith4Arg<Type, MFunc, ArgType1, ArgType2, ArgType3, ArgType4>
						(pobj, mf, arg1,arg2, arg3, arg4));

		}

		template<class Type, class MFunc, class ArgType1, class ArgType2, class ArgType3, class ArgType4>
		void Execute(Type &obj, MFunc mf, ArgType1 arg1, ArgType2 arg2, ArgType3 arg3, ArgType4 arg4)
		{
				execute(new ThreadCallBackMFWith4Arg<Type, MFunc, ArgType1, ArgType2, ArgType3, ArgType4>
						(&obj, mf, arg1,arg2, arg3, arg4));

		}

#endif