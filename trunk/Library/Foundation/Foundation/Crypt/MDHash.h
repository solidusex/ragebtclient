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

#pragma once


#include "TypeDef.h"
#include "Algo/md2.h"
#include "Algo/md4.h"
#include "Algo/md5.h"

namespace CryptSpace {



class FoundationAPI MDHash
{
public:
		enum HashMode { MD2 = 2, MD4 = 4, MD5 = 5 };
private:
		HashMode				m_mode;
		CMD2					*m_pmd2;
		MD4_CTX					*m_pmd4;
		MD5_CTX					*m_pmd5;
public:
		MDHash(HashMode mode);

		~MDHash();
		
		void Reset();

		void Begin();

		void Update(const t_byte *data, t_ulong len);

		void End(t_byte *hash_value);
		
};








}