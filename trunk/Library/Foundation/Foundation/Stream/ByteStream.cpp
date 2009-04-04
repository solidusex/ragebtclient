
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
#include "ByteStream.h"


namespace StreamSpace {


/////////////////////////////ByteOutputStream//////////////////////////////



void ByteOutputStream::Write(const t_byte &b)
{
		m_pbuffer->Put(b);
}

void ByteOutputStream::Write(const t_byte *arr, size_t n)
{
		m_pbuffer->Put(arr, n);
}

void ByteOutputStream::Flush()
{


}

AbstractByteStreamBuffer* ByteOutputStream::Buffer()
{
		return m_pbuffer;
}


ByteOutputStream::ByteOutputStream(AbstractByteStreamBuffer* pbuf)
{
		assert(pbuf);

		m_pbuffer = pbuf;
}


ByteOutputStream::~ByteOutputStream()
{


}


AbstractByteStreamBuffer* ByteOutputStream::Buffer(AbstractByteStreamBuffer* pbuf)
{
		AbstractByteStreamBuffer *tmp = m_pbuffer;
		m_pbuffer = pbuf;
		return tmp;
}




/////////////////////////////ByteInputStream//////////////////////////////

AbstractByteStreamBuffer* ByteInputStream::Buffer()
{
		return m_pbuffer;
}

ByteInputStream::ByteInputStream(AbstractByteStreamBuffer *pbuf)
{
		assert(pbuf);
		m_pbuffer = pbuf;
}

ByteInputStream::~ByteInputStream()
{

}

t_uint64 ByteInputStream::Available()const
{
		return m_pbuffer->Available();

}


size_t ByteInputStream::Read(t_byte *arr, size_t size)
{
		return m_pbuffer->Get(arr, size);
}

void ByteInputStream::Read(t_byte &b)
{
		m_pbuffer->Get(b);
}


size_t ByteInputStream::Skip(size_t n)
{
		return m_pbuffer->Skip(n);
}



AbstractByteStreamBuffer* ByteInputStream::Buffer(AbstractByteStreamBuffer* pbuf)
{
		AbstractByteStreamBuffer *tmp = m_pbuffer;
		m_pbuffer = pbuf;
		return tmp;
}



}







#if(0)

/*
const t_byte* ByteInputStream::GetContent()const
{
		return m_pbuffer->GetContent();
}*/

const t_byte* ByteArrayStreamBuffer::GetContent()const
{
		if(!IsEmpty())
		{
				return (&m_buf[m_head]);
		}else
		{
				return 0;
		}
}


ByteArrayStreamBuffer::~ByteArrayStreamBuffer()
{

}

void ByteArrayStreamBuffer::Clear()
{
		m_head = 0;
		m_tail = -1;
		m_buf.swap(std::vector<t_byte>(0));
		m_buf.resize(400);
}

t_ulong ByteArrayStreamBuffer::Available()const
{
		if(m_tail < m_head)
		{
				return 0;
		}else
		{
				return (m_tail + 1 - m_head);
		}
}

bool ByteArrayStreamBuffer::IsEmpty()const
{
		return (Available() == 0);
}

ByteArrayStreamBuffer::ByteArrayStreamBuffer() : m_buf(400), m_head(0), m_tail(-1)
{

}



void ByteArrayStreamBuffer::increase_capability(t_ulong len)
{
		t_ulong buflen = m_buf.size();
		
		assert(buflen != 0);

		if(buflen < (m_tail + len + 1))
		{
				float total = m_tail + len + 1;
				m_buf.resize( total * 1.5);
		}
}

void ByteArrayStreamBuffer::Put(const t_byte *arr, t_ulong len)
{
		increase_capability(len);
		memcpy((void*)&m_buf[m_tail + 1], (void*)arr, len);
		m_tail += len;
}

t_ulong ByteArrayStreamBuffer::Get(t_byte *arr, t_ulong n)
{
		t_ulong count = std::min(Available(), n);
		memcpy((void*)arr, (void*)&m_buf[m_head], count);
		m_head += count;
		return count;
}

void ByteArrayStreamBuffer::Skip(t_ulong n)
{
		t_ulong count = std::min(Available(), n);
		m_head += count;
}


void ByteArrayStreamBuffer::Put(const t_byte &b)
{
		Put(&b, 1);
}
		
		
void ByteArrayStreamBuffer::Get(t_byte &b)
{
		Get(&b, 1);
}

#endif
