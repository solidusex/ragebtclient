#pragma once

#include "TypeDef.h"
#include "NotifyEvent.h"
#include "Res.h"

namespace Rage {

//////////////////////////////////Validator/////////////////////////////////


#define VALIDATOR_TIMEOUT  10000	//10√Î÷”

#define CONNECT_REMOTE_TIMEOUT  20000 //∫¡√Î


class PeerConnector : public NetSpace::ConnectorEx
{
private:
		const TaskID					m_task_id;
		const PeerID					m_peer_id;
		const Sha1Hash					m_infohash;
		PeerEntry						m_peer_entry;
public:
		PeerConnector(const NetSpace::InetAddress &addr, const TaskID &task_id, const Sha1Hash &infohash, const PeerID &peer_id);
		virtual ~PeerConnector();

protected:
		virtual void on_connected(NetSpace::NetHandle hdl);

		virtual void on_timeout();

		virtual void on_error(t_ulong error_code);
};


class OutConnValidator : public NetSpace::NetEventHandler
{
private:
		typedef DSSpace::SimpleBuffer			BufferT;
private:
		NetSpace::SockStream			m_stream_sock;
		const TaskID					m_task_id;
		const Sha1Hash					m_infohash;
		const PeerID					m_peer_id;//local peer_id
		BufferT							m_buf;
		TimeSpace::TimeStamp			m_ts;
		PeerEntry						m_peer_entry;
private:
		void check_connection_handshake();
public:
		OutConnValidator(NetSpace::NetHandle net_hdl, const PeerEntry &pe, const TaskID &task_id, const Sha1Hash	&info_hash, const PeerID &peer_id);
		
		virtual ~OutConnValidator();
		
public:
		virtual NetSpace::NetHandle GetHandle()const;
		
		virtual void OnInput();

		virtual void OnOutput();
		
		virtual void OnTimer();
};



class InConnValidator : public NetSpace::NetEventHandler
{
private:
		typedef DSSpace::SimpleBuffer			BufferT;
private:
		ResourceManager					&m_res_manager;
		NetSpace::SockStream			m_stream_sock;
		TimeSpace::TimeStamp			m_ts;
		BufferT							m_buf;
		PeerEntry						m_peer_entry;
		t_byte							m_ext_info[8];
private:
		TaskID					m_task_id;
		Sha1Hash				m_infohash;
		PeerID					m_peer_id;//local_peer_id
private:
		bool check_connection_handshake();
public:
		InConnValidator(ResourceManager &res_manager,NetSpace::NetHandle net_hdl);
		
		virtual ~InConnValidator();
public:
		virtual NetSpace::NetHandle GetHandle()const;
		
		virtual void OnInput();

		virtual void OnOutput();

		//virtual void OnException();

		virtual void OnTimer();
};
































}