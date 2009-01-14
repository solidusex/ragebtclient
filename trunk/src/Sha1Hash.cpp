/*
 * The Rage Library
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
 */#include "Sha1Hash.h"


namespace Rage {


void Sha1Hash::Clear()
{
		m_data.Reset(160);
}

void Sha1Hash::Reset(const t_byte data[20])
{
		m_data.Reset(data, 160);

}

const t_byte* Sha1Hash::GetRawData()const
{
		return m_data.GetContent();
}

Sha1Hash::Sha1Hash(const t_byte data[20])
{
		*this = data;

}

Sha1Hash& Sha1Hash::operator=(const t_byte data[20])
{
		assert(data != 0);
		Reset(data);

		return *this;

}

t_int32 Sha1Hash::Log2N()const
{
		for(t_uint32 idx = 0; idx < 160; ++idx)
		{
				if(m_data.IsSet(idx))
				{
						return 159 - idx;
				}
		}

		return -1;//返回-1证明为0
}

std::string Sha1Hash::ToString()const
{
		return m_data.Print();

		
}

Sha1Hash::Sha1Hash() : m_data(160)
{
		

}


Sha1Hash::~Sha1Hash()
{

}


Sha1Hash::Sha1Hash(const Sha1Hash &other)
{
		*this = other;
}

Sha1Hash& Sha1Hash::operator=(const Sha1Hash &other)
{
		if(this != &other)
		{
				m_data = other.m_data;
		}
		return *this;
}

bool Sha1Hash::operator!=(const Sha1Hash &other)const
{		
		
		return !(*this == other);
}


bool Sha1Hash::operator==(const Sha1Hash &other)const
{
		if(this != &other)
		{
				return (m_data == other.m_data);
		}
		return true;
}

bool Sha1Hash::operator <= (const Sha1Hash &other)const
{
		return (*this < other || *this == other);
}

bool Sha1Hash::operator >= (const Sha1Hash &other)const
{
		return (*this > other || *this == other);
}

bool Sha1Hash::operator<(const Sha1Hash &other)const
{
		for(t_uint32 idx = 0; idx < 160; ++idx)
		{
				if((m_data.IsSet(idx) && other.m_data.IsSet(idx)) ||
						(!m_data.IsSet(idx) && !other.m_data.IsSet(idx)))
				{
						continue;
				}else
				{
						if(m_data.IsSet(idx))
						{
								return false;
						}else
						{
								return true;
						}
				}
		}
		
		return false;
}

bool Sha1Hash::operator>(const Sha1Hash &other)const
{
		for(t_uint32 idx = 0; idx < 160; ++idx)
		{
				if((m_data.IsSet(idx) && other.m_data.IsSet(idx)) ||
						(!m_data.IsSet(idx) && !other.m_data.IsSet(idx)))
				{
						continue;
				}else
				{
						if(m_data.IsSet(idx))
						{
								return true;
						}else
						{
								return false;
						}
				}
		}

		return false;

}

Sha1Hash Sha1Hash::operator^(const Sha1Hash &other)const
{
		Sha1Hash new_hash;
		new_hash.m_data = m_data ^ other.m_data;
		return new_hash;
}


std::string Sha1Hash::ToHexString()const
{
		const t_byte *pdata = m_data.GetContent();
		assert(pdata);
		std::string res = "";
		for(t_uint32 idx = 0; idx < 20; ++idx)
		{
				t_uint32 val = (t_uint32)pdata[idx];
				res += FormatSpace::NumberFormatter::FormatHex(val, 2);
		}
		return res;
}




}