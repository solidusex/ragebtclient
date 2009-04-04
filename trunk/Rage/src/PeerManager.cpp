#include "PeerManager.h"
#include "ConnValidator.h"
#include "PeerIO.h"

namespace Rage {



PeerLinkInfo::PeerLinkInfo() : peer_link(0), failed_count(0)
{

}

PeerLinkInfo::PeerLinkInfo(const PeerEntry &pe, PeerLink *plink) : peer_entry(pe), peer_link(plink), failed_count(0)
{

}

PeerLinkInfo::PeerLinkInfo(const PeerLinkInfo &other)
{
				*this = other;
}

PeerLinkInfo& PeerLinkInfo::operator=(const PeerLinkInfo &other)
{
		if(this != &other)
		{
				peer_entry = other.peer_entry;
				peer_link = other.peer_link;
				failed_count = other.failed_count;
		}
		return *this;
}



///////////////////////////////////////////////////////////////////////////////////

PeerManager::PeerManager(ServiceManager			&serv_manager,
					ResourceManager	&res_manager,
					const GlobalSetting		&global_set,
					PieceManager			&piece_manager,
					TaskID					task_id,
					const PeerID			&peer_id,
					const Sha1Hash			&infohash
					)
					: m_serv_manager(serv_manager)
					, m_res_manager(res_manager)
					, m_global_set(global_set)
					, m_piece_manager(piece_manager)
					, m_task_id(task_id)
					, m_peer_id(peer_id)
					, m_infohash(infohash)
{


}

PeerManager::~PeerManager()
{
		Clear();
}

void PeerManager::Clear()
{
		clear_connected_peers();
		clear_unused_peers();
		clear_conneting_peers();

}

void PeerManager::clear_connected_peers()
{
		for(PeerLinkMap::iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				assert(it->second.peer_link != 0);
				m_serv_manager.GetNetService().RemoveHandler(it->second.peer_link->GetPeerIO());
				delete it->second.peer_link;
		}

		m_connected_peers.clear();
}


void PeerManager::clear_unused_peers()
{
		m_connecting_peers.clear();

}


void PeerManager::clear_conneting_peers()
{
		m_unused_peers.clear();
}



t_uint32 PeerManager::count_download_from_local_peers()
{
		t_uint32 count = 0;
		for(PeerLinkMap::iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				PeerLink *peer = it->second.peer_link;
				assert(peer != 0);
				if(!peer->RemoteIsInterestedLocal() && !peer->IsChokedRemote())
				{
						peer->ChokePeer(true);
				}else
				{
						if(!peer->IsChokedRemote())
						{
								++count;
						}
				}
		}
		return count;
}


void PeerManager::check_peers_choke()
{
		t_uint32 down_count = count_download_from_local_peers();
		
		const t_uint32 max_upload_peers = m_global_set.GetMaxUploadFromLocalPeerCount();
		while(down_count < max_upload_peers)
		{
				PeerLinkMap::iterator iter  = m_connected_peers.begin();
				PeerLinkMap::iterator iter2 = m_connected_peers.end();
				
//找出我从他那下载速度最快,对我感兴趣并且被我choke了的，给其发送unchoke消息
				for(;iter != m_connected_peers.end(); ++iter)
				{
						PeerLink* peer = iter->second.peer_link;
						assert(peer != 0);

						if(peer->IsChokedRemote() && peer->RemoteIsInterestedLocal())
						{
								if(iter2 == m_connected_peers.end())
								{
										iter2 = iter;
								}else
								{
										PeerLink *peer2 = iter2->second.peer_link;
										assert(peer2 != 0);
										
										const Statistics &stat1 = peer->GetStatistics();
										const Statistics &stat2 = peer2->GetStatistics();

										if(stat1.DownloadPayloadRate() > stat2.DownloadPayloadRate())
										{
												iter2 = iter;
										}
								}
						}
				}

				if(iter2 != m_connected_peers.end())
				{
						iter2->second.peer_link->ChokePeer(false);
						down_count++;
				}
				else
				{
						break;
				}
		}

		if(down_count >= max_upload_peers)
		{
				
				//找未被我choke的，上传给我的数据最慢的一个peer
				PeerLinkMap::iterator worst_peer = m_connected_peers.end();

				for(PeerLinkMap::iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
				{
						PeerLink *peer = it->second.peer_link;
						assert(peer != 0);
						//首先是我没choke它，其次是他得对我有兴趣
						if(!peer->IsChokedRemote() && peer->RemoteIsInterestedLocal())
						{
								if(worst_peer == m_connected_peers.end())
								{
										worst_peer = it;
								}else
								{
										PeerLink *worst_peer_link = worst_peer->second.peer_link;
										assert(worst_peer_link != 0);
										const Statistics &stat1 = peer->GetStatistics();
										const Statistics &stat2 = worst_peer_link->GetStatistics();

										if(stat1.DownloadPayloadRate() < stat2.DownloadPayloadRate())
										{
												worst_peer = it;
										}
								}
						}
				}


				//现在搜索我choke他了,但是他那下载的速度在被我choke的peer中是速度最快的
				PeerLinkMap::iterator best_peer = m_connected_peers.end();
				for(PeerLinkMap::iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
				{
						PeerLink *peer = it->second.peer_link;
						assert(peer != 0);

						if(peer->IsChokedRemote() && peer->RemoteIsChokedLocal())
						{
								if(best_peer == m_connected_peers.end())
								{
										best_peer = it;
								}else
								{
										PeerLink *best_peer_link = best_peer->second.peer_link;
										assert(best_peer_link != 0);
										const Statistics &stat1 = peer->GetStatistics();
										const Statistics &stat2 = best_peer_link->GetStatistics();

										if(stat1.DownloadPayloadRate() > stat2.DownloadPayloadRate())
										{
												best_peer = it;
										}
								}
						}
				}

				
				PeerLinkMap::iterator end_iter = m_connected_peers.end();

				if(worst_peer != end_iter && best_peer != end_iter)
				{
						const Statistics &stat1 = worst_peer->second.peer_link->GetStatistics();
						const Statistics &stat2 = best_peer->second.peer_link->GetStatistics();
						
						if(stat1.DownloadPayloadRate() < stat2.DownloadPayloadRate())
						{
								worst_peer->second.peer_link->ChokePeer(true);
								best_peer->second.peer_link->ChokePeer(false);
						}
				}
		}

}
/*
void PeerManager::band_peer_addr(const PeerEntry &pe)
{
		NetSpace::InetAddress addr;
		addr.SetAddr(pe.addr);
		addr.SetPort(pe.port);

		m_res_manager.GetIPFilter().BandAddress(addr.Str(), addr.Port());
}*/

#if defined(max)
#undef max
#endif


bool PeerManager::find_disconnect_candidate(PeerEntry &pe)
{
		bool res = false;

		
		double slowest_transfer_rate = std::numeric_limits<double>::max();

		
		for(PeerLinkMap::iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				assert(it->second.peer_link != 0);
				t_uint64 connected_time = it->second.peer_link->GetConnectedTime() ;
				t_uint64 total_download = it->second.peer_link->GetStatistics().TotalPayloadDownload();
				double transfer_rate = (double)total_download / (double)(connected_time + 1);
				
				if(transfer_rate < slowest_transfer_rate)
				{
						slowest_transfer_rate = transfer_rate;
						pe = it->second.peer_entry;
						res = true;
				}
		}

		return res;
}

void PeerManager::disconnect_one_peer()
{
		PeerEntry pe;
		if(find_disconnect_candidate(pe))
		{
				PeerLinkMap::iterator it = m_connected_peers.find(pe);
				assert(it != m_connected_peers.end());
				
				PeerLinkInfo plink_info = it->second;
				m_connected_peers.erase(it);
		
				ConnMode mode = plink_info.peer_link->GetConnectionMode();
				m_serv_manager.GetNetService().RemoveHandler(plink_info.peer_link->GetPeerIO());
				delete plink_info.peer_link;
				plink_info.peer_link = 0;

				if(mode == LOCAL)
				{
						m_unused_peers[pe] = plink_info;
				}
		}
}

void PeerManager::check_peers()
{
		size_t curr_peers_count = m_connected_peers.size() + m_connecting_peers.size();
		//DEBUG_PRINT1("Current peer connection count == %d\n", curr_peers_count);

		const size_t max_connected_count = m_global_set.GetMaxConnectionPerTask();
		
		if(curr_peers_count < max_connected_count)
		{
				size_t max_start_conn_per_call = 0;

				while(max_start_conn_per_call < 5 && curr_peers_count < max_connected_count && !m_unused_peers.empty())
				{
						PeerLinkInfo peer_link_info = m_unused_peers.begin()->second;
						m_unused_peers.erase(m_unused_peers.begin());
						
						assert(peer_link_info.peer_link == 0);

						if(m_res_manager.GetIPFilter().IsBand(peer_link_info.peer_entry))
						{
								DEBUG_PRINT0("PEER ENTRY IS BANDED\n");
								continue;
						}

						try{
								NetSpace::InetAddress addr;
								addr.SetAddr(peer_link_info.peer_entry.addr);
								addr.SetPort(peer_link_info.peer_entry.port);
								PeerConnector *pconn = new PeerConnector(addr, m_task_id, m_infohash, m_peer_id);
								m_connecting_peers[peer_link_info.peer_entry] = peer_link_info;
								
								m_serv_manager.GetNetService().RegisterHandler(pconn);
								pconn->Release();
								
								max_start_conn_per_call++;
								curr_peers_count++;
						}catch(...)
						{
								m_res_manager.GetIPFilter().BandAddress(peer_link_info.peer_entry);
								//band_peer_addr(peer_link_info.peer_entry);
						}
				}
		}
		
		PeerLinkMap::iterator it = m_connected_peers.begin();
		while(it != m_connected_peers.end())
		{
				PeerLink *peer = it->second.peer_link;
				assert(peer != 0);
				
				if(peer->IsNeedClose())
				{
						m_serv_manager.GetNetService().RemoveHandler(peer->GetPeerIO());
						delete peer;
						it = m_connected_peers.erase(it);
				}else
				{
						++it;
				}
		}
}


void PeerManager::check_need_disconnect_peers()
{
		const size_t max_connected_count = m_global_set.GetMaxConnectionPerTask();
		assert(max_connected_count >= 20);
		while(m_connected_peers.size() >= (max_connected_count - 20))
		{
				DEBUG_PRINT0("CLOSE SLOWEST PEER\n");
				disconnect_one_peer();
		}

}


void PeerManager::send_keep_alive()
{
		PeerLinkMap::iterator it = m_connected_peers.begin();
		while(it != m_connected_peers.end())
		{
				PeerLink *peer = it->second.peer_link;
				assert(peer != 0);
				peer->SendKeepAlive();
				++it;
		}

}


#if(0)
void PeerManager::peer_link_second_tick()
{

		for(PeerLinkMap::iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				assert(it->second.peer_link != 0);

				m_stat += it->second.peer_link->GetStatistics();
				it->second.peer_link->SecondTick();
		}

		m_stat.SecondTick();
}

#endif


void PeerManager::peer_link_second_tick()
{
		std::vector<PeerLink*> peer_list;

		peer_list.reserve(m_connected_peers.size());

		for(PeerLinkMap::iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				assert(it->second.peer_link != 0);
				peer_list.push_back(it->second.peer_link);
		}
		
		srand(time(0));
		std::random_shuffle(peer_list.begin(), peer_list.end());
		
		size_t n = peer_list.size();
		for(size_t i = 0; i < n; ++i)
		{
				PeerLink *plink = peer_list[i];
				m_stat += plink->GetStatistics();
				plink->SecondTick();
		}
		m_stat.SecondTick();
}



void PeerManager::reset_low_rate_request()
{
		for(PeerLinkMap::iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				if(it->second.peer_link->GetStatistics().DownloadPayloadRate() <= 0.01f)
				{
						it->second.peer_link->AbortAllRequest();
				}
		}


}



void PeerManager::SecondTick()
{
		peer_link_second_tick();
		
		if(m_check_peers_ts.ElapsedMillisecond() >= CHECK_PEER_TIMEOUT)
		{
				check_peers();
				m_check_peers_ts.Update();
		}
		
		
		if(m_check_choke_ts.ElapsedMillisecond() >= PEER_CHOKE_TIMEOUT)
		{
				check_peers_choke();
				m_check_choke_ts.Update();
		}

		if(m_disconnect_peers_ts.ElapsedMillisecond() >= DISCONNECT_PEERS_TIMEOUT)
		{
				check_need_disconnect_peers();
				m_disconnect_peers_ts.Update();
		}


		if(m_keep_alive_ts.ElapsedMillisecond() >= KEEP_ALIVE_TIMEOUT)
		{
				send_keep_alive();
				m_keep_alive_ts.Update();
		}

		if(m_piece_manager.GetFinishedDegree() > 0.95f)
		{
				if(m_reset_low_rate_req_ts.ElapsedMillisecond() >= RESET_LOW_RATE_REQUEST_TIMEOUT)
				{
						reset_low_rate_request();
						m_reset_low_rate_req_ts.Update();
				}
		}
}


/////////////////////////////////event_handler////////////////////////////

bool PeerManager::is_in_unused_list(const PeerEntry &pe)const
{
		return (m_unused_peers.count(pe) == 1);
		//return (m_unused_peers.find(pe) != m_unused_peers.end());
}

bool PeerManager::is_in_connected_list(const PeerEntry &pe)const
{
		return (m_connected_peers.count(pe) == 1);
		//return (m_connected_peers.find(pe) != m_connected_peers.end());
}

bool PeerManager::is_in_connecting_list(const PeerEntry &pe)const
{
		return (m_connecting_peers.count(pe) == 1);
		//return (m_connecting_peers.find(pe) != m_connecting_peers.end());

}


void PeerManager::AddUnusedPeer(const PeerEntry &pe)
{
		if(is_in_unused_list(pe)) return;

		if(is_in_connecting_list(pe)) return;

		if(is_in_connected_list(pe)) return;

		m_unused_peers[pe] = PeerLinkInfo(pe, 0);
}




#if(0)
#endif

bool PeerManager::on_conn_established(const PeerInfo &peer_info)
{
		assert(peer_info.net_hdl != NET_INVALID_SOCKET);

		if(is_in_connected_list(peer_info.peer_entry))
		{
				return false;
		}
		//assert(!is_in_connected_list(peer_info.peer_entry));
		
		if(m_infohash != peer_info.infohash)
		{
				DEBUG_PRINT0("PeerManager::on_conn_established : established connection peer id infohash invalid\n");
				return false;
		}
		
		if(m_peer_id == peer_info.peer_id) 
		{
				DEBUG_PRINT0("PeerManager::on_conn_established : established connection peer id equal local peer id\n");
				return false;
		}
		
		if(peer_id_is_existed(peer_info.peer_id))
		{
				DEBUG_PRINT0("PeerManager::on_conn_established : established connection peer id existed\n");
				return false;
		}

		PeerLinkInfo peer_link_info;

		if(peer_info.mode == LOCAL)
		{
				PeerLinkMap::iterator it = m_connecting_peers.find(peer_info.peer_entry);
				if(it == m_connecting_peers.end())
				{
						 DEBUG_PRINT0("received established connection msg not in connecting list\n");
						 return false;
				 }else
				 {
						 peer_link_info = it->second;
						 m_connecting_peers.erase(it);
				 }
		}
		
		assert(peer_link_info.peer_link == 0);
		
		if(peer_info.mode == LOCAL)
		{
				assert(peer_link_info.peer_entry == peer_info.peer_entry);
		}else
		{
				peer_link_info.peer_entry = peer_info.peer_entry;
		}

		PeerLink *plink = new PeerLink(peer_info, m_task_id, m_piece_manager, *this, m_global_set, m_res_manager);
		peer_link_info.peer_link = plink;
		m_connected_peers[peer_info.peer_entry] = peer_link_info;

		bool res = m_serv_manager.GetNetService().RegisterHandler(plink->GetPeerIO());
		assert(res);
		
		
		return true;
}



void PeerManager::OnNetEvent(const NetEvent *pnet_event)
{
		assert(pnet_event != 0);

		switch(pnet_event->GetNetEventType())
		{
		case NET_IO_EVENT:
		{
				NetIOEvent *pio = (NetIOEvent*)pnet_event;
				on_net_io_event(pio->GetPeerEntry(), pio->GetBuffer(), pio->GetBufferLength());
				
				break;
		}
		case NET_CONN_ESTABLISHED:
		{
				NetConnEstablished *pconn_ok = (NetConnEstablished*)pnet_event;
				PeerInfo pinfo;
				pinfo.infohash = pconn_ok->GetInfoHash();
				pinfo.mode = pconn_ok->GetConnMode();
				pinfo.net_hdl = pconn_ok->GetNetHandle();
				pinfo.peer_entry = pconn_ok->GetPeerEntry();
				pinfo.peer_id = pconn_ok->GetPeerID();
				memcpy(pinfo.ext_info, pconn_ok->GetExtInfo(), 8);
				if(on_conn_established(pinfo))
				{
						pconn_ok->SetNetHandle(NET_INVALID_SOCKET);
				}
				
				break;
		}
		case NET_CONN_ABORT:
		{
				DEBUG_PRINT0("ON NET_CONN_ABORT\n");
				on_connection_abort(((NetConnAbort*)pnet_event)->GetPeerEntry());
				break;
		}
		case NET_CONNECT_REAMOTE_FAILED:
		{
				DEBUG_PRINT0("NET_CONNECT_REAMOTE_FAILED\n");
				on_connect_failed(((NetConnectPeerFailed*)pnet_event)->GetPeerEntry());
				break;
		}
		default:
		{
				assert(0);
		}
		}
}



void PeerManager::on_connection_abort(const PeerEntry &pe)
{
		/*if(!is_in_connected_list(pe)) 
		{
				assert(0);
				return;
		}*/

		PeerLinkMap::iterator it = m_connected_peers.find(pe);

		if(it == m_connected_peers.end())
		{
				//assert(0);
				return;
		}
		
		PeerLinkInfo peer_link_info = it->second;
		m_connected_peers.erase(it);
		ConnMode mode = peer_link_info.peer_link->GetConnectionMode();
		delete peer_link_info.peer_link;
		peer_link_info.peer_link = 0;
		peer_link_info.failed_count++;
		//delete it->second.peer_link;
		//it->second.peer_link = 0;
		//it->second.failed_count++;
		
		if(peer_link_info.failed_count > MAX_ERROR_TIMES)
		{
				m_res_manager.GetIPFilter().BandAddress(peer_link_info.peer_entry);
		}else
		{
				if(mode == LOCAL)
				{
						DEBUG_PRINT0("insert local abort connection to unused peers\n");
						m_unused_peers[pe] = peer_link_info;
				}
		}
}

void PeerManager::on_connect_failed(const PeerEntry &pe)
{
		//if(!is_in_connecting_list(pe)) return;
		
		PeerLinkMap::iterator it = m_connecting_peers.find(pe);
		if(it == m_connecting_peers.end())
		{
				DEBUG_PRINT0("PeerManager::on_connect_failed : not find peerentry in connecting_list\n");
				return;
		}
		
		PeerLinkInfo plink_info = it->second;
		m_connecting_peers.erase(it);
		
		plink_info.failed_count++;
		
		if(plink_info.failed_count > MAX_ERROR_TIMES)
		{
				m_res_manager.GetIPFilter().BandAddress(plink_info.peer_entry);
		}else
		{
				m_unused_peers[plink_info.peer_entry] = plink_info;
		}
}



bool PeerManager::peer_id_is_existed(const PeerID &peer_id)const
{
		for(PeerLinkMap::const_iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				if(it->second.peer_link->GetPeerID() == peer_id)
				{
						return true;
				}
		}

		return false;
}



void PeerManager::on_net_io_event(const PeerEntry &pe, const t_byte *pbuf, t_uint32 len)
{
		PeerLinkMap::iterator it = m_connected_peers.find(pe);

		if(it == m_connected_peers.end())
		{
				DEBUG_PRINT0("On NET_IO_EVENT not find correct peer_link handler\n");
				return;
		}
		
		assert(it->second.peer_link != 0);

		if(!it->second.peer_link->OnNetPackage(pbuf, len))
		{
				PeerLinkInfo plink_info = it->second;
				m_connected_peers.erase(it);

				m_serv_manager.GetNetService().RemoveHandler(plink_info.peer_link->GetPeerIO());
				ConnMode mode = plink_info.peer_link->GetConnectionMode();
				delete plink_info.peer_link;
				plink_info.peer_link = 0;
				plink_info.failed_count++;

				//本地连接的，才有band的可能，否则，等于没用
				if(plink_info.failed_count > MAX_ERROR_TIMES)
				{
						DEBUG_PRINT0("band ip because plink_info.failed_count > 3\n");
						m_res_manager.GetIPFilter().BandAddress(plink_info.peer_entry);
				}else
				{
						if(mode == LOCAL)//只有本地连接存的是remote的listen ip
						{
								m_unused_peers[pe] = plink_info;
						}
				}
				
		}
}



//////////////////////////////////////////////in peer link call///////////////////////////////////////////////////

void PeerManager::BroadCastHave(t_uint32 piece_idx)
{
		for(PeerLinkMap::iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				assert(it->second.peer_link != 0);
				it->second.peer_link->NotifyHavePiece(piece_idx);
		}
}


void PeerManager::OnDownloadCompleted()
{
		for(PeerLinkMap::iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				assert(it->second.peer_link != 0);
				it->second.peer_link->OnDownloadCompleted();
		}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


t_uint32 PeerManager::GetPeerCount()const
{
		t_uint32 count = 0;

		for(PeerLinkMap::const_iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				assert(it->second.peer_link != 0);
				if(!it->second.peer_link->IsSeed())
				{
						count++;
				}
		}
		return count;
}

t_uint32 PeerManager::GetSeedCount()const
{
		t_uint32 peer_count = GetPeerCount();

		return m_connected_peers.size() - peer_count;

}


t_uint32 PeerManager::GetUnusedPeersCount()const
{
		return m_unused_peers.size();
}


const Statistics& PeerManager::GetTaskStatistics()const
{
		return m_stat;

}




PeerList PeerManager::GetConnectedPeers()const
{
		PeerList peers;

		for(PeerLinkMap::const_iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				peers.push_back(it->first);

		}
		return peers;
}


PeerList PeerManager::GetConnectingPeers()const
{
		PeerList peers;

		for(PeerLinkMap::const_iterator it = m_connecting_peers.begin(); it != m_connecting_peers.end(); ++it)
		{
				peers.push_back(it->first);

		}
		return peers;
}




std::vector<PeerStatus> PeerManager::GetPeersStatus()const
{
		std::vector<PeerStatus> res(m_connected_peers.size());

		for(PeerLinkMap::const_iterator it = m_connected_peers.begin(); it != m_connected_peers.end(); ++it)
		{
				res.push_back(it->second.peer_link->GetPeerStatus());
		}

		return res;

}



}


