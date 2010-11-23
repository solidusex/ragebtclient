
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
#include <cassert>
#include <string>
#include <vector>



#define BIT_MARK(pos) (t_uint64(0x01) << pos)

#define BIT_TEST(val, pos)  ((bool)((val & (BIT_MARK(pos)))))

#define BIT_SET(val, pos)   (val |= (BIT_MARK(pos)))

#define BIT_CLR(val, pos)   (val &=  (~(BIT_MARK(pos))))



namespace DSSpace{


std::string FoundationAPI PrintBits(t_uint64 bits, t_uint32 n);


class FoundationAPI ReverseBitSet
{
private:
		std::vector<t_byte>		m_bitset;
		size_t					m_nbits;
		size_t					m_nbytes;
public:
		ReverseBitSet(const t_byte *raw_data, size_t nbits);

		ReverseBitSet(size_t nbits = 8);

		~ReverseBitSet() { }

		ReverseBitSet(const ReverseBitSet &other);

		ReverseBitSet& operator=(const ReverseBitSet &other);

public:
		ReverseBitSet operator | (const ReverseBitSet &other)const;

		ReverseBitSet& operator |= (const ReverseBitSet &other);
public:
		bool operator==(const ReverseBitSet &other)const;

		bool operator!=(const ReverseBitSet &other)const;

		bool operator[](size_t idx)const;
		
		ReverseBitSet operator &(const ReverseBitSet &other)const;
		
		ReverseBitSet operator ^ (const ReverseBitSet &other)const;

		ReverseBitSet  operator~()const;

		ReverseBitSet& operator &=(const ReverseBitSet &other);
		
		ReverseBitSet& operator ^= (const ReverseBitSet &other);

		ReverseBitSet operator<< (size_t nbit)const;

		ReverseBitSet operator>> (size_t nbit)const;

		ReverseBitSet& operator<<= (size_t nbit);

		ReverseBitSet& operator>>= (size_t nbit);
public:
		void Reset(size_t nbits);

		void Reset(const t_byte *raw_data, size_t nbits);

		void Set(size_t idx);
		
		void UnSet(size_t idx);
		
		bool IsSet(size_t idx)const;

		size_t NBits()const;

		size_t NBytes()const;

		bool IsEmpty()const;

		bool IsAllSet()const;

		std::string Print()const;

		void Clear();

		void Flip();

		const t_byte *GetContent()const;



};

}