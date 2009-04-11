
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

#include "NetSock.h"
#include "Utility.h"

namespace NetSpace {

//timeout 单位为 毫秒

class FoundationAPI SockStream : public NetSock, private NonCopyable
{
private:
		NetSock::Open;
		//NetSock::Close;
public:
		bool Open();

		bool Open(const InetAddress &local_addr);
		
		//void Close();

		bool CloseReader();

		bool CloseWriter();

		int Available()const;
		
		//返回当前的NetHandle
		NetHandle Attach(NetHandle sock = NET_INVALID_SOCKET);
public:
		SockStream();
		
		SockStream(NetHandle sock_handle);

		~SockStream();
		
		int Recv(t_byte *buf, size_t len, int flags = 0)const;

		//此函数应当被调用在nonblocking的io模式下
		int Recv(t_byte *buf, size_t len, size_t *bt , const t_uint64 *timeout = 0, int flags = 0)const;

		int Send(const t_byte *buf, size_t len, int flags = 0)const;
		
		//此函数应当被调用在nonblocking的io模式下
		int Send(const t_byte *buf, size_t len, size_t *bt, const t_uint64 *timeout = 0, int flags = 0)const;
};















}