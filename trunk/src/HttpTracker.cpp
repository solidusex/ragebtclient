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
#include "HttpTracker.h"
#include "URL.h"

namespace Rage {

#if defined(OS_FAMILY_WINDOWS)

#pragma warning(disable : 4244)

#define REQ_URL_FMT \
"GET %s?info_hash=%s&peer_id=%s&port=%d&uploaded=%I64d&downloaded=%I64d&left=%I64d&key=%d&compact=%d&numwant=%devent=%s&no_peer_id=1 HTTP/1.0\r\n\r\n"

#define snprintf _snprintf

#endif


std::string HttpTracker::build_request_url()
{

		std::string infohash = URLEncoder::Encoder((char*)m_req_info.info_hash.GetRawData(), 20);
		std::string peer_id = URLEncoder::Encoder((char*)m_req_info.peer_id.GetRawData(), 20);

		NetSpace::InetAddress iadr;
		iadr.SetAddr(0);
		iadr.SetPort(m_req_info.listen_port);

		std::string event_str;
		switch(m_req_info.event)
		{
		case TS_START:
				event_str = "Started";
				break;
		case TS_COMPLETED:
				event_str = "Completed";
				break;
		case TS_STOP:
				event_str = "Stopped";
				break;
		default:
				event_str = "None";
				break;
		}

		char buf[10240];

		int n = snprintf( 
				buf,
				10240, 
				REQ_URL_FMT,
				m_path.c_str(),
				infohash.c_str(),
				peer_id.c_str(),
				iadr.Port(),
				m_req_info.uploaded,
				m_req_info.downloaded,
				m_req_info.left,
				m_req_info.key,
				m_req_info.compact,
				m_req_info.numwant,
				event_str.c_str()
				);
		buf[n + 1] = '\0';
		return std::string(buf);
}

#undef REQ_URL_FMT













HttpTracker::HttpTracker(const std::string &url, 
					t_uint16 port, 
					const std::string &path,
					const RequestInfo &req_info,
					ResponseInfo &return_info
					)
					: m_url(url)
					, m_port(port)
					, m_path(path)
					, m_req_info(req_info)
					, m_return_info(return_info)
{
		m_req_url_info = build_request_url();

		//DEBUG_PRINT1("m_req_url_info == %s\n", m_req_url_info.c_str());


}

HttpTracker::~HttpTracker()
{




}


bool HttpTracker::resolve_dns()
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

		return m_sock.Open();
}


#define MAX_RECV_BUF_SIZE 10240

bool HttpTracker::request()
{
		const t_uint64 timeout = TCP_WAIT_TIMEOUT;
		if(!NetSpace::Connector::Connect(m_sock, m_addr, &timeout)) return false;
		
		
		if(!m_sock.Enable(NetSpace::NET_NONBLOCK)) return false;

		size_t bt = 0;

		int res = m_sock.Send((const t_byte*)m_req_url_info.c_str(), 
								m_req_url_info.size(), 
								&bt, 
								&timeout, 
								0);

		if(res != m_req_url_info.size()) return false;


		bt = 0;
		std::vector<t_byte> buf(MAX_RECV_BUF_SIZE);

		res  = m_sock.Recv(&buf[0], MAX_RECV_BUF_SIZE, &bt, &timeout, 0);
		
		m_recv_ss.str("");
		m_recv_ss.write((const char*)&buf[0], bt);
		return true;

}


















#define CHECK_CONDITION(cond, failed_val)						\
		do{														\
				if(!(cond))										\
				{												\
						m_return_info.result = failed_val;		\
						return;									\
				}												\
		}while(0)
						




void HttpTracker::Run()throw()
{
RE_REQUEST:
		CHECK_CONDITION(resolve_dns(), TS_RES_DNS_ERROR);

		CHECK_CONDITION(request(), TS_RES_CONN_ERROR);
		
		std::istringstream istr(m_recv_ss.str());
		m_recv_ss.str("");

		HttpResponse hpr;
		
		CHECK_CONDITION(hpr.Parse(istr), TS_RES_REPLY_ERROR);

		
		int ret_code = ::atoi(hpr.Status().c_str());

		if(ret_code == 200)
		{
				DEBUG_PRINT0("HttpTracker::Run : on http request ok\n");
				std::vector<t_byte> content;
				content.reserve(10240);
				
				while(!istr.eof())
				{
						content.push_back((t_byte)istr.get());
				}

				CHECK_CONDITION(parse_response(content), TS_RES_REPLY_ERROR);
				m_return_info.result = TS_RES_SUCCESS;

		}else if(ret_code == 301 || ret_code == 302)
		{
				std::string new_addr = hpr.Value("Location");

				if(new_addr == "")
				{
						m_return_info.result = TS_RES_REPLY_ERROR;
				}else
				{
						m_sock.Close();
						m_addr.Clear();

						DEBUG_PRINT0("HttpTracker::Run : on url direct\n");

						CHECK_CONDITION(reset_new_url(new_addr), TS_RES_REPLY_ERROR);
						goto RE_REQUEST;
				}
		}else
		{
				m_return_info.result = TS_RES_REPLY_ERROR;
		}
}



bool HttpTracker::reset_new_url(const std::string &new_url)
{
		std::string url = new_url.substr(0, new_url.find("?"));

		if(url == "") return false;

		UrlParser upar;
		if(!upar.Parse(url)) return false;

		if(upar.type != UrlParser::HTTP) return false;

		m_url = upar.host;
		m_path = upar.path;
		m_port = upar.port;
		
		m_req_url_info = build_request_url();
		DEBUG_PRINT1("reset_new_url : new url == %s\n", url.c_str());
		return true;
}





static const std::string FAILURE_REASON			=		"failure reason";
static const std::string WARNING_MESSAGE		=		"warning message";
static const std::string INTERVAL				=		"interval";
static const std::string MIN_INTERVAL			=		"min interval";
static const std::string TRACKER_ID				=		"tracker id";
static const std::string COMPLETE				=		"complete";
static const std::string INCOMPLETE				=		"incomplete";
static const std::string PEERS					=		"peers";
static const std::string PEER_ID				=		"peer id";
static const std::string PORT					=		"port";
static const std::string IP						=		"ip";




bool HttpTracker::parse_response(const std::vector<t_byte> content)
{
		BencodingDecoder bdec;
		bdec.SetContent(content);

		DictType  dict;
		

		if(bdec.ReadDict(0, dict) == 0)
		{
				return false;
		}
		
		

		StringType		*pstr = 0;
		IntegerType		*pint = 0;
		ListType		*plist = 0;
		DictType		*pdict = 0;
		
		pstr = (StringType*)dict.get_value(FAILURE_REASON);
		
		if(pstr != 0)
		{
				return false;
		}
		
		t_uint64 interval = 0, completed = 0, incomplete = 0;

		pint = (IntegerType*)dict.get_value(INTERVAL);

		if(pint != 0)
		{
				interval = pint->get_int();
				pint = 0;
		}		

		pint = (IntegerType*)dict.get_value(COMPLETE);
		
		if(pint != 0)
		{
				completed = pint->get_int();
				pint = 0;
				
		}

		pint = (IntegerType*)dict.get_value(INCOMPLETE);

		if(pint != 0)
		{
				incomplete = pint->get_int();
				pint = 0;
		}

		TypeBase *pbase = dict.get_value(PEERS);
		
		if(pbase == 0)
		{
				return false;
		}

		PeerList peer_list;

		if(pbase->type() == STRING_TYPE)
		{
				
				DEBUG_PRINT0("pbase->type() == STRING_TYPE\n");
				
				pstr = (StringType*)pbase;
				
				if(pstr->size() < 6) return false;

				BufferT buf;
				t_byte *pbuf = buf.Allocate(pstr->size());
				
				pstr->get_str((char*)pbuf, pstr->size());
				
				while(buf.Size() >= 6)
				{
						PeerEntry pe;
						pe.addr = *(t_uint32*)buf.Data();
						buf.Erase(4);
						pe.port = ByteOrder::FromNetOrderToNativeOrder(*(t_uint16*)buf.Data());
						peer_list.push_back(pe);
				}

		}else if(pbase->type() == LIST_TYPE)
		{
				DEBUG_PRINT0("pbase->type() == LIST_TYPE\n");
				plist = (ListType*)pbase;
				
				for(ListType::ThisIterator it = plist->begin(); it != plist->end(); ++it)
				{
						PeerEntry pe;
						pdict = (DictType*)((*it));
						if(pdict == 0) continue;

						pint = (IntegerType*)pdict->get_value(PORT);
						
						if(pint == 0)
						{
								continue;
						}
						pe.port = pint->get_int();

						pstr = (StringType*)pdict->get_value(IP);
						if(pstr == 0)
						{
								continue;
						}
						
						pe.addr = ::inet_addr(pstr->get_str().c_str());
						peer_list.push_back(pe);
				}
		}else
		{
				return false;
		}
		
		if(peer_list.empty())
		{
				return false;
		}else
		{
				m_return_info.interval = interval;
				m_return_info.incompleted = incomplete;
				m_return_info.completed = completed;
				m_return_info.peers =peer_list;
				return true;
		}
}











}