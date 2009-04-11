
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

#include "../Connector.h"

namespace NetSpace {







bool Connector::Connect(SockStream &new_stream, const InetAddress &remote_addr, const t_uint64 *timeout)
{
		assert(new_stream.IsValid());

		if(ConnectNB(new_stream, remote_addr))
		{
				return new_stream.Disable(NET_NONBLOCK);
		}else
		{
				if(NetGetLastError() == EWOULDBLOCK)
				{

						::Sleep(35);

						if(IsCompleted(new_stream, timeout))
						{
								return new_stream.Disable(NET_NONBLOCK);
						}
				}
				
				return false;
		}
}


bool Connector::IsCompleted(const SockStream &sock_stream, const t_uint64 *timeout)
{

		SocketSet w_set;
		SocketSet ex_set;
		w_set.Set(sock_stream.GetHandle());
		ex_set.Set(sock_stream.GetHandle());
		
		int ret = Utility::Select(0, &w_set, &ex_set, timeout);
		if(ret == 0 || ret == -1) return false;
		return (!w_set.GetActive().empty() && ex_set.GetActive().empty());

}





//////////////////////////////////ConnectorEx//////////////////////////////


ConnectorEx::ConnectorEx(const InetAddress &addr, t_uint64 timeout)
{
		m_addr = addr;

		if(!m_sock.Open())
		{
				throw ExceptionSpace::NetException("Connecte failed : in ConnectorEx::ConnectorEx", NetGetLastError());
		}

		m_timeout = timeout;
		
		if(!m_sock.Enable(NET_NONBLOCK))
		{
				throw ExceptionSpace::NetException("Enable NET_NONBLOCK mode  failed : ConnectorEx::ConnectorEx", NetGetLastError());
		}

		::connect(m_sock.GetHandle(), addr.Saddr(), addr.Length());

		::Sleep(35);//winµÄbug

		Mask(OUTPUT_MASK|TIMER_MASK);
		
	
}

void ConnectorEx::OnOutput()
{
		t_uint64 timeout = 0;
		int res = Utility::HandleExceptionReady(m_sock.GetHandle(), &timeout);

		if(res != 0)
		{
				on_error(NetGetLastError());
		}else
		{
				NetHandle new_hdl = m_sock.Duplicate();
				on_connected(new_hdl);
		}
		assert(GetSelector() != 0);
		GetSelector()->RemoveHandler(this);
}













}