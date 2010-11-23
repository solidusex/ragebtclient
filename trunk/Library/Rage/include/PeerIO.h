
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



/*ʵ���ϴ�������������߳��й�ͬ���������ղ��ָ���������ݰ���
  �����͵�������У����Ͳ��ֽ����߼��̲߳��֣������ͣ�����ʱ�Ὣ����д��
  send_buf�У��Ǹ�����io�����߳̽�ÿ��N������һ���Ƿ��пɷ������ݣ�������
  Ҳ����˵����on_timer�м��m_send_buf�Ƿ�Ϊ�գ���Ϊ�յĻ����ͽ�timer��������
  ��������on_write_mask��֮��ʼд�룬��������д��send_bufΪ��ʱ
  �������Զ���on_write_mask�����������on_timer_mask��ǣ��������
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
		

		//�κ��ڴ������������漰��GetSelector()�Ĳ�������ҵ�
		virtual ~PeerIO();
public:
		size_t GetSendBufferSize()const;
		
		size_t WriteData(const t_byte *pbuf, size_t len);
};




}