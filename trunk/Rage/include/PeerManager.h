#pragma once

#include "TypeDef.h"
//#include "Status.h"
#include "Setting.h"
#include "Res.h"
#include "Service.h"
#include "Statistics.h"
#include "PeerLink.h"
#include "PieceManager.h"
#include "NotifyEvent.h"

namespace Rage {

class PeerLink;

#define MAX_PEER_FAILED_COUNT  10

#define PEER_CHOKE_TIMEOUT		10000
	
#define KEEP_ALIVE_TIMEOUT		90000

#define CHECK_PEER_TIMEOUT		1000

#define RESET_LOW_RATE_REQUEST_TIMEOUT  15000

#define MAX_ERROR_TIMES			10

#define DISCONNECT_PEERS_TIMEOUT	15000

struct PeerLinkInfo
{
		PeerEntry		peer_entry;
		PeerLink		*peer_link;
		t_uint32		failed_count;

		PeerLinkInfo();
		PeerLinkInfo(const PeerEntry &pe, PeerLink *plink);
		PeerLinkInfo(const PeerLinkInfo &other);
		PeerLinkInfo& operator=(const PeerLinkInfo &other);

};



class PeerManager : private NonCopyable
{
private:
		typedef std::map<PeerEntry, PeerLinkInfo>  PeerLinkMap;
		typedef TimeSpace::TimeStamp				TimeStampT;
private:
		ServiceManager			&m_serv_manager;
		ResourceManager			&m_res_manager;

		const GlobalSetting		&m_global_set;
private:
		PieceManager			&m_piece_manager;
private:		
		const TaskID			m_task_id;
		const PeerID			m_peer_id;
		const Sha1Hash			m_infohash;
private:
		PeerLinkMap				m_connected_peers;
		PeerLinkMap				m_connecting_peers;
		PeerLinkMap				m_unused_peers;
private:
		Statistics				m_stat;
		TimeStampT				m_check_choke_ts;
		TimeStampT				m_keep_alive_ts;
		TimeStampT				m_check_peers_ts;
		TimeStampT				m_disconnect_peers_ts;
		TimeStampT				m_reset_low_rate_req_ts;
private:
		void check_peers_choke();

		t_uint32 count_download_from_local_peers();

		void disconnect_one_peer();
		
		void check_need_disconnect_peers();

		bool find_disconnect_candidate(PeerEntry &pe);

		void check_peers();

		void reset_low_rate_request();

		void count_task_speed();

		void peer_link_second_tick();

		void send_keep_alive();

		void on_connection_abort(const PeerEntry &pe);

		void on_connect_failed(const PeerEntry &pe);

		void on_net_io_event(const PeerEntry &pe,const t_byte *pbuf, t_uint32 len);
		
		bool on_conn_established(const PeerInfo &peer_info);
private:
		void clear_connected_peers();

		void clear_unused_peers();

		void clear_conneting_peers();

		bool is_in_unused_list(const PeerEntry &pe)const;

		bool is_in_connected_list(const PeerEntry &pe)const;

		bool is_in_connecting_list(const PeerEntry &pe)const;

		bool peer_id_is_existed(const PeerID &peer_id)const;

		//void band_peer_addr(const PeerEntry &pe);

public:
		PeerManager(ServiceManager			&serv_manager,
					ResourceManager			&res_manager,
					const GlobalSetting		&global_set,
					PieceManager			&piece_manager,
					TaskID					task_id,
					const PeerID			&peer_id,
					const Sha1Hash			&infohash
					);
		~PeerManager();
public:
		//往m_un_unsed_peers中添加一个未链接的peer地址，无id
		void AddUnusedPeer(const PeerEntry &pe);

		void OnNetEvent(const NetEvent *pnet_event);
		
		void SecondTick();//每秒钟调用一次
public:
		void BroadCastHave(t_uint32 piece_idx);

		//void CanclePieceRequest(t_uint32 piece_idx);

		void OnDownloadCompleted();

public:
		void Clear();

public:
		const Statistics& GetTaskStatistics()const;
		t_uint32		  GetPeerCount()const;
		t_uint32		  GetSeedCount()const;
		t_uint32		  GetUnusedPeersCount()const;

		PeerList GetConnectedPeers()const;
		PeerList GetConnectingPeers()const;

		std::vector<PeerStatus> GetPeersStatus()const;
};




}