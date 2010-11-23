/*
 * The Foundation Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  It is provided "as is" without express
 * or implied warranty.
 */


#include "GlobalMap.h"

namespace MsgSpace {


using namespace ThreadSpace;





typedef ReaderWriterMutex						MutexType;

typedef MutexType::ScopeLock					Lock;

typedef std::map<t_ulong, TLMsgQueue*>			MsgQueueMap;

static MutexType		g_mutex;

static MsgQueueMap		g_map;



bool IsExisted()
{
		t_ulong tid = Thread::GetCurrentThreadId();
		assert(tid != 0);
		Lock lock(g_mutex, true);
		return (g_map.count(tid) != 0);

}


//�޸�������ʱ�����Ͳ�ѯ�����ǻ���ĵĶ�����������ǻ����
bool RegisterMsgQueue(TLMsgQueue *pmq)
{
		assert(pmq != 0);
		if(IsExisted())
		{
				return false;
		}else
		{
				//������������һ���߳�Ҳ����tid == ����̵߳�id�Ĳ���
				t_ulong tid = Thread::GetCurrentThreadId();
				//�޸�ȫ��ӳ����ʱ������ǻ����;
				{
						//MutexType::ScopeLock lock(m_mutex);
						//�����д����
						Lock lock(g_mutex, false); //lock for write
						g_map[tid] = pmq;
				}
				return true;
		}
}



//ִ�����������ͬʱ,��������һ���ͱ��߳���ص�PopMsg����
bool RemoveMsgQueue()
{
		if(!IsExisted())//������̸߳��������ڶ���,�򷵻�false;
		{
				//ע��,������Ϊ���������Ҳ������������߳��ڴ�ʱ���������м����������,
				//��Ϊ��ǰ�̲߳�δ����������������ͬ��id���߳�ִ��RegisterMsgQueue
				//RegisterMsgQueue�����������ֻ�ܲ������̵߳���صĶ���
				return false;

		}else
		{
				//�߳�ID��һ������ϵͳ�������������
				//static int count = 0;
				//return count++;
				//��Ҫ����ͬһʱ���ڲ����������̴߳�����ͬID��

				t_ulong tid = Thread::GetCurrentThreadId();		
				
				//MutexType::ScopeLock lock(m_mutex);
				//���������д����,��������ж���������
				Lock lock(g_mutex, false);
				g_map.erase(tid);//remove֮��Ͳ������߳������������Ͷ����Ϣ��;
				//��ʱ���������߳���������з�����Ϣ������У�
				//��ô��һ��sendmessage/postmessage���߳�Ҫô�������������,
				//�������뵽�����������,Ҫô�ȴ����ɾ��tid�Ĳ���������
				//��ô�Ǹ�������Ϣ���߳̽�����false;,����ʧ��,
				//����,ִ�����������֮��Ľ�û���߳����򱾶��з�����Ϣ;
				//������ʱ�������κ��̷߳��ʵ�������� ��Ϊ���е�send����Ҫôʧ�ܣ�
				//Ҫô���Ѿ����͵������������
				return true;
		}
}


//����������뱣֤��msgѹ����еĹ�����,ӵ��������е��̲߳�����������ɾ����
bool PushMsg(t_ulong tid, MsgCarrier *pmsg, t_ulong priority)
{
		Lock lcok(g_mutex, true);//ֻ��
		
		MsgQueueMap::iterator it = g_map.find(tid);
		if(it != g_map.end())
		{
				it->second->Enqueue(pmsg, priority);
				return true;
		}else
		{
				return false;
		}
}

/*
//�������Ҫ��֤�����ڼ���Ƿ������Ӧ�ı��̶߳���ʱû�����޸����ӳ���
//�����contextһ��δ�����msg,������Ϣ�ķ�������˵,
//�����Ϣ��û������
MsgCarrier* PopMsg()
{
		//���������,���ʱ����߳̾�������������,��Ϊ������̱߳�����˵
		//���ﲢû�й����κ�����;
		assert(IsExisted());

		
		t_ulong tid = Thread::GetCurrentThreadId();
		TLMsgQueue *pqueue = 0;
		assert(tid != 0);

		{
				Lock lock(g_mutex, true);
				pqueue= g_map[tid];
				//����������ڶ�����
				assert(pqueue);
		}

		return pqueue->PopMsg();//�����������ʱ��,�ᱻ����,
		//��Ϊ��������϶���������ͷ�������е��߳��ڵ���
		//,�������������ǲ��ᵼ�¾�̬�����,��Ϊ���pqueue�϶���������߳�
		//�б�delete��,�μ�RemoveMsgQueue
		//��ʱֻ��������������з��͵��߳�


}


const MsgCarrier* PeekMsg()
{
		assert(IsExisted());
		t_ulong tid = Thread::GetCurrentThreadId();
		Lock lock(g_mutex, true);
		return g_map[tid]->PeekQueue();
}
*/

}







#if(0)

class GlobalMsgQueueMap : private Singleton<GlobalMsgQueueMap>
{
private:
		typedef ThreadSpace::Thread				Thread;
		typedef ThreadSpace::Event				Event;
		typedef ThreadSpace::ReaderWriterMutex	MutexType;
		typedef std::map<t_ulong, TLMsgQueue*>	MsgQueueMap;
private:
		friend bool PostMessageEx(t_ulong, Message*, t_ulong)throw();

		friend t_ulong SendMessageEx(t_ulong, Message*, t_ulong)throw();

		friend t_ulong SendMessageTimeoutEx(t_ulong, Message*, t_ulong, t_ulong)throw();
		
		friend class MsgContextImpl;

		friend class Singleton<GlobalMsgQueueMap>;
		
		using Singleton<GlobalMsgQueueMap>::UnInstance;

private:
		GlobalMsgQueueMap();
private:
		mutable MutexType		m_mutex;
		MsgQueueMap				m_map;
private:
		bool IsExisted()const;
		
		bool RegisterMsgQueue(TLMsgQueue *pmq);
		
		bool RemoveMsgQueue();

		bool PushMsg(t_ulong tid, MsgCarrier *pmc, t_ulong priority);

		MsgCarrier* PopMsg();

		const MsgCarrier* PeekMsg();
public:
		~GlobalMsgQueueMap();
private:
		static GlobalMsgQueueMap		*g_msg_map;
};

/***********************************GlobalMsgQueueMap***********************/

GlobalMsgQueueMap* GlobalMsgQueueMap::g_msg_map = &GlobalMsgQueueMap::Instance();

GlobalMsgQueueMap::GlobalMsgQueueMap()
{

}

GlobalMsgQueueMap::~GlobalMsgQueueMap()
{

}

const MsgCarrier* GlobalMsgQueueMap::PeekMsg()
{
		assert(IsExisted());
		t_ulong tid = Thread::GetCurrentThreadId();
		SCOPE_GUARD(&m_mutex, &MutexType::WaitToRead, &MutexType::Done);
		return m_map[tid]->PeekQueue();
		

}

//����������뱣֤��msgѹ����еĹ�����,ӵ��������е��̲߳�����������ɾ����
bool GlobalMsgQueueMap::PushMsg(t_ulong tid, MsgCarrier *pmsg, t_ulong priority)
{
		SCOPE_GUARD(&m_mutex, &MutexType::WaitToRead, &MutexType::Done);
		
		MsgQueueMap::iterator it = m_map.find(tid);
		if(it != m_map.end())
		{
				it->second->PushMsg(pmsg, priority);
				return true;
		}else
		{
				return false;
		}
}


//�������Ҫ��֤�����ڼ���Ƿ������Ӧ�ı��̶߳���ʱû�����޸����ӳ���
//�����contextһ��δ�����msg,������Ϣ�ķ�������˵,
//�����Ϣ��û������
MsgCarrier* GlobalMsgQueueMap::PopMsg()
{
		//���������,���ʱ����߳̾�������������,��Ϊ������̱߳�����˵
		//���ﲢû�й����κ�����;
		assert(IsExisted());

		
		t_ulong tid = Thread::GetCurrentThreadId();
		TLMsgQueue *pqueue = 0;
		assert(tid != 0);

		{
				//MutexType::ScopeLock lock(m_mutex);
				SCOPE_GUARD(&m_mutex, &MutexType::WaitToRead, &MutexType::Done);
				pqueue= m_map[tid];
				//����������ڶ�����
				assert(pqueue);
		}

		return pqueue->PopMsg();//�����������ʱ��,�ᱻ����,
		//��Ϊ��������϶���������ͷ�������е��߳��ڵ���
		//,�������������ǲ��ᵼ�¾�̬�����,��Ϊ���pqueue�϶���������߳�
		//�б�delete��,�μ�RemoveMsgQueue
		//��ʱֻ��������������з��͵��߳�


}

bool GlobalMsgQueueMap::IsExisted()const
{
		t_ulong tid = Thread::GetCurrentThreadId();
		assert(tid != 0);
		//MutexType::ScopeLock lock(m_mutex);
		SCOPE_GUARD(&m_mutex, &MutexType::WaitToRead, &MutexType::Done);//������
		return (m_map.count(tid) != 0);
}

//�޸�������ʱ�����Ͳ�ѯ�����ǻ���ĵĶ�����������ǻ����
bool GlobalMsgQueueMap::RegisterMsgQueue(TLMsgQueue *pmq)
{
		assert(pmq != 0);
		if(IsExisted())
		{
				return false;
		}else
		{
				//������������һ���߳�Ҳ����tid == ����̵߳�id�Ĳ���
				t_ulong tid = Thread::GetCurrentThreadId();
				//�޸�ȫ��ӳ����ʱ������ǻ����;
				{
						//MutexType::ScopeLock lock(m_mutex);
						//�����д����
						SCOPE_GUARD(&m_mutex, &MutexType::WaitToWrite, &MutexType::Done);
						m_map[tid] = pmq;
				}
				return true;
		}
}


//ִ�����������ͬʱ,��������һ���ͱ��߳���ص�PopMsg����
bool GlobalMsgQueueMap::RemoveMsgQueue()
{
		if(!IsExisted())//������̸߳��������ڶ���,�򷵻�false;
		{
				//ע��,������Ϊ���������Ҳ������������߳��ڴ�ʱ���������м����������,
				//��Ϊ��ǰ�̲߳�δ����������������ͬ��id���߳�ִ��RegisterMsgQueue
				//RegisterMsgQueue�����������ֻ�ܲ������̵߳���صĶ���
				return false;

		}else
		{
				//�߳�ID��һ������ϵͳ�������������
				//static int count = 0;
				//return count++;
				//��Ҫ����ͬһʱ���ڲ����������̴߳�����ͬID��

				t_ulong tid = Thread::GetCurrentThreadId();		
				
				//MutexType::ScopeLock lock(m_mutex);
				//���������д����,��������ж���������
				SCOPE_GUARD(&m_mutex, &MutexType::WaitToWrite, &MutexType::Done);
				m_map.erase(tid);//remove֮��Ͳ������߳������������Ͷ����Ϣ��;
				//��ʱ���������߳���������з�����Ϣ������У�
				//��ô��һ��sendmessage/postmessage���߳�Ҫô�������������,
				//�������뵽�����������,Ҫô�ȴ����ɾ��tid�Ĳ���������
				//��ô�Ǹ�������Ϣ���߳̽�����false;,����ʧ��,
				//����,ִ�����������֮��Ľ�û���߳����򱾶��з�����Ϣ;
				//������ʱ�������κ��̷߳��ʵ�������� ��Ϊ���е�send����Ҫôʧ�ܣ�
				//Ҫô���Ѿ����͵������������
				return true;
		}
}
#endif








#if(0)
bool GlobalMsgQueueMap::PushMsg(t_ulong tid, MsgCarrier *pmsg, t_ulong priority)
{
		/*
		������һ���ܴ������,������û���m_map[tid]��queue,��ô���queue�������߳�
		��������ʱ�����queueɾ����,��ô��core dump,
		*/
		SCOPE_GUARD(&m_mutex, &MutexType::WaitToRead, &MutexType::Done);
		if(m_map.count(tid) == 0)
		{
				return false;
		}else
		{
				m_map[tid]->PushMsg(pmsg, priority);
				return true;
		}
}
#endif