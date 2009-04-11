
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

#include "RefCountedObject.h"

template<class Type>
class SharedPtrDetail : public RefCountedObject
{
public:
		Type			*ptr;
public:
		SharedPtrDetail(Type *p) :ptr(p)
		{

		}
		
		~SharedPtrDetail()
		{
				delete ptr;
		}
};



template<class Type>
class SharedPtrDetailArray : public RefCountedObject
{
public:
		Type			*ptr;
public:
		explicit SharedPtrDetailArray(Type *p) :ptr(p)
		{

		}
		
		~SharedPtrDetailArray()
		{
				delete [] ptr;
		}
};



template<class Type, class PtrType>
class SharedPtrImpl
{
private:
		static void* operator new(size_t size);
		static void operator delete(void *ptr);

		static void* operator new[](size_t size);
		static void operator delete[](void *ptr);

private:
		
private:
		PtrType				*m_ptr;
public:
		SharedPtrImpl() : m_ptr(new PtrType(0))
		{

		}

		explicit SharedPtrImpl(Type *ptr)
		{
				m_ptr = new PtrType(ptr);
		}

		SharedPtrImpl(const SharedPtrImpl& other)
		{
				assert(other.m_ptr != 0);
				bool cond = other.m_ptr->Duplicate();
				assert(cond);
				m_ptr = other.m_ptr;
		}
		
		SharedPtrImpl& operator=(const SharedPtrImpl& other)
		{
				assert(m_ptr != 0);
				assert(other.m_ptr != 0);
				if(this != &other)
				{
						bool cond = other.m_ptr->Duplicate();
						assert(cond);
						m_ptr->Release();
						m_ptr = other.m_ptr;
				}
				return *this;
		}
		
		~SharedPtrImpl()
		{
				assert(m_ptr != 0);
				m_ptr->Release();
		}
public:
		void Swap(Type *ptr)
		{
				Swap(SharedPtrImpl(ptr));
		}

		void Swap(SharedPtrImpl& other)
		{
				assert(m_ptr != 0);
				assert(other.m_ptr != 0);
				PtrType *tmp = m_ptr;
				m_ptr = other.m_ptr;
				other.m_ptr = tmp;
				
		}

		Type* Get()									//返回当前指针;
		{
				return m_ptr->ptr;
		}

		const Type* Get()const
		{
				return m_ptr->ptr;
		}

		void Reset(Type* ptr)							//delete前一个,然后设定后一个为ptr
		{
				Swap(ptr);
		}
		
		Type& operator*()
		{
				return *(m_ptr->ptr);
		}

		const Type& operator*()const
		{
				return *(m_ptr->ptr);
		}
		
		Type* operator->()
		{
				return Get();
		}

		const Type* operator->()const
		{
				return Get();
		}
		
		Type& operator[](size_t idx)
		{
				return m_ptr->ptr[idx];
		}

		const Type& operator[](size_t idx)const
		{
				return m_ptr->ptr[idx];
		}
};



#define SharedPtr(Type)  SharedPtrImpl<Type, SharedPtrDetail<Type> >

#define SharedArray(Type)  SharedPtrImpl<Type, SharedPtrDetailArray<Type> >
