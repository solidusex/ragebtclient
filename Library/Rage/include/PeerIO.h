
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
#include "Res.h"
#include "NotifyEvent.h"
namespace Rage {



/*实际上此类对象将在两个线程中共同运作，接收部分负责接收数据包，
  并发送到处理队列，发送部分将在逻辑线程部分，负责发送，发送时会将数据写入
  send_buf中，那个负责io部分线程将每隔N毫秒检测一次是否有可发的数据，有则发送
  也就是说，在on_timer中检测m_send_buf是否为空，不为空的话，就将timer标记清除掉
  而后标记上on_write_mask，之后开始写入，当到不可写或send_buf为空时
  ，他会自动将on_write_mask清除，并加入on_timer_mask标记，继续检测
*/




#define MAX_PROTOCOL_MSG_LEN   (256*KB)

class PeerIO : public NetSpace::NetEventHandler
{
private:

		enum RecvState
		{
				RECV_PROTOCOL_LEN = 0x100A,
				RECV_PROTOCOL_MSG = 0x100B
		};
private:
		typedef DSSpace::SimpleBuffer			BufferT;
		typedef ThreadSpace::FastMutex	MutexT;
		typedef MutexT::ScopeLock		LockT;
		typedef NetSpace::SockStream	SockStreamT;
		typedef NetSpace::InetAddress	AddrT;
private:
		const TaskID			m_task_uni_id;
		PeerEntry				m_peer_entry;
private:
		mutable MutexT			m_send_buf_mutex;
		BufferT					m_send_buf;
		BufferT					m_recv_buf;
		SockStreamT				m_sock;
		t_uint32				m_remain_len;
private:
		RecvState				m_state;
private:
		void send_conn_abort();
		
		void send_io_event(const t_byte *pbuf, size_t len);
private:
		virtual NetSpace::NetHandle GetHandle()const;
		
		virtual void OnInput();

		virtual void OnOutput();

		//virtual void OnException();

		virtual void OnTimer();
public:
		PeerIO(NetSpace::NetHandle net_hdl, TaskID task_uni_id, const PeerEntry &pe);
		

		//任何在此析构函数中涉及到GetSelector()的操作都会挂掉
		virtual ~PeerIO();
public:
		size_t GetSendBufferSize()const;
		
		size_t WriteData(const t_byte *pbuf, size_t len);
};




}