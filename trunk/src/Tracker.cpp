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
 */#include "Tracker.h"
#include "HttpTracker.h"
#include "UDPTracker.h"
#include "URL.h"
#include "PeerManager.h"

namespace Rage{



///////////////////////////TrackerRequest////////////////////////////////////////
TrackerRequest::TrackerRequest(const std::string &url, const RequestInfo &req_info)
				: m_req_info(req_info)
				, m_url(url)
				, m_is_done(false)//m_event(false)
{




}


bool TrackerRequest::IsDone()const
{
		//return m_event.Wait(0);
		return m_is_done;
}

const ResponseInfo& TrackerRequest::GetResponse()const
{
		return m_response_info;
}

TrackerRequest::~TrackerRequest()
{
		DEBUG_PRINT0("TrackerRequest::~TrackerRequest()\n");
}

void TrackerRequest::Run()throw()
{

		UrlParser url_parser;		

		url_parser.Parse(m_url);
		
		switch(url_parser.type)
		{
		case UrlParser::HTTP:
		{
				std::auto_ptr<HttpTracker> phttp(new HttpTracker(url_parser.host, url_parser.port, url_parser.path, m_req_info, m_response_info));
				//HttpTracker *phttp = new HttpTracker(url_parser.host, url_parser.port, url_parser.path, m_req_info, m_response_info);
				phttp->Run();
				break;
		}
		case UrlParser::UDP:
		{
				std::auto_ptr<UDPTracker> pudp(new UDPTracker(url_parser.host, url_parser.port, m_req_info, m_response_info));
				//UDPTracker *pudp = new UDPTracker(url_parser.host, url_parser.port, m_req_info, m_response_info);
				pudp->Run();
				break;
		}
		default:
				assert(0);
		}
		
		//m_event.Set();
		m_is_done = true;

}


////////////////////////////Tracker///////////////////////////////////


Tracker::Tracker(const std::string &url,ServiceManager &serv_manager, PeerManager &peer_manager)
				: m_url(url)
				, m_state(TS_FREE)
				, m_interval(0)
				, m_request(0)
				, m_serv_manager(serv_manager)
				, m_peer_manager(peer_manager)
				, m_failed_count(0)
				, m_tracker_status("UnRequest")
{



}


Tracker::~Tracker()
{
		if(m_state == TS_REQUESTING)
		{
				assert(m_request != 0);
				m_request->Release();
		}
}

t_uint32 Tracker::GetFailedCount()const
{
		return m_failed_count;
}


#define REQUESTING_INTERVAL  300

void Tracker::SecondTick(const RequestInfo &req_info)
{
		if(m_state == TS_FREE && m_ts.ElapsedSecond() >= m_interval)
		{
				assert(m_request == 0);
				
				m_request = new TrackerRequest(m_url, req_info);

				m_serv_manager.GetAsyncService().PostRequest(m_request);
				m_state = TS_REQUESTING;
				return;
		}

		if(m_state == TS_REQUESTING)
		{
				assert(m_request != 0);
				if(m_request->IsDone())
				{
						handle_response(m_request->GetResponse());
						m_request->Release();
						m_request = 0;
						m_state = TS_FREE;
						m_ts.Update();
						m_interval = REQUESTING_INTERVAL;
				}else
				{


				}
		}
}

/*
		TS_RES_SUCCESS			= 0x500A,
		TS_RES_DNS_ERROR		= 0x500B,
		TS_RES_CONN_ERROR		= 0x500C,
		TS_RES_REPLY_ERROR		= 0x500D,
		TS_RES_REPLY_WARNING	= 0x500E,
		TS_RES_UNKNOW_ERROR		= 0x500F
		*/

#define ASSIGN_TO_STATUS_STR(format, param)		\
		do{										\
				char buf[1024];					\
				sprintf(buf, format, param);	\
				m_tracker_status = buf;			\
		}while(0)

void Tracker::handle_response(const ResponseInfo &response)
{
		switch(response.result)
		{
		case TS_RES_SUCCESS:
		{
				for(PeerList::const_iterator it = response.peers.begin(); it != response.peers.end(); ++it)
				{
						m_peer_manager.AddUnusedPeer(*it);
				}
				ASSIGN_TO_STATUS_STR("Tracker Response Success : Return Peers == %d", response.peers.size());
				
				break;
		}
		case TS_RES_DNS_ERROR:
		{
				m_failed_count++;
				
				ASSIGN_TO_STATUS_STR("Tracker Response Failed : Error == %s", "DNS_ERROR");

				break;
		}
		case TS_RES_CONN_ERROR:
		{
				m_failed_count++;

				ASSIGN_TO_STATUS_STR("Tracker Response Failed : Error == %s", "Connect Failed");
				
				break;
		}
		case TS_RES_REPLY_ERROR:
		{
				m_failed_count++;
				ASSIGN_TO_STATUS_STR("Tracker Response Failed : Error == %s", "Tracker Response Illegle");

				break;
		}
		case TS_RES_REPLY_WARNING:
		{
				m_failed_count++;

				ASSIGN_TO_STATUS_STR("Tracker Response Failed : Error == %s", "Tracker Response Warning");

				break;
		}
		case TS_RES_UNKNOW_ERROR:
		{
				m_failed_count++;
				ASSIGN_TO_STATUS_STR("Tracker Response Failed : Error == %s", "UnKnow Error");
				break;
		}
		default:
		{
				m_tracker_status = "";
				m_failed_count++;
				break;
		}
		}

}


std::string  Tracker::GetTrackerStatus()const
{
		return m_tracker_status;		

}


/*
void Tracker::handle_response(const ResponseInfo &response)
{
		if(response.result == TS_RES_SUCCESS)
		{
				printf("response interval == %d\n", response.interval);
				printf("response completed == %d\n", response.completed);
				printf("response incompleted == %d\n", response.incompleted);
				printf("response peers == %d\n", response.peers.size());
				
				ResponseInfo res_info = response;
				printf("aaaaaaaaaaaaaaa\n");
				res_info.peers.sort();
				//std::sort(res_info.peers.begin(), res_info.peers.end());
				printf("xxxxxxxxxxxxxx\n");

				for(PeerList::const_iterator it = res_info.peers.begin(); it != res_info.peers.end(); ++it)
				{
						NetSpace::InetAddress addr;
						addr.SetAddr(it->addr);
						addr.SetPort(it->port);

						printf("addr == %s:%d\n", addr.Str().c_str(), addr.Port());
				}
		}else
		{
				switch(response.result)
				{
				case TS_RES_DNS_ERROR:
						printf("resolve dns failed\n");
						break;
				case TS_RES_CONN_ERROR:
						printf("connection failed\n");
						break;
				case TS_RES_REPLY_ERROR:
						printf("tracker reply failed\n");
						break;
				case TS_RES_REPLY_WARNING:
						printf("tracker reply warning msg == %s\n", response.warning.c_str());
						break;
				default:
						printf("unknow error\n");
						break;
				}
		}
}*/







}