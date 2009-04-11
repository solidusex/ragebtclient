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
#include "Acceptor.h"

namespace NetSpace {


bool Acceptor::IsValid()const
{
		return (m_sock != INVALID_SOCKET);
}

bool Acceptor::Accept(SockStream &new_sock)const
{
		assert(IsValid());
		assert(!new_sock.IsValid());
		
		NetHandle cli_sock = ::accept(m_sock, 0, 0);
		new_sock.Attach(cli_sock);
		return new_sock.IsValid();
}

void Acceptor::Close()
{
		NetSock::Close();
}


#define CHECK_AND_CLOSE(cond) do { if(!cond) { NetSock::Close(); return false; } } while(0)

bool Acceptor::Open(const InetAddress &local_addr, bool reuse_addr, int backlog)
{
		assert(m_sock == NET_INVALID_SOCKET);

		bool res = NetSock::Open(AF_INET, SOCK_STREAM, 0);
		
		if(!res) return false;
		
		if(reuse_addr)
		{
				int val = 1;
				CHECK_AND_CLOSE(
						SetOpt(SOL_SOCKET, SO_REUSEADDR, (void*)&val, sizeof(val))
						);
		}
		
		res = (::bind(m_sock, local_addr.Saddr(), local_addr.Length()) == 0);
		
		CHECK_AND_CLOSE(res);
		
		assert(backlog > 0);
		res = (::listen(m_sock, backlog) == 0);
		
		CHECK_AND_CLOSE(res);

		return res;
}

#undef CHECK_AND_CLOSE

Acceptor::Acceptor()
{
		
}


Acceptor::~Acceptor()
{
		if(IsValid())
		{
				try{
						Close();
				}catch(const std::exception &expt)
				{
						DEBUG_PRINT1("UnKnow exception == %s\n", expt.what());
						assert(0);
				}
		}
}



//////////////////////////////////////AcceptorEx/////////////////////////
InetAddress AcceptorEx::GetLocalAddr()const
{
		assert(m_acceptor.IsValid());
		InetAddress addr;
		m_acceptor.GetLocalAddr(addr);
		return addr;

}

AcceptorEx::AcceptorEx(bool reuse_addr, int backlog)
{
		InetAddress addr;
		addr.SetAddr(::htonl(INADDR_LOOPBACK));
		addr.SetPort(0);

		if(!m_acceptor.Open(addr, reuse_addr, backlog))
		{
				throw ExceptionSpace::NetException("Acceptor Open Failed : in AcceptorEx::AcceptorEx", NetGetLastError());
		}
		if(!m_acceptor.Enable(NET_NONBLOCK))
		{
				throw ExceptionSpace::NetException("Acceptor Enable(NET_NONBLOCK) Failed : in AcceptorEx::AcceptorEx", NetGetLastError());
		}

		Mask(INPUT_MASK);
}

AcceptorEx::AcceptorEx(t_uint16 listen_port, bool reuse_addr, int backlog)
{
		InetAddress addr;
		addr.SetAddr(INADDR_ANY);
		addr.SetPort(listen_port);

		if(!m_acceptor.Open(addr, reuse_addr, backlog))
		{
				throw ExceptionSpace::NetException("Acceptor Open Failed : in AcceptorEx::AcceptorEx", NetGetLastError());
		}
		if(!m_acceptor.Enable(NET_NONBLOCK))
		{
				throw ExceptionSpace::NetException("Acceptor Enable(NET_NONBLOCK) Failed : in AcceptorEx::AcceptorEx", NetGetLastError());
		}

		Mask(INPUT_MASK);
}

AcceptorEx::AcceptorEx(const InetAddress &local_addr, bool reuse_addr, int backlog)
{
		if(!m_acceptor.Open(local_addr, reuse_addr, backlog))
		{
				throw ExceptionSpace::NetException("Acceptor Open Failed : in AcceptorEx::AcceptorEx", NetGetLastError());
		}
		if(!m_acceptor.Enable(NET_NONBLOCK))
		{
				throw ExceptionSpace::NetException("Acceptor Enable(NET_NONBLOCK) Failed : in AcceptorEx::AcceptorEx", NetGetLastError());
		}

		Mask(INPUT_MASK);
}

AcceptorEx::~AcceptorEx()
{
		try{
				if(m_acceptor.IsValid())
				{
						m_acceptor.Close();
				}
		}catch(const std::exception &expt)
		{
				DEBUG_PRINT1("Unknow exception %s : AcceptorEx::~AcceptorEx()", expt.what());
		}
		
}

void AcceptorEx::OnInput()
{
		assert(GetSelector() != 0);
		SockStream new_sock;
		
		while(m_acceptor.Accept(new_sock))
		{
				on_accepted(new_sock.Attach(NET_INVALID_SOCKET));
		}

		if(NetGetLastError() != EWOULDBLOCK)
		{
				on_error(NetGetLastError());
				//因为这个函数被响应就证明一定在某个selector.Run()中
				//如果ref count 为1 执行此操作後此对象被delete
				GetSelector()->RemoveHandler(this);
		}
}

NetHandle AcceptorEx::GetHandle()const
{
		assert(m_acceptor.IsValid());
		return m_acceptor.GetHandle();
}

















}