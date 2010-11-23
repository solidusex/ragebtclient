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
#include "MDHash.h"

#include <memory>




namespace CryptSpace {






#define SIZE_HASH_BUFFER       16384

MDHash::MDHash(HashMode mode) : m_mode(mode), m_pmd2(0), m_pmd4(0), m_pmd5(0)
{


}

MDHash::~MDHash()
{
		Reset();
}

void MDHash::Reset()
{
		delete m_pmd2;
		m_pmd2 = 0;

		delete m_pmd4;
		m_pmd4 = 0;

		delete m_pmd5;
		m_pmd5 = 0;
}

void MDHash::End(t_byte *hash_value)
{

		switch(m_mode)
		{
		case MD2:
				m_pmd2->TruncatedFinal(hash_value, 16);
				break;
		case MD4:
				MD4Final(hash_value, m_pmd4);
				break;
		default:
				MD5Final(m_pmd5);
				memcpy(hash_value, m_pmd5->digest, 16);
				break;
		}
		
		Reset();
}


void MDHash::Update(const t_byte *data, t_ulong len)
{
		const t_byte *pdata = data;
		t_ulong nleft = len;
		while(nleft > 0)
		{
				t_ulong n = (nleft <= SIZE_HASH_BUFFER ? nleft : SIZE_HASH_BUFFER);
				switch(m_mode)
				{
				case MD2:
						m_pmd2->Update(pdata, n);
						break;
				case MD4:
						MD4Update(m_pmd4, const_cast<t_byte*>(pdata), n);
						break;
				default:
						MD5Update(m_pmd5, const_cast<t_byte*>(pdata), n);
				}

				nleft -= n;
				pdata += n;
		}

		
}

void MDHash::Begin()
{
		Reset();
		switch(m_mode)
		{
		case MD2:
				m_pmd2 = new CMD2;
				m_pmd2->Init();
				break;
		case MD4:
				m_pmd4 = new MD4_CTX;
				MD4Init(m_pmd4);
				break;
		default:
				m_pmd5 = new MD5_CTX;
				MD5Init(m_pmd5, 0);
				break;
		}
		

}












}