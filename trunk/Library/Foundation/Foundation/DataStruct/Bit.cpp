
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
#include "Bit.h"

namespace DSSpace{


std::string PrintBits(t_uint64 bits, t_uint32 n)
{
		assert(n <= 64 && n > 0);
		std::string str = "";
		
		for(int i = n -1; i >= 0; --i)
		{
				str += (BIT_TEST(bits, i) ? '1' : '0');
		}

		return str;
}



static const t_byte /*ReverseBitSet::*/HEX_BIT[8] = 
{

		BIT_MARK(7), BIT_MARK(6), BIT_MARK(5), BIT_MARK(4), 
		BIT_MARK(3), BIT_MARK(2), BIT_MARK(1), BIT_MARK(0)
};



ReverseBitSet::ReverseBitSet(const ReverseBitSet &other)
{
		*this = other;

}

ReverseBitSet& ReverseBitSet::operator=(const ReverseBitSet &other)
{
		if(this != &other)
		{
				m_nbytes = other.m_nbytes;
				m_nbits = other.m_nbits;
				m_bitset = other.m_bitset;
		}

		return *this;
}

void ReverseBitSet::Clear()
{
		memset(&m_bitset[0], 0, m_bitset.size());
}


const t_byte* ReverseBitSet::GetContent()const
{
		return (&m_bitset[0]);
}

ReverseBitSet::ReverseBitSet(const t_byte *raw_data, size_t nbits)
{
		Reset(raw_data, nbits);
}




ReverseBitSet::ReverseBitSet(size_t nbits)
{
		Reset(nbits);
}

size_t ReverseBitSet::NBits()const
{
		return m_nbits;
}
		
size_t ReverseBitSet::NBytes()const
{
		return m_nbytes;
}

bool ReverseBitSet::operator[](size_t idx)const
{
		return IsSet(idx);
}

bool ReverseBitSet::IsSet(size_t idx) const
{
		assert(idx < m_nbits);
		if(idx >= m_nbits)
		{
				throw std::logic_error("ReverseBitSet::IsSet : idx out of range");
		}

		return ((m_bitset[idx / 8] & HEX_BIT[idx % 8]) != 0);
}

void ReverseBitSet::Set(size_t idx)
{
		assert(idx < m_nbits);

		if(idx >= m_nbits)
		{
				throw std::logic_error("ReverseBitSet::Set : idx out of range");
		}
		
		t_byte tmp = m_bitset[idx / 8];
		
		tmp |= HEX_BIT[idx % 8];
		m_bitset[idx/8] = tmp;

}

void ReverseBitSet::UnSet(size_t idx)
{
		assert(idx < m_nbits);

		if(idx >= m_nbits)
		{
				throw std::logic_error("ReverseBitSet::UnSet : idx out of range");
		}

		t_byte tmp = m_bitset[idx/8];
		tmp &= ~HEX_BIT[idx % 8];
		m_bitset[idx/8] = tmp;

}


void ReverseBitSet::Reset(size_t nbits)
{
		assert(nbits != 0);

		m_nbits = nbits;
		
		m_nbytes = m_nbits / 8;
		
		if(m_nbits % 8)
		{
				m_nbytes++;
		}

		m_bitset.resize(m_nbytes);

		memset(&m_bitset[0], 0, m_bitset.size() * sizeof(t_byte));
}

void ReverseBitSet::Reset(const t_byte *raw_data, size_t nbits)
{
		assert(nbits > 0);
		assert(raw_data != NULL);
		
		Reset(nbits);
		
		assert(m_nbits > 0);
	//	assert(m_nbytes <= nbytes);
		

		memcpy(&m_bitset[0], raw_data, m_nbytes);

		t_int64 remain = m_nbits % 8;
		if(remain != 0)
		{
				t_byte last_byte = 0;
				t_byte last_tmp = m_bitset.back();
				while(--remain >= 0)
				{
						if(last_tmp & HEX_BIT[remain])
						{
								last_byte |= HEX_BIT[remain];
						}
				}
				m_bitset.back() = last_byte;
		}

}

void ReverseBitSet::Flip()
{
		assert(!m_bitset.empty());

		for(size_t i = 0; i < m_nbytes; ++i)
		{
				m_bitset[i] = ~m_bitset[i];
		}
		
		t_int64 remain = m_nbits % 8;

		if(remain != 0)
		{
				t_byte last_byte = 0;
				t_byte last_tmp = m_bitset.back();
				while(--remain >= 0)
				{
						if((last_tmp & HEX_BIT[remain]))
						{
								last_byte |= HEX_BIT[remain];
						}
				}
				m_bitset.back() = last_byte;
		}

		
}

bool ReverseBitSet::IsEmpty()const
{
		assert(!m_bitset.empty());
		
		std::vector<t_byte> emp(m_bitset.size(), 0);

		return (memcmp(&m_bitset[0], &emp[0], emp.size()) == 0);
}

bool ReverseBitSet::IsAllSet()const
{
		assert(!m_bitset.empty());
		std::vector<t_byte> tmp(m_bitset.size(), (t_byte)~0);
		ReverseBitSet comp(&tmp[0], NBits());
		
		return (*this == comp);

}

std::string ReverseBitSet::Print()const
{
		std::string str = "";
		for(t_uint32 i = 0; i < m_nbits; ++i)
		{
				str += (IsSet(i) ? "1" : "0");
		}

		return str;
}



bool ReverseBitSet::operator==(const ReverseBitSet &other)const
{
		assert(!m_bitset.empty());
		assert(!other.m_bitset.empty());
		assert(other.NBits() == NBits());
		
		if(m_bitset.empty() || (other.NBits() != NBits()))
		{
				throw std::logic_error("ReverseBitSet::operator== failed");
		}

		return (memcmp(&m_bitset[0], &other.m_bitset[0], m_nbytes) == 0);
}

bool ReverseBitSet::operator!=(const ReverseBitSet &other)const
{
		return !(*this == other);
				
}


ReverseBitSet& ReverseBitSet::operator |= (const ReverseBitSet &other)
{
		*this = (*this | other);
		return *this;
				
}


ReverseBitSet& ReverseBitSet::operator ^= (const ReverseBitSet &other)
{
		
		*this = (*this ^ other); 
		return *this;

}

ReverseBitSet& ReverseBitSet::operator &=(const ReverseBitSet &other)
{
		*this = (*this & other);
		return *this;
}

ReverseBitSet ReverseBitSet::operator~()const
{
		assert(!m_bitset.empty());
		
		if(m_bitset.empty())
		{
				throw std::logic_error("ReverseBitSet::operator~  failed");
		}

		//ReverseBitSet rbs(NBits());
		ReverseBitSet rbs(*this);

		//rbs = *this;

		rbs.Flip();
		
		return rbs;
}

ReverseBitSet ReverseBitSet::operator &(const ReverseBitSet &other)const
{
		assert(!m_bitset.empty());
		assert(!other.m_bitset.empty());
		assert(other.NBits() == NBits());

		if(m_bitset.empty() || other.m_bitset.empty() || (other.NBits() != NBits()))
		{
				throw std::logic_error("ReverseBitSet::operator & failed");
		}


		std::vector<t_byte> temp(m_nbytes, 0);

		for(size_t i = 0; i < m_nbytes; ++i)
		{
				temp[i] = m_bitset[i] & other.m_bitset[i];
		}

		return ReverseBitSet(&temp[0], m_nbits);
}


ReverseBitSet ReverseBitSet::operator | (const ReverseBitSet &other)const
{

		assert(!m_bitset.empty());
		assert(!other.m_bitset.empty());
		assert(other.NBits() == NBits());

		if(m_bitset.empty() || other.m_bitset.empty() || (other.NBits() != NBits()))
		{
				throw std::logic_error("ReverseBitSet::operator |=  failed");
		}


		std::vector<t_byte> temp(m_nbytes, 0);

		for(size_t i = 0; i < m_nbytes; ++i)
		{
				temp[i] = m_bitset[i] | other.m_bitset[i];
		}

		return ReverseBitSet(&temp[0], m_nbits);

		
}




ReverseBitSet ReverseBitSet::operator ^ (const ReverseBitSet &other)const
{
		assert(!m_bitset.empty());
		assert(!other.m_bitset.empty());
		assert(other.NBits() == NBits());

		if(m_bitset.empty() || other.m_bitset.empty() || (other.NBits() != NBits()))
		{
				throw std::logic_error("ReverseBitSet::operator ^   failed");
		}


		std::vector<t_byte> temp(m_nbytes, 0);

		for(size_t i = 0; i < m_nbytes; ++i)
		{
				temp[i] = m_bitset[i] ^ other.m_bitset[i];
		}

		return ReverseBitSet(&temp[0], m_nbits);
}







ReverseBitSet& ReverseBitSet::operator<<= (size_t nbit)
{
		if(nbit > 0)
		{
				*this = (*this << nbit);
		}

		return *this;
}

ReverseBitSet ReverseBitSet::operator<< (size_t nbit)const
{
		if(nbit ==  0) return *this;
		
		ReverseBitSet rbs(m_nbits);

		if(nbit < m_nbits)
		{
				size_t rbs_ib = 0;
				for(size_t ib = nbit; ib < m_nbits; ++ib)
				{
						if(IsSet(ib))
						{
								rbs.Set(rbs_ib);
						}

						rbs_ib++;
				}
		}

		return rbs;
}


ReverseBitSet ReverseBitSet::operator>> (size_t nbit)const
{
		
		if(nbit ==  0) return *this;
		
		ReverseBitSet rbs(m_nbits);

		if(nbit < m_nbits)
		{
				size_t remain_nbits = m_nbits - nbit;
				size_t rbs_ib = nbit;
				for(size_t ib = 0; ib < remain_nbits; ++ib)
				{
						if(IsSet(ib))
						{
								rbs.Set(rbs_ib);
						}

						rbs_ib++;
				}
		}
		return rbs;
}




ReverseBitSet& ReverseBitSet::operator>>= (size_t nbit)
{
		if(nbit > 0)
		{
				*this = (*this >> nbit);
		}

		return *this;
}






}
