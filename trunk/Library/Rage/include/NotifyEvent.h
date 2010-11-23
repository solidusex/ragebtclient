
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
#pragma once

#include "TypeDef.h"
#include "PeerEntry.h"


namespace Rage {


enum NotifyType
{
		NET_EVENT = 0x000A, 
		TIMER_EVENT  = 0x000B,
		USER_EVENT = 0x000C
};


////////////////////////////////////BaseEvent/////////////////////////////////////////

class NotifyEvent : public MsgSpace::Message
{
private:
		NotifyType				m_type;
public:
		NotifyEvent(NotifyType type);

		virtual ~NotifyEvent();
public:
		NotifyType GetType()const;
};


//////////////////////////////UserEvent////////////////////////////////

enum  UserEventType
{
		WakeUpEvent		= 0x200A,
		ADD_PEERS		= 0x200B,
		RELOAD_SETTING	= 0x200C
};


class UserEvent : public NotifyEvent
{
private:
		UserEventType	m_user_event_type;
public:
		UserEvent(UserEventType user_event_type);

		virtual ~UserEvent();
public:
		UserEventType GetUserEventType()const;
};



class  AddPeersUserEvent : public UserEvent
{
private:
		std::list<PeerEntry>	m_peers;
public:
		AddPeersUserEvent();

		virtual ~AddPeersUserEvent();

		const std::list<PeerEntry>& GetPeers()const;
		
		void InsertPeer(const PeerEntry &peer);
		
		void RemovePeer(const PeerEntry &peer);
};


////////////////////////ReloadSetting//////////////////////////
class ReLoadSettingEvent : public UserEvent
{
public:
		ReLoadSettingEvent();
		virtual ~ReLoadSettingEvent();
};






//////////////////////////////TimerEvent//////////////////////

class  TimerEvent : public NotifyEvent
{
public:
		TimerEvent();

		virtual ~TimerEvent();
};



//////////////////////////////NetEvent/////////////////////////////////////////

enum NetEventType
{
		NET_IO_EVENT						= 0xA001,
		NET_CONN_ESTABLISHED				= 0xA002,
		NET_CONN_ABORT						= 0xA003,
		NET_CONNECT_REAMOTE_FAILED			= 0xA004
};


class  NetEvent : public NotifyEvent
{
private:
		NetEventType	m_net_event_type;
public:
		NetEvent(NetEventType net_event_type);
		
		virtual ~NetEvent();

		NetEventType GetNetEventType()const;
};


class  NetIOEvent : public NetEvent
{
private:
		PeerEntry		m_peer_entry;
		t_byte			*m_buf;
		t_uint32		m_len;
public:
		NetIOEvent(const PeerEntry &peer_entry, const t_byte *pbuf, t_uint32 len);

		virtual ~NetIOEvent();
		
		const PeerEntry& GetPeerEntry()const;

		const t_byte* GetBuffer()const;

		t_uint32 GetBufferLength()const;
};


////////////////////////////////////////NetConnEstablished//////////////////////////

class NetConnEstablished : public NetEvent
{
private:
		Sha1Hash				m_infohash;
		NetSpace::NetHandle		m_net_hdl;
		PeerID					m_peer_id;
		PeerEntry				m_peer_entry;
		ConnMode				m_mode;
		t_byte					m_ext_info[8];
public:
		NetConnEstablished(NetSpace::NetHandle	m_net_hdl,
							ConnMode				m_mode,
						   const PeerID			&m_peer_id,
						   const Sha1Hash		&m_infohash,
						   const PeerEntry		&m_peer_entry,
						   const t_byte			ext_info[8]
						   );

		virtual ~NetConnEstablished();
		
		NetSpace::NetHandle GetNetHandle()const;
		ConnMode GetConnMode()const;
		const PeerID& GetPeerID()const;//remote peer id
		const Sha1Hash& GetInfoHash()const;
		const PeerEntry& GetPeerEntry()const;
		const t_byte* GetExtInfo()const;
public:
		void SetNetHandle(NetSpace::NetHandle net_hdl = NET_INVALID_SOCKET);
};

/////////////////////////////////NetConnAbort//////////////////////////////

class NetConnAbort : public NetEvent
{
private:
		PeerEntry		m_peer_entry;
public:
		NetConnAbort(const PeerEntry &peer_entry);
		virtual ~NetConnAbort();

		const PeerEntry& GetPeerEntry()const;
};


///////////////////////////////////NetConnectPeerFailed////////////////////////////////

class NetConnectPeerFailed : public NetEvent
{
private:
		PeerEntry		m_peer_entry;
public:
		NetConnectPeerFailed(const PeerEntry &pe);
		virtual ~NetConnectPeerFailed();

		const PeerEntry& GetPeerEntry()const;
};


}

















//////////////////////////////TrackerEvent/////////////////////////////////////////



#if(0)
//////////////////////////////TrackerEvent/////////////////////////////////////////

enum TrackerReqState
{
		TRACKER_WARNING = 0x000A,
		TRACKER_SUCCESS = 0x000B,
		TRACKER_TIMEOUT = 0x000C,
		TRACKER_REDIRECT = 0x000D,
		TRACKER_ERROR    = 0x000E
};


class RageAPI TrackerEvent : public NotifyEvent
{
private:
		std::string				m_url;
		Sha1Hash				m_infohash;
		TrackerReqState			m_state;
		
		PeerList				m_peers;			//用户列表
		t_uint32				m_interval;		//距离下一次query的间隔
		t_uint32				m_completed;		//seed
		t_uint32				m_incompleted;		//正在下载的人

		std::string				m_msg;
		std::string				m_redirect_url;
public:
		void SetUrl(const std::string &url)
		{
				m_url = url;
		}

		void SetInfohash(const Sha1Hash &infohash)
		{
				m_infohash = infohash;
		}

		void SetReqState(TrackerReqState state)
		{
				m_state = state;
		}

		void SetPeerList(const PeerList &peers)
		{
				m_peers = peers;
		}

		void SetWarningMsg(const std::string &msg)
		{
				m_msg = msg;
		}

		void SetRedirectUrl(const std::string &redirect_url)
		{
				m_redirect_url = redirect_url;
		}

		void SetInterval(t_uint32 interval)
		{
				m_interval = interval;
		}

		void SetCompleted(t_uint32 completed)
		{
				m_completed = completed;
		}

		void SetInCompleted(t_uint32 incompleted)
		{
				m_incompleted = incompleted;
		}


		const std::string&		GetUrl()const
		{
				return m_url;
		}

		const Sha1Hash&			GetInfohash()const
		{
				return m_infohash;
		}

		TrackerReqState			GetState()const
		{
				return m_state;
		}

		const PeerList&			GetPeerList()const
		{
				return m_peers;
		}

		const std::string&		GetWarningMsg()const
		{
				return m_msg;
		}

		const std::string&		GetRedirectUrl()const
		{
				return m_redirect_url;
		}

		t_uint32				GetInterval()const
		{
				return m_interval;
		}

		t_uint32				GetCompleted()const
		{
				return m_completed;
		}

		t_uint32				GetInCompleted()const
		{
				return m_incompleted;
		}

public:
		TrackerEvent() : NotifyEvent(TRACKER_EVENT)
						, m_url("")
						, m_infohash(Sha1Hash())
						, m_state(TRACKER_ERROR)
						, m_peers(PeerList())
						, m_interval(0)
						, m_completed(0)
						, m_incompleted(0)
						, m_msg("")
						, m_redirect_url("")
		{


		}

		virtual ~TrackerEvent()
		{


		}
};




#endif
