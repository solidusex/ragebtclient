
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
#include "NetSock.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/NetSockImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace NetSpace {


bool NetSock::IsValid()const 
{
		return m_sock != NET_INVALID_SOCKET;
}


bool NetSock::Open(int family, int type, int protocol)
{
		assert(m_sock == NET_INVALID_SOCKET);
		if(m_sock == NET_INVALID_SOCKET)
		{
				m_sock = ::socket(family, type, protocol);
				return (m_sock != NET_INVALID_SOCKET ? true : false);
		}else
		{
				return false;
		}
}


NetHandle NetSock::GetHandle()const
{
		return m_sock;
}

NetSock::~NetSock()
{
		if(m_sock != NET_INVALID_SOCKET)
		{
				Close();
				NetSetLastError(0);
		}
}


NetSock::NetSock() : m_sock(NET_INVALID_SOCKET)
{

}
		
NetSock::NetSock(const NetSock &other)
{
		*this = other;
}

NetSock& NetSock::operator=(const NetSock &other)
{
		if(this != &other)
		{
				//m_sock = other.m_sock;
				Close();
				m_sock = other.Duplicate();
		}

		return *this;
}

bool NetSock::GetRemoteAddr(InetAddress &addr)const
{
		assert(m_sock != NET_INVALID_SOCKET);
		addr.Clear();
		int len = addr.Length();
		return (::getpeername(m_sock, addr.Saddr(), &len) == 0);
}

bool NetSock::GetLocalAddr(InetAddress &addr)const
{
		assert(m_sock != NET_INVALID_SOCKET);
		addr.Clear();
		int len = addr.Length();
		return (::getsockname(m_sock, addr.Saddr(), &len) == 0);
}



NetHandle NetSock::Duplicate()const
{
		assert(IsValid());

		return Duplicate(m_sock);
}



bool NetSock::Close()
{
		bool res = CloseSocket(m_sock);
		m_sock = NET_INVALID_SOCKET;
		return res;
}



}