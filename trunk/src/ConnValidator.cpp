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
 */#include "ConnValidator.h"

namespace Rage {

///////////////////////////PeerConnector//////////////////////////////////////


PeerConnector::PeerConnector(const NetSpace::InetAddress &addr,
					  const TaskID				  &task_id,
					  const Sha1Hash			  &infohash,
					  const PeerID				  &peer_id
					  )
					  : NetSpace::ConnectorEx(addr, CONNECT_REMOTE_TIMEOUT)
					  , m_task_id(task_id)
					  , m_infohash(infohash)
					  , m_peer_id(peer_id)
{
		assert(m_task_id != INVALID_TASK_ID);

		m_peer_entry.addr = addr.IP();
		m_peer_entry.port = addr.Port();
}

PeerConnector::~PeerConnector()
{
		

}

void PeerConnector::on_connected(NetSpace::NetHandle hdl)
{
		assert(GetSelector() != 0);
		
		OutConnValidator *pov = new OutConnValidator(hdl, m_peer_entry, m_task_id, m_infohash, m_peer_id);
		assert(pov != 0);
		bool res = GetSelector()->RegisterHandler(pov);
		assert(res);
		pov->Release();
}

void PeerConnector::on_timeout()
{
		
		//PeerEntry pe(GetRemoteAddr().IP(), GetRemoteAddr().Port());
		//PeerEntry pe(m_addr.IP(), m_addr.Port());
		
		MsgSpace::PostMessageEx(m_task_id, new NetConnectPeerFailed(m_peer_entry));
}

void PeerConnector::on_error(t_ulong error_code)
{
		//PeerEntry pe(GetRemoteAddr().IP(), GetRemoteAddr().Port());
		MsgSpace::PostMessageEx(m_task_id, new NetConnectPeerFailed(m_peer_entry));
}



//////////////////////////////////Validator//////////////////////////////////////


static const t_byte PROTOCOL_LENGTH = 19;
static const  char* PROTOCOL_STRING = "BitTorrent protocol";
static const t_byte RESERVED_8BYTES[8] = {0,0,0,0,0,0,0,0 };


//Peer_id 为返回对方peer_id;
bool parse_handshake_info(const t_byte* pbuf, Sha1Hash &info_hash, PeerID &peer_id, t_byte ext_info[8])
{
		assert(pbuf != 0);
		
		if(pbuf[0] != PROTOCOL_LENGTH)
		{
				
				DEBUG_PRINT0("invalid protocol length\n");
				return false;
		}
		
		//DEBUG_PRINT1("%s\n", (const char*)&pbuf[1]);
		if(memcmp(&pbuf[1], PROTOCOL_STRING, PROTOCOL_LENGTH) == 0)
		{
				//DEBUG_PRINT0("invalid protocol string\n");
				memcpy(ext_info, &pbuf[20], 8); 
				info_hash.Reset(&pbuf[28]);
				peer_id.Reset(&pbuf[48]);
				return true;
		}else
		{
				DEBUG_PRINT0("invalid protocol string\n");
				return false;
		}
		
		
}

std::vector<t_byte> build_hand_shake(const Sha1Hash &info_hash, const PeerID &local_peer_id)
{
		std::vector<t_byte>  buf(68, 0);
		buf[0] = PROTOCOL_LENGTH;
		memcpy(&buf[1], PROTOCOL_STRING, (t_uint32)PROTOCOL_LENGTH);
		memcpy(&buf[28], info_hash.GetRawData(), 20);
		memcpy(&buf[48], local_peer_id.GetRawData(), 20);
		return buf;
}


//////////////////////////////OutConnValidator///////////////////////////////////////////

OutConnValidator::OutConnValidator(NetSpace::NetHandle net_hdl,const PeerEntry &peer_entry, const TaskID	&task_id, 
								   const Sha1Hash &info_hash, const PeerID &peer_id)
						: m_stream_sock(net_hdl)
						, m_task_id(task_id)
						, m_infohash(info_hash)
						, m_peer_id(peer_id)
						, m_peer_entry(peer_entry)
{
		assert(m_stream_sock.IsValid());
		assert(m_task_id != INVALID_TASK_ID);

		/*NetSpace::InetAddress addr;
		m_stream_sock.GetRemoteAddr(addr);
		m_peer_entry = PeerEntry(addr.IP(), addr.Port());*/
		
		Mask(NetSpace::OUTPUT_MASK|NetSpace::TIMER_MASK);

		m_buf.Insert(&build_hand_shake(m_infohash, m_peer_id)[0], 68);
		assert(m_buf.Size() == 68);

		
}


OutConnValidator::~OutConnValidator()
{
		


}




NetSpace::NetHandle OutConnValidator::GetHandle()const
{
		assert(m_stream_sock.IsValid());
		return m_stream_sock.GetHandle();
}


void OutConnValidator::OnOutput()
{
		m_ts.Update();
		
		assert(m_buf.Size() > 0);
		int wn = m_stream_sock.Send(m_buf.Data(),m_buf.Size());

		if(wn > 0)
		{
				m_buf.Erase(wn);
				if(m_buf.Size() == 0)
				{
						Mask(NetSpace::INPUT_MASK|NetSpace::TIMER_MASK);
				}
		}else if(wn == -1 && NetGetLastError() == EWOULDBLOCK)
		{
				//等待下一次可写
		}else
		{
				//失败
				DEBUG_PRINT0("OutConnValidator::OnOutput() failed\n");
				MsgSpace::PostMessageEx(m_task_id, new NetConnectPeerFailed(m_peer_entry));
				GetSelector()->RemoveHandler(this);
		}

}

void OutConnValidator::check_connection_handshake()
{
		assert(m_buf.Size() == 68);
		
		PeerID remote_peer_id;
		Sha1Hash info_hash;
		t_byte ext_info[8];
		bool res = parse_handshake_info(m_buf.Data(), info_hash, remote_peer_id, ext_info);

		if(!res)
		{
				DEBUG_PRINT0("parse_handshae_info failed\n");
				goto FAILED;
		}
		if(info_hash != m_infohash)
		{
				DEBUG_PRINT0("response infohash invalid\n");
				goto FAILED;
		}
		
		if(remote_peer_id == m_peer_id)
		{
				DEBUG_PRINT0("OutConnValidator::response peer id equal to local\n");
				goto FAILED;
		}
		
		MsgSpace::PostMessageEx(m_task_id, new NetConnEstablished(m_stream_sock.Duplicate(), LOCAL, remote_peer_id, m_infohash, m_peer_entry, ext_info));
		return;

FAILED:
		MsgSpace::PostMessageEx(m_task_id, new NetConnectPeerFailed(m_peer_entry));
		

}

void OutConnValidator::OnInput()
{
		m_ts.Update();
		
		assert(m_buf.Size() < 68);
		t_byte buf[68];
		size_t read_n = 68 - m_buf.Size();
		assert(read_n <= 68);

		int rn = m_stream_sock.Recv(buf, read_n);
		assert(rn <= 68);

		if(rn > 0)
		{
				m_buf.Insert(buf, rn);
				assert(m_buf.Size() <= 68);

				if(m_buf.Size() == 68)
				{
						check_connection_handshake();
						GetSelector()->RemoveHandler(this);
				}
		}else if(rn == -1 && NetGetLastError() == EWOULDBLOCK)
		{
				//等待下次
		}else
		{
				DEBUG_PRINT0("OutConnValidator::OnInput connection abort\n");
				//网络连接断开
				MsgSpace::PostMessageEx(m_task_id, new NetConnectPeerFailed(m_peer_entry));
				GetSelector()->RemoveHandler(this);
		}
}



void OutConnValidator::OnTimer()
{
		if(m_ts.ElapsedMillisecond() >= VALIDATOR_TIMEOUT)
		{
				MsgSpace::PostMessageEx(m_task_id, new NetConnectPeerFailed(m_peer_entry));
				GetSelector()->RemoveHandler(this);
		}
}
















/////////////////////////////////InConnValidator///////////////////////////////////////////////

InConnValidator::InConnValidator(ResourceManager &res_manager,NetSpace::NetHandle net_hdl) 
						: m_stream_sock(net_hdl)
						, m_res_manager(res_manager)
						, m_task_id(0)
{
		assert(m_stream_sock.IsValid());
		
		NetSpace::InetAddress addr;
		m_stream_sock.GetRemoteAddr(addr);

		m_peer_entry.addr = addr.IP();
		m_peer_entry.port  = addr.Port();
		Mask(NetSpace::INPUT_MASK|NetSpace::TIMER_MASK);
}


bool InConnValidator::check_connection_handshake()
{
		assert(m_buf.Size() == 68);

		PeerID remote_peer_id;
		
		if(parse_handshake_info(m_buf.Data(), m_infohash, remote_peer_id, m_ext_info))
		{
				DownloaderInfo dl_info;
				if(m_res_manager.GetDownloaderRecorder().GetRecord(m_infohash, dl_info))
				{
						if(dl_info.peer_id == remote_peer_id)
						{
								DEBUG_PRINT0("InConnValidator::remote peer id equal to local\n");
								NetSpace::InetAddress addr;
								m_stream_sock.GetLocalAddr(addr);//因为这个很可能是自己，反正无论如何也要band
								m_res_manager.GetIPFilter().BandAddress(addr.Str(), addr.Port());
								DEBUG_PRINT2("InConnValidator::check_connection_handshake : band peer entry == %s:%d\n", addr.Str().c_str(), addr.Port());
								return false;
						}

						m_task_id = dl_info.task_id;
						m_infohash = dl_info.info_hash;
						m_peer_id = remote_peer_id;
						
						m_buf.Clear();
						m_buf.Insert(&build_hand_shake(m_infohash, dl_info.peer_id)[0], 68);
						assert(m_buf.Size() == 68);
						return true;
				}else
				{
						DEBUG_PRINT0("unknow infohash\n");
						return false;
				}

		}else
		{
				DEBUG_PRINT0("parse_handshake_info failed\n");
				return false;
		}
}

void InConnValidator::OnInput()
{
		m_ts.Update();
		t_byte buf[68];

		int read_len = 68 - m_buf.Size();
		
		assert(read_len <= 68);

		int rn = m_stream_sock.Recv(buf, read_len);

		if(rn > 0)
		{
				m_buf.Insert(buf, rn);
				assert(m_buf.Size() <= 68);

				if(m_buf.Size() == 68)
				{
						if(!check_connection_handshake())
						{
								//DEBUG_PRINT0("check_connection_handshake failed\n");
								GetSelector()->RemoveHandler(this);
								return;
						}else
						{
								assert(m_buf.Size() == 68);
								Mask(NetSpace::OUTPUT_MASK|NetSpace::TIMER_MASK);
						}
				}
		}else if(rn == -1 && NetGetLastError() == EWOULDBLOCK)
		{
				//等待下次;
		}else
		{
				DEBUG_PRINT0("accepted peer connection abort\n");
				GetSelector()->RemoveHandler(this);
		}
}

void InConnValidator::OnOutput()
{
		m_ts.Update();
		
		int wn = m_stream_sock.Send(m_buf.Data(), m_buf.Size());
		
		if(wn > 0)
		{
				m_buf.Erase(wn);
				if(m_buf.IsEmpty())
				{
						MsgSpace::PostMessageEx(m_task_id, new NetConnEstablished(m_stream_sock.Duplicate(), REMOTE, m_peer_id, m_infohash, m_peer_entry, m_ext_info));
						GetSelector()->RemoveHandler(this);
				}
		}else if(wn == -1 && NetGetLastError() == EWOULDBLOCK)
		{
				//等待下次；
		}else
		{
				DEBUG_PRINT0("accepted peer connection abort\n");
				GetSelector()->RemoveHandler(this);
		}
}



void InConnValidator::OnTimer()
{
		if(m_ts.ElapsedMillisecond() >= VALIDATOR_TIMEOUT)
		{
				//MsgSpace::PostMessageEx(m_task_id, new NetConnectPeerFailed(m_peer_entry));
				DEBUG_PRINT0("accepted peer connection timeout\n");
				GetSelector()->RemoveHandler(this);
		}


}

InConnValidator::~InConnValidator()
{
		
}

NetSpace::NetHandle InConnValidator::GetHandle()const
{
		return m_stream_sock.GetHandle();
}

#if(0)
#endif

}