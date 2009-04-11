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
#include "SpinLock.h"

namespace ThreadSpace {

template<class T>
class Singleton
{
private:
		typedef SpinLock	MutexType;
private:
		Singleton(const Singleton&);
		Singleton& operator=(const Singleton&);
private:
		inline static volatile T*& get_singleton_pobj()
		{
				static volatile T  *pt = 0;
				return pt;
		}

		inline static typename MutexType& get_singleton_mutex()
		{
				static MutexType		mutex;
				return mutex;
		}

protected:
		Singleton()
		{
		
		}

		~Singleton()
		{

		}
public:
		static T& Instance();
		
		static void UnInstance();
};

template<class T>
T& Singleton<T>::Instance()
{
		typename MutexType::ScopeLock lock(get_singleton_mutex());
		
		if(get_singleton_pobj() == 0)
		{
				get_singleton_pobj() = new T();
		}

		return (*(T*)get_singleton_pobj());
		
}

template<class T>
void Singleton<T>::UnInstance()
{
		typename MutexType::ScopeLock lock(get_singleton_mutex());
		if(get_singleton_pobj() != 0)
		{
				delete get_singleton_pobj();
				get_singleton_pobj() = 0;
		}
}


}