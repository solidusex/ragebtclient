#include "PeerIO.h"
#include "NotifyEvent.h"
namespace Rage {

NetSpace::NetHandle PeerIO::GetHandle()const
{
		assert(m_sock.IsValid());
		return m_sock.GetHandle();
}

/*
ConnMode PeerIO::GetConnectionMode()const
{
		return m_mode;
}

const PeerID& PeerIO::GetPeerID()const
{
		return m_peer_id;
}

const Sha1Hash& PeerIO::GetInfoHash()const
{
		return m_infohash;
}

const PeerEntry& PeerIO::GetPeerEntry()const
{
		return m_peer_entry;
}
*/



void PeerIO::OnInput()
{
		int avail_n = m_sock.Available();
		t_byte *buf = 0;
		if(avail_n <= 0)
		{
				send_conn_abort();
				return;
		}else
		{
				buf = m_recv_buf.Allocate(avail_n);
				
		}

		assert(buf != 0);
		int recv_n = m_sock.Recv(buf, avail_n);
		
		if(recv_n != avail_n)
		{
				send_conn_abort();
				return;
		}
		
		do{
				if(m_state == RECV_PROTOCOL_LEN && m_recv_buf.Size() >= 4)
				{
						m_remain_len = *(t_uint32*)m_recv_buf.Data();
						m_recv_buf.Erase(sizeof(t_uint32));
						m_remain_len = ByteOrder::FromNetOrderToNativeOrder(m_remain_len);

						if(m_remain_len > MAX_PROTOCOL_MSG_LEN)
						{
								send_conn_abort();
								return;
						}else
						{
								DEBUG_PRINT1("PeerIO::OnInput get remain_len == %d\n", m_remain_len);
								m_state = RECV_PROTOCOL_MSG;
						}
				}

				if(m_state == RECV_PROTOCOL_MSG && m_recv_buf.Size() >= m_remain_len)
				{
						DEBUG_PRINT1("PeerIO::OnInput get packet packet size == %d\n", m_remain_len);
						assert(m_recv_buf.Size() >= m_remain_len);
						const t_byte *pbuf = (m_remain_len > 0 ? m_recv_buf.Data() : 0);
						send_io_event(pbuf, m_remain_len);
						
						if(m_remain_len > 0)
						{
								size_t rn = m_recv_buf.Erase(m_remain_len);
								assert(rn == m_remain_len);
						}

						m_state = RECV_PROTOCOL_LEN;
						m_remain_len = 0;
				}
		}while(m_state == RECV_PROTOCOL_LEN && m_recv_buf.Size() >= 4);
}


void PeerIO::OnOutput()
{
		
		{
				LockT lock(m_send_buf_mutex);

				while(!m_send_buf.IsEmpty())
				{
						int wn = m_sock.Send(m_send_buf.Data(), m_send_buf.Size());

						if(wn == -1 && NetGetLastError() == EWOULDBLOCK)
						{
								return;
						}else if(wn <= 0)
						{
								goto CONN_ABORT_LABEL;
						}else
						{
								size_t skip_n = m_send_buf.Erase(wn);
								assert(skip_n == wn);
						}
				}

				if(m_send_buf.IsEmpty())
				{
						ClrMask(NetSpace::OUTPUT_MASK);
				}

				return;
		}

CONN_ABORT_LABEL:
		send_conn_abort();
		return;

}


size_t PeerIO::GetSendBufferSize()const
{
		LockT lock(m_send_buf_mutex);
		return m_send_buf.Size();
}

size_t PeerIO::WriteData(const t_byte *pbuf, size_t len)
{
		assert(pbuf != 0 && len > 0);
		LockT lock(m_send_buf_mutex);
		m_send_buf.Insert(pbuf, len);
		return len;
}


void PeerIO::OnTimer()
{
		if(!IsMask(NetSpace::OUTPUT_MASK))
		{
				LockT lock(m_send_buf_mutex);
				if(!m_send_buf.IsEmpty())
				{
						AddMask(NetSpace::OUTPUT_MASK);
				}
		}
}


void PeerIO::send_conn_abort()
{
		MsgSpace::PostMessageEx(m_task_uni_id, new NetConnAbort(m_peer_entry));
		GetSelector()->RemoveHandler(this);

}

void PeerIO::send_io_event(const t_byte *pbuf, size_t len)
{
		NetIOEvent *pio_event = new NetIOEvent(m_peer_entry, pbuf, len);
		MsgSpace::PostMessageEx(m_task_uni_id, pio_event);
}




PeerIO::PeerIO(NetSpace::NetHandle net_hdl, TaskID task_uni_id, const PeerEntry &pe)
								: m_sock(net_hdl)
								, m_task_uni_id(task_uni_id)
								, m_peer_entry(pe)
								, m_state(RECV_PROTOCOL_LEN)
								, m_remain_len(0)
{
		assert(m_sock.IsValid());
		assert(m_task_uni_id != INVALID_TASK_ID);
		Mask(NetSpace::INPUT_MASK|NetSpace::TIMER_MASK);
		
}

PeerIO::~PeerIO()
{
		DEBUG_PRINT0("PeerIO Release\n");
		assert(GetSelector() == 0);

}








}






















