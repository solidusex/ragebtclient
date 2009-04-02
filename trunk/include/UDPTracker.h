/*
* The Rage Library
* Copyright (c) 2007 by Solidus
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* provided that the above copyright notice appear in all copies and
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*
*/
#pragma once

#include "TypeDef.h"
#include "Tracker.h"

namespace Rage {


#define UDP_WAIT_TIMEOUT  5000		//∫¡√Î

class UDPTracker : NonCopyable
{
private:
		typedef NetSpace::SockDgram		SockDgramT;
		typedef NetSpace::InetAddress	NetAddrT;
		typedef DSSpace::SimpleBuffer			BufferT;
private:
		enum ACTION
		{ 
				ACTION_CONNECT = 0, 
				ACTION_ANNOUNCE = 1, 
				ACTION_SCRAPE = 2, 
				ACTION_ERROR = 3 
		};
private:
		std::string				m_url;
		t_uint16				m_port;
private:
		const RequestInfo		&m_req_info;
		ResponseInfo			&m_return_info;
private:
		t_uint64				m_connection_id;
		t_uint32				m_action;
		t_uint32				m_transaction_id;
private:
		SockDgramT				m_sock;
		NetAddrT				m_addr;
private:
		bool get_host_entry();

		bool connect_to_tracker();

		bool announcing_udp_tracker();

		bool get_tracker_response();
		
		bool parse_announce_response(const t_byte *pbuf, size_t len);

public:
		UDPTracker( const std::string &url, 
					t_uint16 port, 
					const RequestInfo &req_info, 
					ResponseInfo &return_info
					);
		
		~UDPTracker();
public:
		void Run()throw();
};



}


