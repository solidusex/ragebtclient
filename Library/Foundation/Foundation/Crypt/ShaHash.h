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
#include "Algo/sha1.h"
#include "Algo/sha2.h"


namespace CryptSpace {





class FoundationAPI ShaHash
{

public:
		enum HashMode { SHA160 = 160, SHA256 = 256, SHA384 = 384, SHA512 = 512 };
private:
		HashMode				m_mode;
		sha1_ctx				*m_psha160;
		sha256_ctx				*m_psha256;
		sha384_ctx				*m_psha384;
		sha512_ctx				*m_psha512;
public:
		ShaHash(HashMode  mode);
		~ShaHash();
public:
		void Reset();

		void Begin();

		void Update(const t_byte *data, t_ulong len);

		void End(t_byte *hash_value);
			
};



}