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

#include "ShaHash.h"



namespace CryptSpace {

#define SIZE_HASH_BUFFER       16384



ShaHash::ShaHash(HashMode mode) : m_mode(mode) , m_psha160(0), m_psha256(0), m_psha384(0), m_psha512(0)
{
		
}

ShaHash::~ShaHash()
{
		Reset();
}


void ShaHash::Reset()
{
		delete m_psha160;
		m_psha160 = 0;

		delete m_psha256;
		m_psha256 = 0;

		delete m_psha384;
		m_psha384 = 0;

		delete m_psha512;
		m_psha512 = 0;
}

void ShaHash::Update(const t_byte *data, t_ulong len)
{
		
		t_ulong nleft = len;
		const t_byte *pdata = data;

		while(nleft > 0)
		{
				t_ulong n = (nleft <= SIZE_HASH_BUFFER ? nleft : SIZE_HASH_BUFFER);
				switch(m_mode)
				{
				case SHA256:
						sha256_hash(pdata, n, m_psha256);
						break;
				case SHA384:
						sha384_hash(pdata, n, m_psha384);
						break;
				case SHA512:
						sha512_hash(pdata, n, m_psha512);
						break;
				default:
						sha1_hash(pdata, n, m_psha160);
						break;
				}

				nleft -= n;
				pdata += n;
		}
}

void ShaHash::Begin()
{
		Reset();
		switch(m_mode)
		{
		case SHA256:
				m_psha256 = new sha256_ctx;
				sha256_begin(m_psha256);
				break;
		case SHA384:
				m_psha384 = new sha384_ctx;
				sha384_begin(m_psha384);
				break;
		case SHA512:
				m_psha512 = new sha512_ctx;
				sha512_begin(m_psha512);
				break;
		default:
				m_psha160 = new sha1_ctx;
				sha1_begin(m_psha160);
				break;
		}
}

void ShaHash::End(t_byte *hash_value)
{
		switch(m_mode)
		{
		case SHA256:
				sha256_end(hash_value, m_psha256);
				break;
		case SHA384:
				sha384_end(hash_value, m_psha384);
				break;
		case SHA512:
				sha512_end(hash_value, m_psha512);
				break;
		default:
				sha1_end(hash_value, m_psha160);
				break;
		}

		Reset();
}











}