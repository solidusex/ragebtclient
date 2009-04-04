
/*
 * The Foundation Library
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
 */
#pragma once

#include "TypeDef.h"
#include "NetEventHandler.h"


namespace NetSpace {


//����ʱ���������һ��timeout�Ĳ��ԣ�ÿ�δﵽRun��timeout����ָ����ʱ�䶼����Ӧ
//������ע��TIMER_MASK��ǵ�handler��OnTimer����,�Լ�⵽�׹��˶���ʱ��,
//����Ϊ���timeout��ֵ��250������߸���Ϊ��

/*�������ȫ������,��Ϊ�����һ��δ���ǵ������飬���ǵ�ǰsocket����һ���߳�remove���رպ��ڲ�����һ����Ҫ�������socket,��ʱע�����һ��ֵ��ͬ��socket���*/

class FoundationAPI Selector : NonCopyable
{
private:
		typedef std::map<NetHandle, NetEventHandler*>	HandlerMap;
		typedef ThreadSpace::FastMutex				MutexType;
		//typedef ThreadSpace::RecursiveSpinLock			MutexType;
		typedef MutexType::ScopeLock					Lock;
private:
		HandlerMap						m_map;
		volatile bool					m_is_running;
		mutable MutexType				m_mutex;
private:
		//�⼸��������Ҫ��ֹһ�ֺ�XX�����󣬾��ǵ���ʱѾ��m_map������
		//��ע���ɾ�����������handler)
		bool dispatch_read(const NetHandle &nhdl);

		bool dispatch_write(const NetHandle &nhdl);

		bool dispatch_expt(const NetHandle  &nhdl);

		bool dispatch_timer(const NetHandle  &nhdl);

		void on_timer();
public:
		void Run(t_uint64 timer_interval = 250); //������һ����ʱֵ����,����
		
		void Stop();

		bool IsRunning()const;

		bool RegisterHandler(NetEventHandler *handler);
		
		//��������Ӧ�����ڴ�֮��selector������������κγ�Ա������
		bool RemoveHandler(NetEventHandler *handler);

		void RemoveAll();

		size_t Count()const;
};

}