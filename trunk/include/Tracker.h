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
#include "Sha1Hash.h"
#include "PeerEntry.h"
#include "Service.h"
#include "Setting.h"


namespace Rage{

enum TrackerState
{
		TS_FREE				= 0x300A,
		TS_REQUESTING		= 0x300B,
};


enum TrackerEvent
{
		TS_NONE			= 0,
		TS_COMPLETED	= 1,
		TS_START		= 2,
		TS_STOP			= 3
};


struct RequestInfo
{
		//std::string		url;
		Sha1Hash		info_hash;	//����ʱ����Կ������������ĸ�task��
		PeerID			peer_id;	
		t_uint16		listen_port;	//��client�ļ����˿ڣ�

		t_uint64		downloaded;		//����������
		t_uint64		uploaded;		//�����ϴ���
		t_uint64		left;			//����ʣ������
		TrackerEvent	event;
		t_uint32		compact;		//�Ƿ�Ϊcompactģʽ
		t_uint32		numwant;		//ϣ���ܷ��ؼ�����
		t_uint32		key;			//transaction key
};


enum ResponseResult
{
		TS_RES_SUCCESS			= 0x500A,
		TS_RES_DNS_ERROR		= 0x500B,
		TS_RES_CONN_ERROR		= 0x500C,
		TS_RES_REPLY_ERROR		= 0x500D,
		TS_RES_REPLY_WARNING	= 0x500E,
		TS_RES_UNKNOW_ERROR		= 0x500F
};

struct ResponseInfo
{
		ResponseResult	result;

		PeerList		peers;
		t_uint32		interval;		//������һ��query�ļ��
		t_uint32		completed;		//seed
		t_uint32		incompleted;		//�������ص���
		std::string		warning;
		ResponseInfo() 
				: result(TS_RES_UNKNOW_ERROR)
				, interval(0)
				, completed(0)
				, incompleted(0)
		{


		}
};

///////////////////////TrackerRequest//////////////////////////////////

class TrackerRequest : public AsyncRequest
{
private:
		typedef ThreadSpace::Event		EventT;
private:
		//mutable EventT			m_event;
private:
		const std::string		m_url;
		RequestInfo				m_req_info;
		ResponseInfo			m_response_info;
		bool					m_is_done;
public:
		TrackerRequest(const std::string &url, const RequestInfo &req_info);

		virtual ~TrackerRequest();
public:
		virtual void Run()throw();
		
		const ResponseInfo& GetResponse()const;

		bool IsDone()const;
};





////////////////////////////////Tracker////////////////////////////////////////


class PeerManager;


class Tracker : NonCopyable
{
private:
		typedef TimeSpace::TimeStamp	TimeStampT;
private:
		std::string				m_url;
		TimeStampT				m_ts;
		TrackerState			m_state;
		t_uint64				m_interval;
		t_uint32				m_failed_count;

		std::string				m_tracker_status;
private:
		ServiceManager			&m_serv_manager;
		PeerManager				&m_peer_manager;
private:
		TrackerRequest			*m_request;
public:
		Tracker(const std::string &url,ServiceManager &serv_manager, PeerManager &peer_manager);

		~Tracker();
public:
		void SecondTick(const RequestInfo &req_info);
		
		t_uint32 GetFailedCount()const;
		
		std::string  GetTrackerStatus()const;
		
private:
		void handle_response(const ResponseInfo &response);
};









}