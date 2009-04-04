
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

#include "TypeDef.h"

#include <cassert>
#include <map>


namespace DSSpace {

class FoundationAPI Buffer : NonCopyable
{
public:
		struct AllocType
		{
				void* (*Malloc)(size_t size);
				void (*Free)(void*);
		};
public:

		struct FoundationAPI Interval
		{
				Interval(t_byte* begin, t_byte* end);

				t_byte operator[](int index) const;

				size_t Left() const;

				t_byte			*m_begin;
				t_byte			*m_end;
		};

		struct FoundationAPI ConstInterval
		{

				ConstInterval(const t_byte *begin, const t_byte *end);

				t_byte operator[](int index) const;

				size_t Left() const;

				const t_byte* m_begin;
				const t_byte* m_end;
		};
public:
		typedef std::pair<ConstInterval, ConstInterval> IntervalType;
public:
		Buffer(std::size_t n = 1024, const AllocType *palloc = 0);

		~Buffer();
public:
		Interval Allocate(std::size_t n);

		void Insert(const t_byte* pbuf, size_t len);

		void Erase(std::size_t n);

		std::size_t Size() const;

		std::size_t Capacity() const;

		void Reserve(std::size_t n);

		IntervalType Data() const;

		bool Empty() const;

		std::size_t SpaceLeft() const;

		const t_byte* RawData() const
		{
				return m_first;
		}
private:
		AllocType		m_alloc;
private:
		t_byte* m_first;
		t_byte* m_last;
		t_byte* m_write_cursor;
		t_byte* m_read_cursor;
		t_byte* m_read_end;
		bool m_empty;

};




class FoundationAPI SimpleBuffer : private NonCopyable
{
public:
		struct AllocType
		{
				void* (*Malloc)(size_t size);
				void (*Free)(void*);
		};
private:
		t_byte*					m_first;
		t_byte*					m_last;
		t_byte*					m_read_cursor;
		t_byte*					m_write_cursor;
		size_t					m_buf_size;
private:
		AllocType				m_alloc;
private:
		void increase_capability(size_t len);

		bool move_internal(size_t len);
public:
		SimpleBuffer(AllocType *palloc = 0);
		
		~SimpleBuffer();
public:
		void Clear();			//清空，释放内部buf

		bool IsEmpty()const;	//是否无数据可读（并非buf为0)
public:
		t_byte* Allocate(std::size_t n);

		void Insert(const t_byte* pbuf, size_t len);

		size_t Erase(std::size_t n);

		std::size_t Size() const;		//有多少字节可读

		std::size_t Capacity() const;	//可以插入直接插入多少字节而不用重分配空间

		void Reserve(std::size_t n);	//预留n个字节的空间，而不必

		const t_byte* Data() const;		//返回可读数据的起始指针
		
		const t_byte* RawData()const;

		size_t RawBufSize()const;//返回当前buf大小（全部）
};



}