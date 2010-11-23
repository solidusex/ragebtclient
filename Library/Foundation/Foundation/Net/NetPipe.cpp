
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
#include "NetPipe.h"

#include "Acceptor.h"
#include "Connector.h"
#include "NetSock.h"
#include "InetAddress.h"
#include "SockStream.h"

namespace NetSpace {

int NetPipe::Available()
{
		assert(IsOpen());
		return m_reader.Available();

}

bool NetPipe::IsOpen()const
{
		return (m_reader.IsValid() && m_writer.IsValid());
}

NetPipe::NetPipe()
{


}

NetPipe::~NetPipe()
{
		//assert(!m_reader.IsValid() && !m_writer.IsValid());
		Close();
}


#define CHECK_AND_RET(cond) do{ if(!cond) { Close(); NetSetLastError(0) ;return false; } }while(0)

bool NetPipe::Open(size_t buf_size)
{
		assert(!m_reader.IsValid() && !m_writer.IsValid());
		
		if(m_reader.IsValid() || m_writer.IsValid()) return false;
		
		InetAddress addr;
		addr.SetAddr(::htonl(INADDR_LOOPBACK));
		addr.SetPort(0);
		
		Acceptor acceptor;
		
		CHECK_AND_RET( acceptor.Open(addr) );
		
		
		CHECK_AND_RET( acceptor.GetLocalAddr(addr) );
		



		CHECK_AND_RET( m_writer.Open() );
		
		int opt_val = 0;
		CHECK_AND_RET( m_writer.SetOpt(IPPROTO_TCP, TCP_NODELAY, &opt_val, sizeof(opt_val))	);
		
		opt_val = buf_size;
		
		CHECK_AND_RET( m_writer.SetOpt(SOL_SOCKET, SO_SNDBUF, &opt_val, sizeof(opt_val)) );

		CHECK_AND_RET(	Connector::Connect(m_writer, addr, 0)	);

		CHECK_AND_RET(	acceptor.Accept(m_reader) );

		assert(m_reader.IsValid() && m_writer.IsValid());

		m_reader.CloseWriter();
		m_writer.CloseReader();
		
		CHECK_AND_RET( m_reader.Enable(NET_NONBLOCK) && m_writer.Enable(NET_NONBLOCK) );

		return true;

}
#undef CHECK_AND_RET


void NetPipe::Close()
{
		m_reader.Close();
		m_writer.Close();
}

NetHandle NetPipe::ReaderHandle()const
{
		return m_reader.GetHandle();
}

NetHandle NetPipe::WriterHandle()const
{
		return m_writer.GetHandle();
}


bool NetPipe::Read(t_byte* buf, size_t len)
{
		assert(buf != 0 && len != 0);
		if(len == 0) return true;
		size_t bt = 0;
		
		return (m_reader.Recv(buf, len, &bt, 0, 0) > 0);

}

bool NetPipe::Write(const t_byte* buf, size_t len)
{
		assert(buf != 0 && len != 0);
		if(len == 0) return true;
		size_t bt = 0;
		
		return (m_writer.Send(buf, len, &bt, 0, 0) > 0);

}






}