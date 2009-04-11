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
		//timeout Ϊmillisecond

		//����connect
		static bool Connect(SockStream &new_stream, 
							const InetAddress &remote_addr, 
							const t_uint64 *timeout = 0
						);
		
		//������
		static bool ConnectNB(SockStream &sock_stream, const InetAddress &addr);
		
		//�鿴һ��sock�Ƿ����
		static bool IsCompleted(const SockStream &sock_stream, const t_uint64 *timeout);

};


class FoundationAPI ConnectorEx : public NetEventHandler
{
private:
		TimeSpace::TimeStamp	m_ts;
		t_uint64				m_timeout; //����
		SockStream				m_sock;
		InetAddress				m_addr;
private:
		using RefCountedObject::Duplicate;
public:
		//�˽�������ֻ�ǽ���һ����������socket���Ҷ������һ��::connect����
		ConnectorEx(const InetAddress &addr, t_uint64 timeout);

		virtual ~ConnectorEx();
protected:
		//���������������ڣ���Ҫǧǧ����ҪGetSelector()->RemoveHandler(this)��ô��
		//��Ϊ�����������ĺ�������������,�����������ˣ���ô�����ٴε���ʱ��
		//ʵ���ϴ˶����Ѿ���ɾ����
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