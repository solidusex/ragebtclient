
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

#include "InetAddress.h"

namespace NetSpace {



class FoundationAPI NetSock
{
protected:
		NetHandle		m_sock;
public:
		bool IsValid()const;

		NetHandle GetHandle()const;
		
		bool Enable(SockCtrlCmd op)const;

		bool Disable(SockCtrlCmd op)const;

		bool Control(int cmd, void *val)const;

		bool SetOpt(int level, int option, void *optval,int optlen)const;

		bool GetOpt (int level, int option, void *optval,int *optlen)const;
		
		bool GetLocalAddr(InetAddress &addr)const;

		bool GetRemoteAddr(InetAddress &addr)const;

		bool Open(int family, int type, int protocol);
		
		bool Close();

public:
		NetHandle Duplicate()const;

		static NetHandle Duplicate(NetHandle sour_hdl);
		
		static bool CloseSocket(NetHandle sour_hdl);

protected:
		NetSock();
		
		NetSock(const NetSock &other);
		
		NetSock& operator=(const NetSock &other);
		
		virtual ~NetSock() = 0;
};











}