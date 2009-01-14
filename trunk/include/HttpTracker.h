/*
 * The Rage Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */#pragma once

#include "TypeDef.h"
#include "Tracker.h"
#include "HttpResponse.h"
#include "BencodingDecoder.h"
#include "PeerEntry.h"

#include <sstream>
namespace Rage {

#define TCP_WAIT_TIMEOUT  5000

class HttpTracker : private NonCopyable
{
private:
		typedef NetSpace::SockStream			SockStreamT;
		typedef NetSpace::InetAddress			NetAddrT;
		typedef DSSpace::SimpleBuffer			BufferT;
private:
		const RequestInfo		&m_req_info;
		ResponseInfo			&m_return_info;
private:
		SockStreamT				m_sock;
		NetAddrT				m_addr;
private:
		std::string				m_url;
		std::string				m_path;
		t_uint16				m_port;

		std::string				m_req_url_info;
private:
		std::stringstream		m_recv_ss;
		
private:
		std::string build_request_url();

		bool resolve_dns();
		
		bool request();

		bool parse_response(const std::vector<t_byte> content);

		bool reset_new_url(const std::string &new_url);
public:
		HttpTracker(const std::string &url, 
					t_uint16 port, 
					const std::string &path,
					const RequestInfo &req_info,
					ResponseInfo &return_info
					);
		
		~HttpTracker();
public:
		void Run()throw();

};








}
