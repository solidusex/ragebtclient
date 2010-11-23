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

namespace StreamSpace {


template<class T>
class AbstractStreamBuffer
{
public:
		virtual void Put(const T &obj) = 0;		//增加到buffer中
		
		virtual void Put(const T *arr, size_t n) = 0;

		virtual void Get(T &obj) = 0;			//从buf中取出，之后buf不再存在

		virtual size_t Get(T *arr, size_t n) = 0;
		
		virtual t_uint64  Available()const = 0;

		virtual const T* GetContent()const = 0;
		
		virtual size_t Skip(size_t n) = 0;

		virtual void Clear() = 0;

		virtual ~AbstractStreamBuffer()
		{

		}
};


template<class T>
class AbstractOutputStream
{
public:
		virtual void Flush()  = 0;
       
		virtual void Write(const T &obj) = 0;

		virtual void Write(const T  *arr, size_t size) = 0;

		virtual AbstractStreamBuffer<T>* Buffer(AbstractStreamBuffer<T>* pbuf) = 0;

		virtual AbstractStreamBuffer<T>* Buffer() = 0;

		virtual ~AbstractOutputStream()
		{

		}

};


template<class T>
class AbstractInputStream
{
private:
		typedef typename AbstractOutputStream<T>	OutputStreamType;
public:
		virtual t_uint64 Available()const = 0;
		
		virtual void Read(T &item) = 0;

		virtual size_t Read(T *arr, size_t size) = 0;
		
		virtual size_t Skip(size_t n) = 0;

		virtual AbstractStreamBuffer<T>* Buffer(AbstractStreamBuffer<T>* pbuf) = 0;

		virtual AbstractStreamBuffer<T>* Buffer() = 0;

		virtual ~AbstractInputStream()
		{

		}
};


















}

