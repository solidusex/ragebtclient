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
//#include "PieceRequest.h"
#include "PeerEntry.h"
#include "Setting.h"
#include "Res.h"
#include "Statistics.h"
#include "PieceManager.h"

namespace Rage {


#define KEEPALIVE_TIMEOUT  240000


class PeerIO;

class Storage;

class PeerManager;

struct PeerInfo
{
		NetSpace::NetHandle		net_hdl;
		PeerID					peer_id;
		Sha1Hash				infohash;
		PeerEntry				peer_entry;
		ConnMode				mode;
		t_byte					ext_info[8];
};

struct PeerStatus
{
		PeerEntry		peer_entry;
		float			down_rate;
		float			up_rate;
		t_uint64		total_down;
		t_uint64		total_up;
		float			finished_degree;
		ConnMode		mode;
};


class PeerLink : private NonCopyable
{
private:
		typedef std::list<SliceInfo> PeerRequestList;
		typedef std::list<SliceInfo> LocalRequest;

		typedef TimeSpace::TimeStamp	TimeStampT;
private:
		PeerIO					*m_peer_io;
private:
		LocalRequest			m_local_req_list;//本地
		PeerRequestList			m_remote_req_list;
		
		BitField				m_remote_own_pieces;

		const PeerID			m_remote_peer_id;
		const PeerEntry			m_peer_entry;
		t_byte					m_ext_info[8];
private:
		bool					m_local_choking_remote;
		bool					m_local_interested_remote;
		bool					m_remote_choking_local; 
		bool					m_remote_interested_local;

private:
		bool					m_received_bitfield;
private:
		Statistics				m_stat;
		TimeStampT				m_last_recv_time_stamp;

		TimeStampT				m_connected_ts;
private:
		PieceManager			&m_piece_manager;
		PeerManager				&m_manager;
		
		const GlobalSetting		&m_global_setting;
		ResourceManager			&m_res_manager;
private:
		const ConnMode			m_mode;

public:
		PeerLink(const PeerInfo &peer_info, 
				TaskID task_uni_id,
				PieceManager &piece_manager,
				PeerManager &peer_manager, 
				const GlobalSetting &g_setting,
				ResourceManager	&res_manager
				);

		~PeerLink();
public:
		const PeerID& GetPeerID()const;

		const Statistics& GetStatistics()const;

		bool  IsSeed()const;
public:
		const PeerEntry& GetPeerEntry()const;
		
		const BitField& GetPeerOwnBitField()const;

		ConnMode GetConnectionMode()const;

		PeerIO* GetPeerIO();


private:
		/*根据下载流量控制发送m_request中的请求，
			根据上传流量控制回复m_peer_req_list中的请求
			此函数会调用storage相关操作读写磁盘
		*/
		
		void send_bitfield();
		
		void send_choke(bool choke);
		
		void send_interested(bool interested);
		
		void send_have(t_uint32 piece_idx);
		
		void send_request(t_uint32 piece_idx, t_uint32 offset, t_uint32 len);

		void send_piece_data(t_uint32 piece_idx, t_uint32 offset, t_uint32 len, const t_byte *pbuf);

		void send_cancel(t_uint32 piece_idx, t_uint32 offset, t_uint32 len);

		void send_keep_alive();

private:
		//所有OnXXXX如果返回false,则此peerlink都需要被close,代表协议错误

		bool on_bitfield(const t_byte *pbuf, t_uint32 len);

		bool on_choke(const t_byte *pbuf, t_uint32 len);
		
		bool on_unchoke(const t_byte *pbuf, t_uint32 len);
		
		bool on_interested(const t_byte *pbuf, t_uint32 len);
		
		bool on_not_interested(const t_byte *pbuf, t_uint32 len);
		
		bool on_have(const t_byte *pbuf, t_uint32 len);

		bool on_request(const t_byte *pbuf, t_uint32 len);

		bool on_cancel(const t_byte *pbuf, t_uint32 len);
		
		bool on_piece(const t_byte *pbuf, t_uint32 len);

private:
		void do_pieces_request();

		void do_pieces_send();

		//void elect_new_piece_task();
public:
		bool IsChokedRemote()const;
		
		bool IsInterestedRemote()const;

		bool RemoteIsChokedLocal()const;

		bool RemoteIsInterestedLocal()const;
		
		bool OnNetPackage(const t_byte *pbuf, t_uint32 len);

		void NotifyHavePiece(t_uint32 piece_idx);

		//void CancelPieceRequest(t_uint32 piece_idx);
		
		void ChokePeer(bool choke);
		
		void OnDownloadCompleted();
		
		void SecondTick();

		bool IsNeedClose();

		void SendKeepAlive();

		t_uint64 GetConnectedTime()const;//秒

		PeerStatus GetPeerStatus()const;

		void AbortAllRequest();
};











}
















/*private:
		struct PeerRequest
		{
				t_uint32		piece_idx;
				t_uint32		offset;
				t_uint32		len;
		};
		typedef std::list<PeerRequest> PeerRequestList;
			//	PeerEntry				m_entry;
		
	//	PieceRequest			m_request;
	//	PeerRequestList			m_peer_req_list;*/