
/*
 * The Rage Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  It is provided "as is" without express
 * or implied warranty.
 */

#include "UDPTracker.h"
#include "URL.h"

namespace Rage {




UDPTracker::UDPTracker( const std::string &url, 
					   t_uint16 port, 
					   const RequestInfo &req_info, 
					   ResponseInfo &return_info
					   )
		: m_url(url)
		, m_port(port)
		, m_req_info(req_info)
		, m_return_info(return_info)
{


}
		
UDPTracker::~UDPTracker()
{




}






bool UDPTracker::get_host_entry()
{
		NetSpace::HostEntry		he;

		if(!he.Reset(m_url))
		{
				return false;
		}
		
		NetSpace::HostEntry::AddrList addrlist = he.GetAddrList();
		
		if(addrlist.empty()) return false;
		
		m_addr = addrlist[0];
		m_addr.SetPort(m_port);
		
		//DEBUG_PRINT2("addr == %s:%d\n", m_addr.Str().c_str(), m_addr.Port());

		if(!m_sock.Open()) return false;

		return m_sock.Enable(NetSpace::NET_NONBLOCK);
}


bool UDPTracker::connect_to_tracker()
{
		m_connection_id = 0x41727101980;
		m_action		= ACTION_CONNECT;
		m_transaction_id = (t_uint32)TimeSpace::CycleTimer::GetCycleCount() % 2000;


		t_byte buf[16];
		
		*((t_uint64*)buf)		=	ByteOrder::FromNativeOrderToNetOrder(m_connection_id);
		*((t_uint32*)(buf + 8)) = ByteOrder::FromNativeOrderToNetOrder(m_action);
		*((t_uint32*)(buf + 12)) = ByteOrder::FromNativeOrderToNetOrder(m_transaction_id);

		
		t_uint64 timeout = UDP_WAIT_TIMEOUT;
		
		int len = m_sock.Send(buf, sizeof(buf), m_addr, 0, &timeout);
		m_sock.Send(0, 0, m_addr, 0, 0);
		
		if(len != sizeof(buf))
		{
				return false;
		}

		memset(buf, 0, sizeof(buf));
		
		NetSpace::InetAddress addr;
		len = m_sock.Recv(buf, sizeof(buf), addr,0,  &timeout);
		
		if(len != sizeof(buf))
		{
				return false;
		}
		
		
		t_uint32 action = ByteOrder::FromNetOrderToNativeOrder(*((t_uint32*)buf));
		t_uint32 transaction_id =ByteOrder::FromNetOrderToNativeOrder(*((t_uint32*)(buf + 4)));
		t_uint64 connection_id = ByteOrder::FromNetOrderToNativeOrder(*((t_uint64*)(buf + 8)));
		
		
		if(transaction_id != m_transaction_id) return false;
		
		

		if(action != ACTION_CONNECT) return false;

		m_connection_id = connection_id;
		return true;
}

bool UDPTracker::announcing_udp_tracker()
{
		m_transaction_id = (t_uint32)TimeSpace::CycleTimer::GetCycleCount() % 2000;
		m_action = ACTION_ANNOUNCE;

		BufferT buf;
		NetSpace::InetAddress iadr;

		t_uint64 *pconn_id = (t_uint64*)buf.Allocate(sizeof(t_uint64));
		*pconn_id = ByteOrder::FromNativeOrderToNetOrder(m_connection_id);

		t_uint32 *p_action = (t_uint32*)buf.Allocate(sizeof(t_uint32));
		*p_action = ByteOrder::FromNativeOrderToNetOrder(m_action);
		
		t_uint32 *p_trans_id = (t_uint32*)buf.Allocate(sizeof(t_uint32));
		*p_trans_id = ByteOrder::FromNativeOrderToNetOrder(m_transaction_id);
		
		buf.Insert(m_req_info.info_hash.GetRawData(), 20);
		buf.Insert(m_req_info.peer_id.GetRawData(), 20);
		
		t_uint64 *p_down = (t_uint64*)buf.Allocate(sizeof(t_uint64));
		*p_down = ByteOrder::FromNativeOrderToNetOrder(m_req_info.downloaded);
		
		t_uint64 *p_left = (t_uint64*)buf.Allocate(sizeof(t_uint64));
		*p_left = ByteOrder::FromNativeOrderToNetOrder(m_req_info.left);
		
		t_uint64 *p_up = (t_uint64*)buf.Allocate(sizeof(t_uint64));
		*p_up = m_req_info.uploaded;
		
		t_uint32 *p_event = (t_uint32*)buf.Allocate(sizeof(t_uint32));
		*p_event = m_req_info.event;
		
		t_uint32 listen_addr = 0;
		buf.Insert((t_byte*)&listen_addr, sizeof(t_uint32));

		t_uint32 *p_key = (t_uint32*)buf.Allocate(sizeof(t_uint32));
		*p_key = ByteOrder::FromNativeOrderToNetOrder(m_req_info.key);

		t_uint32 *p_num_want = (t_uint32*)buf.Allocate(sizeof(t_uint32));
		*p_num_want = ByteOrder::FromNativeOrderToNetOrder(m_req_info.numwant);
		
		t_uint16 *p_listen_port = (t_uint16*)buf.Allocate(sizeof(t_uint16));
		*p_listen_port = ByteOrder::FromNativeOrderToNetOrder(m_req_info.listen_port);
		t_uint16 ext = 0;
		buf.Insert((t_byte*)&ext, sizeof(t_uint16));

		t_uint64 timeout = UDP_WAIT_TIMEOUT;
		
		int len = m_sock.Send(buf.Data(), buf.Size(), m_addr, 0, &timeout);
		
		if(len != buf.Size())
		{
				return false;
		}
		
		m_sock.Send(0, 0, m_addr, 0, 0);
		
		return true;
}

#define MAX_RECV_BUF_LENGTH (8*KB)


bool UDPTracker::get_tracker_response()
{
		t_byte buf[MAX_RECV_BUF_LENGTH];

		NetSpace::InetAddress iadr;
		
		t_uint64 timeout = UDP_WAIT_TIMEOUT;
		int len = m_sock.Recv(buf, MAX_RECV_BUF_LENGTH, m_addr, 0, &timeout);
		
		if(len <= 20)
		{
				return false;
		}
		
		

		m_action = ByteOrder::FromNetOrderToNativeOrder(*((t_uint32*)buf));
		
		t_uint32 tid = ByteOrder::FromNetOrderToNativeOrder(*((t_uint32*)(buf + 4)));


		if(m_action == ACTION_ANNOUNCE && tid == m_transaction_id)
		{
				return parse_announce_response(buf + 8, len - 8);
		}else
		{
				return false;
		}
}

bool UDPTracker::parse_announce_response(const t_byte *pbuf, size_t len)
{
		assert(len >= 12);
		const t_byte *last = pbuf + len;
		
		m_return_info.interval = ByteOrder::FromNetOrderToNativeOrder(*(t_uint32*)pbuf);
		pbuf += sizeof(t_uint32);
		m_return_info.completed = ByteOrder::FromNetOrderToNativeOrder(*(t_uint32*)pbuf);
		pbuf += sizeof(t_uint32);
		m_return_info.incompleted = ByteOrder::FromNetOrderToNativeOrder(*(t_uint32*)pbuf);
		pbuf += 4;
		
		len -= 12;
		while(len >= 6)
		{
				PeerEntry pe;
				pe.addr = *(t_uint32*)pbuf;
				pbuf += 4;
				pe.port = ByteOrder::FromNetOrderToNativeOrder(*(t_uint16*)pbuf);
				m_return_info.peers.push_back(pe);
				pbuf += 2;
				len -= 6;
		}
		return true;
}





void UDPTracker::Run()throw()
{
		if(!get_host_entry())
		{
				m_return_info.result = TS_RES_DNS_ERROR;
				return;
		}


		if(!connect_to_tracker())
		{
				m_return_info.result = TS_RES_CONN_ERROR;
				return;
		}

		if(!announcing_udp_tracker())
		{
				m_return_info.result = TS_RES_REPLY_ERROR;
				return;
		}

		if(!get_tracker_response())
		{
				m_return_info.result = TS_RES_REPLY_ERROR;
				return;
		}

		m_return_info.result = TS_RES_SUCCESS;
}














}
