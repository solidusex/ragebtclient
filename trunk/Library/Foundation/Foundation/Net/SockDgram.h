
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
#include "Utility.h"

namespace NetSpace {



class FoundationAPI SockDgram : public NetSock, private NonCopyable
{
private:
		using NetSock::Open;
		using NetSock::GetRemoteAddr;
		
public:
		SockDgram();

		virtual ~SockDgram();

		bool Open();

		bool Open(const InetAddress &local_addr);
		
		//注意，这里的remote_addr的语意是对udp调用connect，
		//以过滤所来到的数据报和优化性能
		bool Open(const InetAddress &local_addr, const InetAddress &remote_addr);

		//void Close();

		int Recv(t_byte *buf, size_t len, InetAddress &addr, int flags);

		int Recv(t_byte *buf, size_t len, InetAddress &addr, int flags, const t_uint64 *ptimeout);


		int Send(const t_byte *buf, size_t len, const InetAddress &addr, int flags);

		int Send(const t_byte *buf, size_t len, const InetAddress &addr, int flags, const t_uint64 *ptimeout);
		
};





















}