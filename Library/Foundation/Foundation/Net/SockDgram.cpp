
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
#include "SockDgram.h"


#if defined(OS_FAMILY_WINDOWS)

		#if !defined(SIO_UDP_CONNRESET)
				#define   IOC_VENDOR   0x18000000
				#define   _WSAIOW(x,y)   (IOC_IN|(x)|(y))
				#define   SIO_UDP_CONNRESET   _WSAIOW(IOC_VENDOR,12)
		#endif

		#define WINSOCK_UDP_CONNRESET_BUG(sock_fd)		\
				do{										\
				BOOL bNewBehavior = FALSE;				\
				DWORD dwBytesReturned = 0;				\
														\
				int res = WSAIoctl(sock_fd,				\
				SIO_UDP_CONNRESET,						\
				&bNewBehavior,							\
				sizeof(bNewBehavior),					\
				NULL,									\
				0,										\
				&dwBytesReturned,						\
				NULL,									\
				NULL);									\
				assert(res == 0);						\
				}while(0)
#else

		#define WINSOCK_UDP_CONNRESET_BUG(sock_fd)

#endif

namespace NetSpace {


bool SockDgram::Open(const InetAddress &local_addr)
{
		assert(!IsValid());
		if(!SockDgram::Open())
		{
				return false;
		}

		if(::bind(m_sock, local_addr.Saddr(), local_addr.Length()) != 0)
		{
				return false;
		}
		
		return true;
}


bool SockDgram::Open(const InetAddress &local_addr, const InetAddress &remote_addr)
{
		assert(!IsValid());
		if(!Open(local_addr))
		{
				return false;
		}

		return (::connect(m_sock, remote_addr.Saddr(), remote_addr.Length()) == 0);
}


SockDgram::SockDgram()
{
				


}

SockDgram::~SockDgram()
{
		Close();
}




bool SockDgram::Open()
{
		assert(!IsValid());
		bool res = NetSock::Open(AF_INET, SOCK_DGRAM, 0);
		if(res)
		{
				WINSOCK_UDP_CONNRESET_BUG(m_sock);
		}
		return res;
}
/*
void SockDgram::Close()
{
		if(IsValid())
		{
				NetSock::Close();
		}
}*/


int SockDgram::Recv(t_byte *buf, size_t len, InetAddress &addr, int flags)
{
		assert(IsValid());
		int adr_len  = addr.Length();
		return ::recvfrom(m_sock, (char*)buf, len, flags, addr.Saddr(), &adr_len);
}

int SockDgram::Recv(t_byte *buf, size_t len, InetAddress &addr, int flags, const t_uint64 *ptimeout)
{
		assert(IsValid());

		int res = Utility::HandleReadReady(m_sock, ptimeout);

		switch(res)
		{
		case 0:
				NetSetLastError(ETIME);
				return -1;
		case -1:
				return -1;
		default:
				return Recv(buf, len, addr,flags);
		}
}


int SockDgram::Send(const t_byte *buf, size_t len, const InetAddress &addr, int flags)
{
		assert(IsValid());
		return ::sendto(m_sock, (const char*)buf, len, flags, addr.Saddr(), addr.Length());
}


int SockDgram::Send(const t_byte *buf, size_t len, const InetAddress &addr, int flags, const t_uint64 *ptimeout)
{
		assert(IsValid());

		int res = Utility::HandleWriteReady(m_sock, ptimeout);

		switch(res)
		{
		case 0:
				NetSetLastError(ETIME);
				return -1;
		case -1:
				return -1;
		default:
				return Send(buf, len, addr, flags);
		}
}























}