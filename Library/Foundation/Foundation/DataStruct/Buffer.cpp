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

#include "Buffer.h"

namespace DSSpace {

////////////////////////////Buffer::iterator//////////////////////////////////////////
		
Buffer::Interval::Interval(t_byte* begin, t_byte* end) : m_begin(begin), m_end(end)
{

}

t_byte Buffer::Interval::operator[](int index) const
{
		assert(m_begin + index < m_end);
		return m_begin[index];
}

size_t Buffer::Interval::Left() const 
{ 
		assert(m_end >= m_begin); 
		return (int)(m_end - m_begin);
}


		
 
 
Buffer::ConstInterval::ConstInterval(const t_byte *begin, const t_byte *end) 
{
		m_begin = begin;
		m_end = end;
}

t_byte Buffer::ConstInterval::operator[](int index) const
{
		assert(m_begin + index < m_end);
		return m_begin[index];
}

size_t Buffer::ConstInterval::Left() const
{ 
		assert(m_end >= m_begin); 
		return (int)(m_end - m_begin);
}



////////////////////////////////////Buffer////////////////////////////////////
Buffer::Buffer(std::size_t n, const Buffer::AllocType *palloc)
{
		if(palloc == 0)
		{
				m_alloc.Malloc = ::malloc;
				m_alloc.Free = ::free;
		}else
		{
				m_alloc = *palloc;
		}

		
		//m_first = (t_byte*)::malloc(n);
		n = (n >= 1024 ? n : 1024);
		m_first = (t_byte*)m_alloc.Malloc(n);
		
		if(m_first == 0)
		{
				assert(false);
				throw std::bad_alloc("bad alloc : Buffer::Buffer");
		}

		m_last = m_first + n;
		m_write_cursor = m_first;
		m_read_cursor = m_first;
		m_read_end = m_last;
		m_empty = true;
}

Buffer::~Buffer()
{
		//::free(m_first);
		m_alloc.Free(m_first);
}

std::size_t Buffer::Capacity() const
{
		return m_last - m_first;
}


std::size_t Buffer::SpaceLeft() const
{
		if (m_empty) return m_last - m_first;

		// ...R***W.
		if (m_read_cursor < m_write_cursor)
		{
				return (m_last - m_write_cursor) + (m_read_cursor - m_first);
		}
		// ***W..R*
		else
		{
				return m_read_cursor - m_write_cursor;
		}
}

void Buffer::Insert(const t_byte* pbuf, size_t len)
{
		assert(pbuf != 0);
		if(len == 0)
		{
				return;
		}

		std::size_t n = len;

		if (SpaceLeft() < n)
		{
				Reserve(Capacity() + n);
		}

		m_empty = false;

		const t_byte* end = (m_last - m_write_cursor) < (std::ptrdiff_t)n 
				? m_last : m_write_cursor + n;

		std::size_t copied = end - m_write_cursor;
		std::memcpy(m_write_cursor, pbuf, copied);

		m_write_cursor += copied;
		if (m_write_cursor > m_read_end) 
		{
				m_read_end = m_write_cursor;
		}

		pbuf += copied;
		n -= copied;

		if (n == 0) return;

		assert(m_write_cursor == m_last);
		m_write_cursor = m_first;

		memcpy(m_write_cursor, pbuf, n);
		m_write_cursor += n;
}


void Buffer::Reserve(std::size_t size)
{
		std::size_t n = (std::size_t)(Capacity() * 1.5f);
		if (n < size)
		{
				n = size;
		}

		t_byte *buf = (t_byte*)m_alloc.Malloc(n);
		if(buf == 0)
		{
				assert(false);
				throw std::bad_alloc("bad alloc : Buffer::Reserve");
		}

		
		t_byte* old = m_first;

		if (m_read_cursor < m_write_cursor)
		{
				// ...R***W.<>.
				std::memcpy(buf + (m_read_cursor - m_first), m_read_cursor, 
						m_write_cursor - m_read_cursor);

				m_write_cursor = buf + (m_write_cursor - m_first);
				m_read_cursor = buf + (m_read_cursor - m_first);
				m_read_end = m_write_cursor;
				m_first = buf;
				m_last = buf + n;
		}else
		{
				// **W..<>.R**
				std::size_t skip = n - (m_last - m_first);

				std::memcpy(buf, m_first, m_write_cursor - m_first);
				std::memcpy(
						buf + (m_read_cursor - m_first) + skip
						, m_read_cursor
						, m_last - m_read_cursor
						);

				m_write_cursor = buf + (m_write_cursor - m_first);

				if (!m_empty)
				{
						m_read_cursor = buf + (m_read_cursor - m_first) + skip;
						m_read_end = buf + (m_read_end - m_first) + skip;
				}
				else
				{
						m_read_cursor = m_write_cursor;
						m_read_end = m_write_cursor;
				}

				m_first = buf;
				m_last = buf + n;
		}

		//::free(old);
		m_alloc.Free(old);
}


void Buffer::Erase(std::size_t n)
{
		if (n == 0) return;
		assert(!m_empty);

		assert(m_read_cursor <= m_read_end);
		m_read_cursor += n;
		if (m_read_cursor > m_read_end)
		{
				m_read_cursor = m_first + (m_read_cursor - m_read_end);
				assert(m_read_cursor <= m_write_cursor);
		}

		m_empty = (m_read_cursor == m_write_cursor);
}

std::size_t Buffer::Size() const
{
		// ...R***W.
		if (m_read_cursor < m_write_cursor)
		{
				return m_write_cursor - m_read_cursor;
		}
		// ***W..R*
		else
		{
				if (m_empty) return 0;
				return (m_write_cursor - m_first) + (m_read_end - m_read_cursor);
		}
}


Buffer::IntervalType Buffer::Data() const
{
		// ...R***W.
		if (m_read_cursor < m_write_cursor)
		{
				return IntervalType(
						ConstInterval(m_read_cursor, m_write_cursor)
						, ConstInterval(m_last, m_last)
						);
		}
		// **W...R**
		else
		{
				if (m_read_cursor == m_read_end)
				{

						return IntervalType(
								ConstInterval(m_first, m_write_cursor)
								, ConstInterval(m_last, m_last));
				}


				assert(m_read_cursor <= m_read_end || m_empty);
				return IntervalType(
						ConstInterval(m_read_cursor, m_read_end)
						, ConstInterval(m_first, m_write_cursor)
						);
		}
}


bool Buffer::Empty() const
{
		return m_empty;
}


Buffer::Interval Buffer::Allocate(std::size_t n)
{
		assert(m_read_cursor <= m_read_end || m_empty);


		if (m_read_cursor < m_write_cursor || m_empty)
		{
				// ..R***W..
				if (m_last - m_write_cursor >= (std::ptrdiff_t)n)
				{
						Interval ret(m_write_cursor, m_write_cursor + n);
						m_write_cursor += n;
						m_read_end = m_write_cursor;
						assert(m_read_cursor <= m_read_end);
						if (n) m_empty = false;
						return ret;
				}

				if (m_read_cursor - m_first >= (std::ptrdiff_t)n)
				{
						m_read_end = m_write_cursor;
						Interval ret(m_first, m_first + n);
						m_write_cursor = m_first + n;
						assert(m_read_cursor <= m_read_end);
						if (n) m_empty = false;
						return ret;
				}

				Reserve(Capacity() + n - (m_last - m_write_cursor));
				assert(m_last - m_write_cursor >= (std::ptrdiff_t)n);
				Interval ret(m_write_cursor, m_write_cursor + n);
				m_write_cursor += n;
				m_read_end = m_write_cursor;
				if (n) m_empty = false;
				assert(m_read_cursor <= m_read_end);
				return ret;

		}
		//**W...R**
		if (m_read_cursor - m_write_cursor >= (std::ptrdiff_t)n)
		{
				Interval ret(m_write_cursor, m_write_cursor + n);
				m_write_cursor += n;
				if (n) m_empty = false;
				return ret;
		}
		Reserve(Capacity() + n - (m_read_cursor - m_write_cursor));
		assert(m_read_cursor - m_write_cursor >= (std::ptrdiff_t)n);
		Interval ret(m_write_cursor, m_write_cursor + n);
		m_write_cursor += n;
		if (n) m_empty = false;
		return ret;
}


///////////////////////////////////////////////////////////////////////////////////////


SimpleBuffer::SimpleBuffer(SimpleBuffer::AllocType *palloc)
						: m_first(0)
						, m_last(0)
						, m_read_cursor(0)
						, m_write_cursor(0)
						, m_buf_size(0)
{
		if(palloc != 0)
		{
				m_alloc = *palloc;
		}else
		{
				m_alloc.Malloc = ::malloc;
				m_alloc.Free = ::free;
		}
}

SimpleBuffer::~SimpleBuffer()
{
		Clear();

}


bool SimpleBuffer::move_internal(size_t len)
{
		if(m_buf_size == 0) return false;

		assert(m_read_cursor >= m_first);
		
		size_t vacancy_len = m_read_cursor - m_first;
		size_t cur_size = m_write_cursor - m_read_cursor;
		size_t cap_size = m_last - m_write_cursor;
		
		if(vacancy_len < cur_size || (cap_size + vacancy_len) < len)
		{
				return false;
		}else
		{
				memcpy(m_first, m_read_cursor, cur_size);
				m_read_cursor = m_first;
				m_write_cursor = m_read_cursor + cur_size;
				return true;
		}
}

t_byte* SimpleBuffer::Allocate(size_t len)
{
		assert(m_write_cursor <= m_last);

		if(m_write_cursor == m_last || (m_last - m_write_cursor) < len )
		{
				if(!move_internal(len))
				{
						//float inc_size = (float)m_buf_size * 1.5f + (float)len;
						//increase_capability((size_t)inc_size);
						increase_capability(len);
				}
		}
		
		assert(m_first != 0);
		assert(m_write_cursor >= m_read_cursor);
		assert((m_last - m_write_cursor) >= len);
		t_byte *res = m_write_cursor;
		assert(res != 0);
		m_write_cursor += len;
		return res;

}


void SimpleBuffer::Insert(const t_byte* pbuf, size_t len)
{
		assert(pbuf != 0);

		t_byte *write_pos = Allocate(len);
		memcpy(write_pos, pbuf, len);
}

/*
void SimpleBuffer::Insert(const t_byte* pbuf, size_t len)
{
		assert(m_write_cursor <= m_last);

		if(m_write_cursor == m_last || (m_last - m_write_cursor) < len )
		{
				if(!move_internal(len))
				{
						float inc_size = (float)m_buf_size * 1.5f + (float)len;
						increase_capability((size_t)inc_size);
						//increase_capability(len);
				}
		}
		
		assert(m_first != 0);
		assert(m_write_cursor >= m_read_cursor);
		assert((m_last - m_write_cursor) >= len);
		memcpy(m_write_cursor, pbuf, len);
		m_write_cursor += len;
}*/

size_t SimpleBuffer::Erase(std::size_t n)
{
		size_t remain_len = m_write_cursor - m_read_cursor;
		
		size_t read_n = (remain_len < n ? remain_len : n);
		
		if(read_n != 0)
		{
				m_read_cursor += read_n;
		}
		return read_n;



}

std::size_t SimpleBuffer::Size() const		//有多少字节可读
{

		return (m_write_cursor - m_read_cursor);

}

std::size_t SimpleBuffer::Capacity() const	//可以插入直接插入多少字节而不用重分配空间
{
		return (m_last - m_write_cursor) + (m_read_cursor - m_first);
}


void SimpleBuffer::increase_capability(size_t len)
{
		assert(m_read_cursor <= m_write_cursor);
		assert(m_buf_size >= 0);

		t_byte *pbuf = (t_byte*)m_alloc.Malloc(m_buf_size + len);
		
		assert(m_write_cursor >= m_read_cursor);
		size_t cont_len = m_write_cursor - m_read_cursor;
		
		if(cont_len > 0)
		{
				memcpy(pbuf, m_read_cursor, cont_len);
		}

		m_buf_size += len;
		m_alloc.Free(m_first);
		m_first = pbuf;
		m_last = m_first + m_buf_size;

		m_read_cursor = m_first;
		m_write_cursor = m_read_cursor + cont_len;

}

void SimpleBuffer::Reserve(std::size_t len)	//预留n个字节的空间，而不必
{
		if(!move_internal(len))
		{
				size_t capa = Capacity();
				//如果总共可用的空间（读取完的+未使用的）小于len,则增加len - capa个字节
				//否则，增加0字节（就是重整buf,将read_cursor定位到m_first，后面的连起来
				//的空间就够用了
				size_t inc_size = (len < capa ? 0 : len - capa);
				increase_capability(inc_size);
		}
}

const t_byte* SimpleBuffer::Data() const		//返回可读数据的起始指针
{
		if(Size() > 0)
		{
				return m_read_cursor;
		}else
		{
				return 0;
		}
}

const t_byte* SimpleBuffer::RawData()const
{
		return m_first;
}

size_t SimpleBuffer::RawBufSize()const//返回当前buf大小（全部）
{
		return m_buf_size;
}


void SimpleBuffer::Clear()
{
		m_alloc.Free(m_first);
		m_first = 0;
		m_last = 0;
		m_read_cursor = 0;
		m_write_cursor = 0;
		m_buf_size = 0;
}

bool SimpleBuffer::IsEmpty()const
{
		return (Size() == 0);
}






}