
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
#include "SocketSet.h"

#if defined(OS_FAMILY_WINDOWS)
		#include "Win/SocketSetImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif


namespace NetSpace {


///////////////////////////////////FdSetIterator////////////////////////////////









FdSetIterator::FdSetIterator(const fd_set & sock_set)
{
		Reset(sock_set);
}

FdSetIterator::~FdSetIterator()
{

}



void FdSetIterator::Reset(const fd_set & sock_set)
{
		m_curr = 0;
		memcpy(&m_set, &sock_set, sizeof(m_set));


}

////////////////////////////SocketSet//////////////////////////////////////////

SocketSet::SocketSet() 
{
		Reset();
}
		
SocketSet::~SocketSet()
{
		
}
		

SocketSet::SocketSet(const SocketSet& other)
{
		*this = other;
}


SocketSet& SocketSet::operator=(const SocketSet& other)
{
		if(this != &other)
		{
				memcpy((void*)&m_mask, (void*)&(other.m_mask), sizeof(m_mask));
				m_set = other.m_set;
		}
		return *this;
}


SocketSet::SocketSet(const fd_set &mask)
{
		Reset(mask);
}

void SocketSet::Reset(void)
{
		m_set.clear();
		FD_ZERO(&m_mask);
}

void SocketSet::Reset(const fd_set &mask)
{
		Reset();
		memcpy((void*)&m_mask, (void*)&mask, sizeof(m_mask));
		
		FdSetIterator fd_iter(m_mask);

		for(fd_iter.First(); !fd_iter.IsDone(); fd_iter.Next())
		{
				assert(!ISSet(fd_iter.Current()));
				m_set.insert(fd_iter.Current());
		}
}

bool SocketSet::ISSet(NetHandle sock) const
{
		return (m_set.count(sock) != 0);
}

void SocketSet::Set(NetHandle sock)
{
		assert(m_set.count(sock) == 0);
		m_set.insert(sock);
		assert(m_set.count(sock) == 1);
}

void SocketSet::Clr(NetHandle sock)
{
		assert(m_set.count(sock) == 1);
		m_set.erase(sock);
		assert(m_set.count(sock) == 0);
}



fd_set* SocketSet::Fdset()
{
		return &m_mask;
}



void SocketSet::Sync()
{
		FD_ZERO(&m_mask);
		for(std::set<NetHandle>::iterator it = m_set.begin(); it != m_set.end(); ++it)
		{
				FD_SET(*it, &m_mask);
		}
}
		
size_t SocketSet::NumSet() const
{
		return m_set.size();
}
		




SocketSet::ActiveSockSet SocketSet::GetActive()const
{
		ActiveSockSet v_set;
		
		FdSetIterator fd_iter(m_mask);

		for(fd_iter.First(); !fd_iter.IsDone(); fd_iter.Next())
		{
				v_set.push_back(fd_iter.Current());
		}
		return v_set;
}



}