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
#pragma once

#include "../NetSock.h"


namespace NetSpace {


bool NetSock::CloseSocket(NetHandle sour_hdl)
{
		if(sour_hdl != NET_INVALID_SOCKET)
		{
				return (::closesocket(sour_hdl) == 0);
		}else
		{
				return false;
		}
}


bool NetSock::GetOpt(int level, int option, void *optval,int *optlen)const
{
		assert(m_sock != NET_INVALID_SOCKET);
		return (::getsockopt(m_sock, level, option, (char*)optval, optlen) == 0);
}

bool NetSock::SetOpt(int level, int option, void *optval,int optlen)const
{
		assert(m_sock != NET_INVALID_SOCKET);
		return (::setsockopt(m_sock, level, option, (char*)optval, optlen) == 0);

}

bool NetSock::Control(int cmd, void *val)const
{
		assert(m_sock != NET_INVALID_SOCKET);
		return (::ioctlsocket(m_sock, cmd, (t_ulong*)val) == 0);


}

bool NetSock::Enable(SockCtrlCmd op)const
{
		assert(m_sock != NET_INVALID_SOCKET);
		t_ulong enable = 1;
		switch(op)
		{
		case NET_NONBLOCK:
				enable = 1;
				return Control(FIONBIO, (void*)&enable);
		default:
				return false;
		}



}

bool NetSock::Disable(SockCtrlCmd op)const
{
		assert(m_sock != NET_INVALID_SOCKET);
		t_ulong disable = 0;
		switch(op)
		{
		case NET_NONBLOCK:
				disable = 0;
				return Control(FIONBIO, (void*)&disable);
		default:
				return false;
		}
}


NetHandle NetSock::Duplicate(NetHandle sour_hdl)
{
		assert(sour_hdl != NET_INVALID_SOCKET);

		::HANDLE dest = INVALID_HANDLE_VALUE;
		BOOL res = ::DuplicateHandle(::GetCurrentProcess(), 
									 (HANDLE)sour_hdl, 
									 ::GetCurrentProcess(),
									  &dest, 
									  0, 
									  FALSE, 
									  DUPLICATE_SAME_ACCESS
									  );

		assert(res);
		if(!res)
		{
				return NET_INVALID_SOCKET;
		}else
		{
				return (NetHandle)dest;
		}


}

#if(0)
NetHandle NetSock::Duplicate()const
{
		assert(IsValid());
		
		
		::HANDLE dest = INVALID_HANDLE_VALUE;
		BOOL res = ::DuplicateHandle(::GetCurrentProcess(), 
									 (HANDLE)m_sock, 
									 ::GetCurrentProcess(),
									  &dest, 
									  0, 
									  FALSE, 
									  DUPLICATE_SAME_ACCESS
									  );

		assert(res);
		if(!res)
		{
				return NET_INVALID_SOCKET;
		}else
		{
				return (NetHandle)dest;
		}
}
#endif
















}