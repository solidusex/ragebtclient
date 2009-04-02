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
#include "PeerLink.h"
#include "PeerManager.h"
#include "PeerIO.h"
namespace Rage {

PeerLink::PeerLink(const PeerInfo &peer_info, 
				   TaskID task_uni_id,
					PieceManager &piece_manager,
					PeerManager &peer_manager, 
					const GlobalSetting &g_setting,
					ResourceManager	&res_manager
					) 
					: m_manager(peer_manager)
					, m_piece_manager(piece_manager)
					, m_global_setting(g_setting)
					, m_res_manager(res_manager)
					, m_mode(peer_info.mode)
					, m_remote_own_pieces(piece_manager.GetBitField().NBits())
					, m_local_choking_remote(true)
					, m_local_interested_remote(false)
					, m_remote_choking_local(true)
					, m_remote_interested_local(false)
					, m_remote_peer_id(peer_info.peer_id)
					, m_peer_entry(peer_info.peer_entry)
					, m_received_bitfield(false)
{
		assert(task_uni_id != INVALID_TASK_ID);
		memcpy(m_ext_info, peer_info.ext_info, 8);
		m_peer_io = new PeerIO(peer_info.net_hdl, task_uni_id,peer_info.peer_entry);
		//assert(m_piece_manager.GetStorage().IsOpen());
		send_bitfield();

}

PeerLink::~PeerLink()
{
		m_peer_io->Release();
		
		//所有request的都是未完成的子片请求
		for(LocalRequest::iterator it = m_local_req_list.begin(); it != m_local_req_list.end(); ++it)
		{
				m_piece_manager.AbortRequest(*it);
		}
		
		m_piece_manager.DecreaseHave(m_remote_own_pieces);
}









///////////////////////////////////////////////////////////////Send////////////////////////////////////

void PeerLink::send_choke(bool choke)
{
		DEBUG_PRINT0("send_choke(bool choke)\n");
		t_byte buf[5];

		*((t_uint32*)buf) = ::htonl(1);

		if(choke)
		{
				*((t_byte*)(buf+4)) = 0;
				m_local_choking_remote = true;
		}
		else
		{
				*((t_byte*)(buf+4)) = 1;
				m_local_choking_remote = false;
		}

		m_peer_io->WriteData(buf, 5);
		m_stat.SentBytes(0, 5);
}
		

void PeerLink::send_interested(bool interested)
{
		DEBUG_PRINT0("send_interested(bool interested)\n");
		t_byte buf[5];
		*((t_uint32*)buf) = htonl(1);

		if(interested)
		{
				*((t_byte*)(buf+4)) = 2;
				m_local_interested_remote = true;
		}
		else
		{
				*((t_byte*)(buf+4)) = 3;
				m_local_interested_remote = false;
		}

		m_peer_io->WriteData(buf, 5);
		m_stat.SentBytes(0,5);
}



void PeerLink::send_bitfield()
{
		DEBUG_PRINT0("PeerLink::send_bitfield()\n");
		const BitField &bit_field = m_piece_manager.GetBitField();
		
		if(bit_field.IsEmpty())
		{
				return;
		}else
		{
				t_byte header[5];
				*((t_uint32*)(header)) =  htonl(1 + bit_field.NBytes());
				*((t_byte*)(header+4))  = 5;
				
				m_peer_io->WriteData(header, 5);
				m_peer_io->WriteData(bit_field.GetContent(), bit_field.NBytes());
				m_stat.SentBytes(0, 5 + bit_field.NBytes());
		}
}

void PeerLink::send_have(t_uint32 piece_idx)
{
		DEBUG_PRINT0("PeerLink::send_have()\n");

		assert(piece_idx < m_piece_manager.GetBitField().NBits());
		assert(m_piece_manager.GetBitField().IsSet(piece_idx));

		t_byte buf[9];
		*((t_uint32*)buf) = htonl(5);
		*((t_byte*)(buf+4)) = 4;
		*((t_uint32*)(buf+5)) = htonl(piece_idx);
		m_peer_io->WriteData(buf, 9);
		m_stat.SentBytes(0, 9);
}

void PeerLink::send_request(t_uint32 piece_idx, t_uint32 offset, t_uint32 len)
{
		DEBUG_PRINT0("PeerLink::send_request\n");
		assert(piece_idx < m_piece_manager.GetBitField().NBits());
		assert(!m_piece_manager.GetBitField().IsSet(piece_idx));

		t_byte buf[17];
		*((t_uint32*)buf) = htonl(13);
		*((t_byte*)(buf+4)) = 6;
		*((t_uint32*)(buf+5)) = htonl(piece_idx);
		*((t_uint32*)(buf+9)) = htonl(offset);
		*((t_uint32*)(buf+13)) = htonl(len);
		m_peer_io->WriteData(buf, 17);
		m_stat.SentBytes(0, 17);
}

void PeerLink::send_cancel(t_uint32 piece_idx, t_uint32 offset, t_uint32 len)
{
		DEBUG_PRINT0("PeerLink::send_cancel\n");
		assert(piece_idx < m_piece_manager.GetBitField().NBits());
	//	assert(m_storage.GetBitfield().IsSet(piece_idx));

		t_byte buf[17];
		*((t_uint32*)buf) = htonl(13);
		*((t_byte*)(buf+4)) = 8;
		*((t_uint32*)(buf+5)) = htonl(piece_idx);
		*((t_uint32*)(buf+9)) = htonl(offset);
		*((t_uint32*)(buf+13)) = htonl(len);

		m_peer_io->WriteData(buf, 17);

		m_stat.SentBytes(0, 17);
}

void PeerLink::send_piece_data(t_uint32 piece_idx, t_uint32 offset, t_uint32 len, const t_byte *pbuf)
{
		
		DEBUG_PRINT0("PeerLink::send_piece_data\n");

		assert(piece_idx < m_piece_manager.GetBitField().NBits());
		assert(m_piece_manager.GetBitField().IsSet(piece_idx));
		assert(pbuf != 0 && len != 0);

		t_byte buf[13];
		*((t_uint32*)buf) = htonl(9+len);
		*((t_byte*)(buf+4)) = 7;
		*((t_uint32*)(buf+5)) = htonl(piece_idx);
		*((t_uint32*)(buf+9)) = htonl(offset);

		m_peer_io->WriteData(buf, 13);
		m_peer_io->WriteData(pbuf, len);
		m_stat.SentBytes(len, 13);
}



void PeerLink::send_keep_alive()
{
		DEBUG_PRINT0("PeerLink::send_keep_alive()\n");
		t_byte buf[4];
		
		*((t_uint32*)buf) = 0;

		m_peer_io->WriteData(buf, 4);
		m_stat.SentBytes(0, 4);
}


///////////////////////////////////////////////core strategy/////////////////
#if(0)
void PeerLink::do_pieces_request()
{
		//if(!m_local_req_list.empty())return;

		if(m_piece_manager.IsFinished()) return;

		if(m_remote_choking_local) return;
		
		const t_uint32 pending_count = m_global_setting.GetMaxRequestPendingCount();
		assert(pending_count != 0);
		
		while(m_local_req_list.size() < pending_count)
		{
				SliceInfo slice_info;

				//这里是有可能选不出来了的
				if(!m_piece_manager.GetRequest(slice_info, m_remote_own_pieces))
				{
						//如果一个都没选出来 && local interested remote,
						//send not interested
						if(m_local_req_list.empty() && m_local_interested_remote)
						{
								send_interested(false);
						}
						return;
				}else
				{
						//如果选成功了，而且loca not interested remote，则send interested
						//这里只会执行一次，下次m_local_interested_remote就会为true了
						if(!m_local_interested_remote)
						{
								send_interested(true);
						}
						m_local_req_list.push_back(slice_info);
						send_request(slice_info.piece_idx, slice_info.start_pos, slice_info.slice_len);
				}
		}
}
#endif
/*
void PeerLink::do_pieces_request()
{
		if(m_piece_manager.IsFinished()) return;

		if(m_remote_choking_local) return;
		
		const t_uint32 pending_count = m_global_setting.GetMaxRequestPendingCount();
		assert(pending_count != 0);
		
		if(m_local_req_list.size() >= pending_count) return;

		t_uint32 remain_count = pending_count - m_local_req_list.size();
		
		std::list<SliceInfo> slist;
		while(remain_count > 0)
		{
				
				if(!m_piece_manager.GetRequest(slist, remain_count, m_remote_own_pieces))
				{
						if(m_local_interested_remote)
						{
								send_interested(false);
						}
						return;
				}else
				{
						if(!m_local_interested_remote)
						{
								send_interested(true);
						}
						
						assert(slist.size() > 0);
						assert(slist.size() <= remain_count);

						for(std::list<SliceInfo>::iterator it = slist.begin(); it != slist.end(); ++it)
						{
								send_request(it->piece_idx, it->start_pos, it->slice_len);
								m_local_req_list.push_back(*it);
								remain_count--;
						}
						slist.clear();
				}
		}
}
*/

void PeerLink::do_pieces_request()
{
		if(m_piece_manager.IsFinished()) return;

		if(m_remote_choking_local) return;
		
		const t_uint32 pending_count = m_global_setting.GetMaxRequestPendingCount();
		assert(pending_count != 0);
		
		if(m_local_req_list.size() > 0) return;

		t_uint32 remain_count = pending_count - m_local_req_list.size();
		
		std::list<SliceInfo> slist;
		while(remain_count > 0)
		{
				
				if(!m_piece_manager.GetRequest(slist, remain_count, m_remote_own_pieces))
				{
						if(m_local_interested_remote)
						{
								send_interested(false);
						}
						return;
				}else
				{
						if(!m_local_interested_remote)
						{
								send_interested(true);
						}
						
						assert(slist.size() > 0);
						assert(slist.size() <= remain_count);

						for(std::list<SliceInfo>::iterator it = slist.begin(); it != slist.end(); ++it)
						{
								send_request(it->piece_idx, it->start_pos, it->slice_len);
								m_local_req_list.push_back(*it);
								remain_count--;
						}
						slist.clear();
				}
		}
}


//给对端回复对端的请求
void PeerLink::do_pieces_send()
{
		//如果本端choke对端了，则返回
		if(m_local_choking_remote) return;
		if(m_remote_req_list.empty())return;
		if(m_peer_io->GetSendBufferSize() > (0*KB)) return;

		SliceInfo peer_request = m_remote_req_list.front();
		m_remote_req_list.pop_front();
		assert(peer_request.slice_len != 0);
		std::vector<t_byte> buf(peer_request.slice_len);
		
//		Storage &storage = m_piece_manager.GetStorage();
		
		
		bool res  = m_piece_manager.ReadData(&buf[0], peer_request.slice_len, peer_request.piece_idx, peer_request.start_pos);
		assert(res);
		send_piece_data(peer_request.piece_idx, peer_request.start_pos, peer_request.slice_len, &buf[0]);
}





/////////////////////////////////////////////////////////Recv////////////////////////////////



bool PeerLink::on_bitfield(const t_byte *pbuf, t_uint32 len)
{
		DEBUG_PRINT0("PeerLink::on_bitfield\n");

		const BitField &bitfield = m_piece_manager.GetBitField();
		
		if(pbuf == 0 || len != bitfield.NBytes() || m_received_bitfield)
		{
				return false;
		}else
		{
				m_remote_own_pieces.Reset(pbuf, bitfield.NBits());
				m_received_bitfield = true;
				m_stat.ReceivedBytes(0, len + 4 + 1);//所有协议消息都要加一个4字节的len和一个1字节的id
				
				
				for(size_t pidx = 0; pidx < m_remote_own_pieces.NBits(); ++pidx)
				{
						if(m_remote_own_pieces.IsSet(pidx) && !bitfield.IsSet(pidx))
						{
								send_interested(true);
								break;
						}
				}
				
				m_piece_manager.IncreaseHave(m_remote_own_pieces);
				return true;
		}
}


bool PeerLink::on_choke(const t_byte *pbuf, t_uint32 len)
{
		DEBUG_PRINT0("PeerLink::on_choke\n");
		assert(pbuf == 0 && len == 0);
		m_remote_choking_local = true;
		m_stat.ReceivedBytes(0, 4 + 1);


		for(LocalRequest::iterator it = m_local_req_list.begin(); it != m_local_req_list.end(); ++it)
		{
				m_piece_manager.AbortRequest(*it);
		}
		m_local_req_list.clear();
		return true;
}


bool PeerLink::on_unchoke(const t_byte *pbuf, t_uint32 len)
{
		DEBUG_PRINT0("PeerLink::on_unchoke\n");
		assert(pbuf == 0 && len == 0);
		m_remote_choking_local = false;
		
		do_pieces_request();

		m_stat.ReceivedBytes(0, 4 + 1);
		return true;
}


bool PeerLink::on_interested(const t_byte *pbuf, t_uint32 len)
{
		DEBUG_PRINT0("PeerLink::on_interested\n");

		assert(pbuf == 0 && len == 0);
		m_remote_interested_local = true;
		m_stat.ReceivedBytes(0, 4 + 1);
		return true;
}


bool PeerLink::on_not_interested(const t_byte *pbuf, t_uint32 len)
{
		DEBUG_PRINT0("PeerLink::on_not_interested\n");
		
		assert(pbuf == 0 && len == 0);
		m_remote_interested_local = false;
		m_stat.ReceivedBytes(0, 4 + 1);
		return true;
}

bool PeerLink::on_have(const t_byte *pbuf, t_uint32 len)
{
		DEBUG_PRINT0("PeerLink::on_have\n");

		if(len != 4)
		{
				return false;
		}

		t_uint32 index = *((t_uint32*)pbuf);
		index = ntohl(index);
		
		if(index >= m_remote_own_pieces.NBits()) return false;
		
		if(!m_remote_own_pieces.IsSet(index))
		{
				m_remote_own_pieces.Set(index);
				m_piece_manager.IncreaseHave(index);
				if(!m_remote_choking_local)//如果远方peer没choking我
				{
						do_pieces_request();
				}
		}
		m_stat.ReceivedBytes(0, len + 4 + 1);
		return true;
}




bool PeerLink::on_request(const t_byte *pbuf, t_uint32 len)
{
		DEBUG_PRINT0("PeerLink::on_request\n");

		assert(pbuf != 0);

		if(len != 12)
		{
				return false;
		}

		if(m_local_choking_remote)
		{
				//choked remote client
				return true;
		}

		t_uint32 index = ntohl(*((t_uint32*)pbuf));

		t_uint32 offset = ntohl(*((t_uint32*)((t_byte*)pbuf+4)));
		
		t_uint32 request_len = ntohl(*((t_uint32*)((t_byte*)pbuf+8)));

		if(index >= m_remote_own_pieces.NBits()||request_len == 0 || request_len > (128*KB))
		{
				DEBUG_PRINT0("garbage request param , disconnect connection\n");
				return false;
		}else if(!m_piece_manager.GetBitField().IsSet(index))
		{
				return false;
		}
		
		m_stat.ReceivedBytes(0, len + 4 + 1);

		PeerRequestList::iterator iter = m_remote_req_list.begin();
		for(; iter != m_remote_req_list.end(); ++iter)
		{
				if(iter->piece_idx == index	&& iter->start_pos == offset)
				{
						break;
				}
		}

		if(iter == m_remote_req_list.end() )
		{
				SliceInfo peer_request;
				peer_request.piece_idx = index;
				peer_request.start_pos = offset;
				peer_request.slice_len = request_len;
				m_remote_req_list.push_back(peer_request);
		}
		//do_pieces_send();
		return true;
}



bool PeerLink::on_cancel(const t_byte *pbuf, t_uint32 len)
{
		DEBUG_PRINT0("PeerLink::on_cancel\n");
		
		

		if(len != 12 || pbuf == 0)
		{
				return false;
		}

		t_uint32 piece_index = ntohl(*((t_uint32*)pbuf));
		
		t_uint32 offset = ntohl(*((t_uint32*)((t_byte*)pbuf+4)));
		
		t_uint32 slice_len = ntohl(*((t_uint32*)((t_byte*)pbuf+8)));

		PeerRequestList::iterator iter = m_remote_req_list.begin();

		for(;iter!= m_remote_req_list.end();++iter)
		{
				if(iter->piece_idx == piece_index && iter->start_pos == offset)
				{
						m_remote_req_list.erase(iter);
						break;
				}
		}
		
		m_stat.ReceivedBytes(0, 4 + 1 +  4 + 4 + 4);
		return true;

}




bool PeerLink::on_piece(const t_byte *pbuf, t_uint32 len)
{
		DEBUG_PRINT0("PeerLink::on_piece\n");

		if(len <= 8)
		{
				return false;
		}

		t_uint32 piece_idx = ntohl(*((t_uint32*)pbuf));
		
		t_uint32 offset = ntohl(*((t_uint32*)((t_byte*)pbuf+4)));
		
		t_uint32 slice_len = len - 8;
		
		
		//一个peer_io分析的32bit整数，一个byte的id,一个index长度，一个offset，剩下的是slice
		m_stat.ReceivedBytes(slice_len, 4 + 1 + 4 + 4);

		
		LocalRequest::iterator it = m_local_req_list.begin();
		
		for(; it != m_local_req_list.end(); ++it)
		{
				if(it->piece_idx == piece_idx && it->start_pos == offset)
				{
						break;
				}
		}
		
		 //这是有可能的，例如某片请求之后被其他peer下载ok了,对方peer在本地cancel到达之前
		//send了这slice数据

		if(it == m_local_req_list.end()) return true;
		
		SliceInfo slice_info = *it;

		m_local_req_list.erase(it);
		
		//垃圾数据，和我请求的长度不一样，则关闭链接
		if(slice_info.slice_len != slice_len) 
		{
				DEBUG_PRINT0("on_piece : Get slice len not equal request\n");
				return false;
		}
		
		
		
		
		//这同样是有可能的,如果本片段已经完成，则返回
		if(m_piece_manager.PieceIsCompleted(piece_idx)) return true;

		
		//m_piece_manager.WriteData((const t_byte*)pbuf+8, slice_len, piece_idx, offset);
		
		if(!m_piece_manager.SliceCompleted(slice_info, (const t_byte*)pbuf+8))
		{
				//这是有可能的，在end game模式下
				return true;
		}
		
		
		
		
		
		

		if(m_piece_manager.PieceIsCompleted(piece_idx))
		{
				m_manager.BroadCastHave(piece_idx);
		}

		if(m_piece_manager.IsFinished())
		{
				m_manager.OnDownloadCompleted();
		}else
		{
				do_pieces_request();
		}
		
		return true;
}





////////////////////////////////////////////////////////////////////////////////////////
/*
void PeerLink::CancelPieceRequest(t_uint32 piece_idx)
{
		if(m_request.GetPieceIndex() == piece_idx)
		{
				t_uint32 offset;
				t_uint32 len;

				while(m_request.CancelPendingRequest(offset, len))
				{
						send_cancel(piece_idx, offset, len);
				}

				m_request.SetInValid();
				
				if(!m_remote_choking_local)
				{
						elect_new_piece_task();
						do_pieces_request();
				}	
		}
}*/



void PeerLink::NotifyHavePiece(t_uint32 piece_idx)
{
		LocalRequest::iterator it = m_local_req_list.begin();

		while(it != m_local_req_list.end())
		{
				if(it->piece_idx == piece_idx)
				{
						//如果有任何我已发送request的slice所在的piece已经获得，
						//则send_cancel,这地方也不用调用m_piece_manager了，也为已经成功了
						send_cancel(piece_idx, it->start_pos, it->slice_len);
						it = m_local_req_list.erase(it);
				}else
				{
						++it;
				}
		}
		send_have(piece_idx);

		if(!m_remote_choking_local)
		{
				do_pieces_request();
		}	
}






///////////////////////////////////////////PeerManager Call///////////////////////////////
void PeerLink::ChokePeer(bool choke)
{
		if(choke)
		{
				m_remote_req_list.clear();
		}else
		{

		}
		send_choke(choke);
}

bool PeerLink::IsNeedClose()
{
		if(m_last_recv_time_stamp.ElapsedMillisecond() >= KEEPALIVE_TIMEOUT)
		{
				DEBUG_PRINT0("NOT RECV KEEP ALIVE Should close this connection\n");
				return true;
		}else if(m_piece_manager.IsFinished() && m_remote_own_pieces.IsAllSet())
		{
				return true;
		}else
		{
				return false;
		}
}


void PeerLink::SecondTick()
{
		m_stat.SecondTick();
		do_pieces_send();//尽可能的发送
		do_pieces_request();//尽可能的接收

		
		
		
}


void PeerLink::OnDownloadCompleted()
{
		send_interested(false);
		m_local_req_list.clear();
}


bool  PeerLink::IsSeed()const
{
		return m_remote_own_pieces.IsAllSet();
}

bool PeerLink::OnNetPackage(const t_byte *pbuf, t_uint32 len)
{
		m_last_recv_time_stamp.Update();
		
		if(len == 0) return true;
		
		assert(pbuf != 0);
		t_byte cmd = pbuf[0];
		const t_byte *data = pbuf + 1;
		t_uint32 data_len = len - 1;
		bool res;
		switch(cmd)
		{
		case 0:
				res =  on_choke(0,0);//(data, data_len);
				break;
		case 1:
				res =  on_unchoke(0,0);//(data, data_len);
				break;						
		case 2:
				res =  on_interested(0,0);//(data, data_len);
				break;
		case 3:
				res =  on_not_interested(0,0);//(data, data_len);
				break;
		case 4:
				res =  on_have(data, data_len);
				break;
		case 5:
				res =  on_bitfield(data, data_len);
				break;
		case 6:
				res =  on_request(data, data_len);
				break;
		case 7:
				res =  on_piece(data, data_len);
				break;
		case 8:
				res =  on_cancel(data, data_len);
				break;
		default:
				return true;
		}

		

		return res;
}



void PeerLink::SendKeepAlive()
{
		send_keep_alive();
}


///////////////////////////////////////////////////////////

bool PeerLink::IsChokedRemote()const
{
		return m_local_choking_remote;
}

bool PeerLink::IsInterestedRemote()const
{
		return m_local_interested_remote;
}


bool PeerLink::RemoteIsChokedLocal()const
{
		return m_remote_choking_local;
}


bool PeerLink::RemoteIsInterestedLocal()const
{
		return m_remote_interested_local;
}


t_uint64 PeerLink::GetConnectedTime()const
{

		return m_connected_ts.ElapsedSecond();
}



PeerIO* PeerLink::GetPeerIO()
{
		return m_peer_io;
}

const PeerID& PeerLink::GetPeerID()const
{
		return m_remote_peer_id;
}

const Statistics& PeerLink::GetStatistics()const
{
		return m_stat;
}


const PeerEntry& PeerLink::GetPeerEntry()const
{
		return m_peer_entry;
}
		
const BitField& PeerLink::GetPeerOwnBitField()const
{
		return m_remote_own_pieces;
}

ConnMode PeerLink::GetConnectionMode()const
{
		return m_mode;
}





void PeerLink::AbortAllRequest()
{
		for(LocalRequest::iterator it = m_local_req_list.begin(); it != m_local_req_list.end(); ++it)
		{
				m_piece_manager.AbortRequest(*it);

		}

		m_local_req_list.clear();
}




PeerStatus PeerLink::GetPeerStatus()const
{
		PeerStatus res;

		res.down_rate = m_stat.DownloadRate();
		res.up_rate = m_stat.UploadRate();

		res.peer_entry = GetPeerEntry();

		res.mode = m_mode;
		res.total_down = m_stat.TotalPayloadDownload();
		res.total_up = m_stat.TotalPayloadUpload();
		
		size_t own_pieces = 0;
		for(size_t i = 0; i < m_remote_own_pieces.NBits(); ++i)
		{
				if(m_remote_own_pieces.IsSet(i))
				{
						own_pieces++;
				}
		}

		res.finished_degree = (float)own_pieces / (float)m_remote_own_pieces.NBits();

		return res;
}


#if(0)
#endif







}

