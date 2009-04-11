
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

#include "TypeDef.h"
#include "NetSock.h"
#include "InetAddress.h"
#include "SockStream.h"
#include "Selector.h"

namespace NetSpace {



class FoundationAPI Acceptor : public NetSock, private NonCopyable
{
private:
		NetSock::Open;
		NetSock::Close;
public:
		Acceptor();

		~Acceptor();

		bool Open(const InetAddress &local_addr, bool reuse_addr = true, int backlog = 15);
		
		void Close();

		bool Accept(SockStream &new_sock)const;

		bool IsValid()const;

};



class FoundationAPI AcceptorEx : public NetEventHandler
{
protected:
		Acceptor		m_acceptor;
protected:
		virtual void on_accepted(NetHandle hdl) = 0;

		virtual void on_error(t_ulong err_code) = 0;
public:
		AcceptorEx(bool reuse_addr = true, int backlog = 15);

		AcceptorEx(t_uint16 listen_port, bool reuse_addr = true, int backlog = 15);

		AcceptorEx(const InetAddress &local_addr, bool reuse_addr = true, int backlog = 15);
		
		virtual ~AcceptorEx();
public:
		virtual NetHandle GetHandle()const;
		//当有错误发生时，会先调用on_error()方法，然后会调用selector->RemoveHandler函数，
		//所有决不能在on_accepted,on_error或析构函数中调用GetSelector函数
		virtual void OnInput();

		InetAddress GetLocalAddr()const;
};







}