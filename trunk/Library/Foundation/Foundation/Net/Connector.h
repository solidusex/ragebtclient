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
#include "InetAddress.h"
#include "SockStream.h"
#include "Selector.h"

namespace NetSpace {




class FoundationAPI Connector
{
public:
		//timeout 为millisecond

		//阻塞connect
		static bool Connect(SockStream &new_stream, 
							const InetAddress &remote_addr, 
							const t_uint64 *timeout = 0
						);
		
		//非阻塞
		static bool ConnectNB(SockStream &sock_stream, const InetAddress &addr);
		
		//查看一个sock是否完成
		static bool IsCompleted(const SockStream &sock_stream, const t_uint64 *timeout);

};


class FoundationAPI ConnectorEx : public NetEventHandler
{
private:
		TimeSpace::TimeStamp	m_ts;
		t_uint64				m_timeout; //毫秒
		SockStream				m_sock;
		InetAddress				m_addr;
private:
		using RefCountedObject::Duplicate;
public:
		//此建构函数只是建立一个非阻塞的socket并且对其调用一次::connect函数
		ConnectorEx(const InetAddress &addr, t_uint64 timeout);

		virtual ~ConnectorEx();
protected:
		//在以下三个函数内，不要千千万万不要GetSelector()->RemoveHandler(this)这么做
		//因为这三个函数的后续会帮你调用它,如果你调用它了，那么当它再次调用时候
		//实际上此对象已经被删除了
		virtual void on_connected(NetHandle hdl) = 0;

		virtual void on_timeout() = 0;

		virtual void on_error(t_ulong error_code) = 0;
public:
		virtual NetHandle GetHandle()const;

		virtual void OnOutput();

		virtual void OnTimer();
public:
		const InetAddress& GetRemoteAddr()const;

		
};



}