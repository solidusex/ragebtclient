
/*
 * The Rage Library
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

#include "Config.h"



namespace Rage {


class RageAPI Sha1Hash
{
private:
		DSSpace::ReverseBitSet	m_data;
public:
		Sha1Hash();

		~Sha1Hash();

		Sha1Hash(const t_byte data[20]);

		Sha1Hash(const Sha1Hash &other);

		Sha1Hash& operator=(const Sha1Hash &other);

		Sha1Hash& operator=(const t_byte data[20]);

		const t_byte* GetRawData()const;

		void Reset(const t_byte data[20]);

		void Clear();
public:
		bool operator == (const Sha1Hash &other)const;
		
		bool operator != (const Sha1Hash &other)const;
		
		bool operator < (const Sha1Hash &other)const;
		
		bool operator > (const Sha1Hash &other)const;

		bool operator <= (const Sha1Hash &other)const;

		bool operator >= (const Sha1Hash &other)const;
		
		Sha1Hash operator^(const Sha1Hash &other)const;
public:
		std::string ToHexString()const;

		std::string ToString()const;

		t_int32 Log2N()const;
};


typedef Sha1Hash	PeerID;




}