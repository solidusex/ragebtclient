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
 */#include "NotifyEvent.h"


namespace Rage {


NotifyEvent::NotifyEvent(NotifyType type) : m_type(type)
{

}

NotifyEvent::~NotifyEvent()
{


}

NotifyType NotifyEvent::GetType()const
{
		return m_type;
}


/////////////////////////////UserEvent/////////////////////////

UserEventType UserEvent::GetUserEventType()const
{
		return m_user_event_type;
}

UserEvent::UserEvent(UserEventType user_event_type) : NotifyEvent(USER_EVENT), m_user_event_type(user_event_type)
{


}

UserEvent::~UserEvent()
{


}







AddPeersUserEvent::AddPeersUserEvent() : UserEvent(ADD_PEERS)
{

}


AddPeersUserEvent::~AddPeersUserEvent()
{

}


const std::list<PeerEntry>& AddPeersUserEvent::GetPeers()const
{
		return m_peers;
}
		
void AddPeersUserEvent::InsertPeer(const PeerEntry &peer)
{
		m_peers.push_back(peer);
}
		
void AddPeersUserEvent::RemovePeer(const PeerEntry &peer)
{
		m_peers.remove(peer);
}





//////////////////////////ReloadSetting//////////////////////////////////////

ReLoadSettingEvent::ReLoadSettingEvent() : UserEvent(RELOAD_SETTING)
{

}

ReLoadSettingEvent::~ReLoadSettingEvent()
{


}








//////////////////////////////////////////TimerEvent/////////////

TimerEvent::TimerEvent() : NotifyEvent(TIMER_EVENT)
{

}

TimerEvent::~TimerEvent()
{
		//DEBUG_PRINT0("TimerEvent::~TimerEvent()\n");

}


///////////////////////////////NetEvent/////////////////////////

NetEvent::NetEvent(NetEventType net_event_type) 
				: NotifyEvent(NET_EVENT)
				, m_net_event_type(net_event_type)
{


}
		
NetEvent::~NetEvent()
{

}

NetEventType NetEvent::GetNetEventType()const
{
		return m_net_event_type;
}





////////////////////////////////NetIOEvent////////////////////////////////////

NetIOEvent::NetIOEvent(const PeerEntry &peer_entry, const t_byte *pbuf, t_uint32 len) 
				: NetEvent(NET_IO_EVENT)
				, m_peer_entry(peer_entry)
				,m_buf(0)
				, m_len(0)
{
		if(len > 0)
		{
				m_buf = (t_byte*)malloc(len);
				memcpy(m_buf, pbuf, len);
				m_len = len;
		}
}

NetIOEvent::~NetIOEvent()
{
		//DEBUG_PRINT1("NetIOEvent::~NetIOEvent release buffer len == %d\n", m_len);
		if(m_len != 0)	//等于0的时候为keep alive消息
		{
				::free(m_buf);
		}
}

const t_byte* NetIOEvent::GetBuffer()const
{
		return m_buf;
}

t_uint32 NetIOEvent::GetBufferLength()const
{
		return m_len;
}




const PeerEntry& NetIOEvent::GetPeerEntry()const
{
		return m_peer_entry;

}


/////////////////////////NetConnEstablished/////////////////////////////////////////////

		
NetConnEstablished::NetConnEstablished(NetSpace::NetHandle	net_hdl,
										ConnMode			mode,
										const PeerID		&peer_id,
										const Sha1Hash		&infohash,
										const PeerEntry		&peer_entry,
										const t_byte			ext_info[8]
						   ) 
						   : NetEvent(NET_CONN_ESTABLISHED)
						   , m_net_hdl(net_hdl)
						   , m_mode(mode)
						   , m_peer_id(peer_id)
						   , m_infohash(infohash)
						   , m_peer_entry(peer_entry)
{
		assert(m_net_hdl != NET_INVALID_SOCKET);
		memset(m_ext_info, 0, 8);
		
		if(ext_info != 0)
		{
				memcpy(m_ext_info, ext_info, 8);
		}

}

NetConnEstablished::~NetConnEstablished()
{
		if(m_net_hdl != NET_INVALID_SOCKET)
		{
				//NetSpace::SockStream(m_net_hdl);	//close掉socket
				NetSpace::NetSock::CloseSocket(m_net_hdl);
		}
}

NetSpace::NetHandle NetConnEstablished::GetNetHandle()const
{
		return m_net_hdl;
}

ConnMode NetConnEstablished::GetConnMode()const
{
		return m_mode;
}

const PeerID& NetConnEstablished::GetPeerID()const
{
		return m_peer_id;
}

const Sha1Hash& NetConnEstablished::GetInfoHash()const
{
		return m_infohash;
}

const PeerEntry& NetConnEstablished::GetPeerEntry()const
{
		return m_peer_entry;
}

void NetConnEstablished::SetNetHandle(NetSpace::NetHandle net_hdl)
{
		m_net_hdl = net_hdl;

}

const t_byte* NetConnEstablished::GetExtInfo()const
{
		return m_ext_info;
}

///////////////////////////////NetConnAbort//////////////////////////////////////////////////

NetConnAbort::NetConnAbort(const PeerEntry &peer_entry) 
							: NetEvent(NET_CONN_ABORT)
							, m_peer_entry(peer_entry)
{


}

NetConnAbort::~NetConnAbort()
{

}

const PeerEntry& NetConnAbort::GetPeerEntry()const
{
		return m_peer_entry;
}


/////////////////////////////////NetConnectPeerFailed/////////////////////////////


NetConnectPeerFailed::NetConnectPeerFailed(const PeerEntry &pe) 
				: NetEvent(NET_CONNECT_REAMOTE_FAILED)
				, m_peer_entry(pe)
{


}

NetConnectPeerFailed::~NetConnectPeerFailed()
{


}

const PeerEntry& NetConnectPeerFailed::GetPeerEntry()const
{
		return m_peer_entry;
}






















}