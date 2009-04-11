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


//修改这个表的时侯必须和查询动作是互斥的的动作本身必须是互斥的
bool RegisterMsgQueue(TLMsgQueue *pmq)
{
		assert(pmq != 0);
		if(IsExisted())
		{
				return false;
		}else
		{
				//不可能有另外一个线程也加入tid == 这个线程的id的操作
				t_ulong tid = Thread::GetCurrentThreadId();
				//修改全局映射表的时候必须是互斥的;
				{
						//MutexType::ScopeLock lock(m_mutex);
						//这个是写操作
						Lock lock(g_mutex, false); //lock for write
						g_map[tid] = pmq;
				}
				return true;
		}
}



//执行这个操作的同时,不可能有一个和本线程相关的PopMsg存在
bool RemoveMsgQueue()
{
		if(!IsExisted())//如果本线程根本不存在队列,则返回false;
		{
				//注意,这里因为如果不存在也不会有另外的线程在此时向整个表中加入这个队列,
				//因为当前线程并未结束，不可能有相同的id的线程执行RegisterMsgQueue
				//RegisterMsgQueue和这个函数的只能操作本线程的相关的队列
				return false;

		}else
		{
				//线程ID不一定依赖系统本身，例如可以是
				//static int count = 0;
				//return count++;
				//主要是在同一时间内不可有两个线程存在相同ID；

				t_ulong tid = Thread::GetCurrentThreadId();		
				
				//MutexType::ScopeLock lock(m_mutex);
				//这个调用是写操作,必须和所有读操作互斥
				Lock lock(g_mutex, false);
				g_map.erase(tid);//remove之后就不会有线程在向这个队列投递消息了;
				//此时，可能有线程向这个队列发送消息，如果有，
				//那么那一个sendmessage/postmessage的线程要么检测存在这个队列,
				//并发加入到这个队列中来,要么等待这个删除tid的操作结束，
				//那么那个发送消息的线程将返回false;,发送失败,
				//所以,执行完这个操作之后的将没有线程再向本队列发送消息;
				//在这里时不会有任何线程访问到这个队列 因为所有的send请求要么失败，
				//要么就已经发送到这个队列中了
				return true;
		}
}


//这个函数必须保证在msg压入队列的过程中,拥有这个队列的线程不会把这个队列删除掉
bool PushMsg(t_ulong tid, MsgCarrier *pmsg, t_ulong priority)
{
		Lock lcok(g_mutex, true);//只读
		
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
//这个函数要保证的是在检测是否存在相应的本线程队列时没有人修改这个映射表
//负责给context一个未处理的msg,对于消息的发送者来说,
//这个消息还没处理处理
MsgCarrier* PopMsg()
{
		//如果不存在,则此时这个线程决不会加入这个表,因为相对于线程本身来说
		//这里并没有共享任何数据;
		assert(IsExisted());

		
		t_ulong tid = Thread::GetCurrentThreadId();
		TLMsgQueue *pqueue = 0;
		assert(tid != 0);

		{
				Lock lock(g_mutex, true);
				pqueue= g_map[tid];
				//这个动作属于读操作
				assert(pqueue);
		}

		return pqueue->PopMsg();//流动到这里的时候,会被阻塞,
		//因为这个函数肯定是申请和释放这个队列的线程在调用
		//,所以这里阻塞是不会导致竟态问题的,因为这个pqueue肯定是在这个线程
		//中被delete的,参见RemoveMsgQueue
		//此时只可能有向这个队列发送的线程


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

//这个函数必须保证在msg压入队列的过程中,拥有这个队列的线程不会把这个队列删除掉
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


//这个函数要保证的是在检测是否存在相应的本线程队列时没有人修改这个映射表
//负责给context一个未处理的msg,对于消息的发送者来说,
//这个消息还没处理处理
MsgCarrier* GlobalMsgQueueMap::PopMsg()
{
		//如果不存在,则此时这个线程决不会加入这个表,因为相对于线程本身来说
		//这里并没有共享任何数据;
		assert(IsExisted());

		
		t_ulong tid = Thread::GetCurrentThreadId();
		TLMsgQueue *pqueue = 0;
		assert(tid != 0);

		{
				//MutexType::ScopeLock lock(m_mutex);
				SCOPE_GUARD(&m_mutex, &MutexType::WaitToRead, &MutexType::Done);
				pqueue= m_map[tid];
				//这个动作属于读操作
				assert(pqueue);
		}

		return pqueue->PopMsg();//流动到这里的时候,会被阻塞,
		//因为这个函数肯定是申请和释放这个队列的线程在调用
		//,所以这里阻塞是不会导致竟态问题的,因为这个pqueue肯定是在这个线程
		//中被delete的,参见RemoveMsgQueue
		//此时只可能有向这个队列发送的线程


}

bool GlobalMsgQueueMap::IsExisted()const
{
		t_ulong tid = Thread::GetCurrentThreadId();
		assert(tid != 0);
		//MutexType::ScopeLock lock(m_mutex);
		SCOPE_GUARD(&m_mutex, &MutexType::WaitToRead, &MutexType::Done);//读操作
		return (m_map.count(tid) != 0);
}

//修改这个表的时侯必须和查询动作是互斥的的动作本身必须是互斥的
bool GlobalMsgQueueMap::RegisterMsgQueue(TLMsgQueue *pmq)
{
		assert(pmq != 0);
		if(IsExisted())
		{
				return false;
		}else
		{
				//不可能有另外一个线程也加入tid == 这个线程的id的操作
				t_ulong tid = Thread::GetCurrentThreadId();
				//修改全局映射表的时候必须是互斥的;
				{
						//MutexType::ScopeLock lock(m_mutex);
						//这个是写操作
						SCOPE_GUARD(&m_mutex, &MutexType::WaitToWrite, &MutexType::Done);
						m_map[tid] = pmq;
				}
				return true;
		}
}


//执行这个操作的同时,不可能有一个和本线程相关的PopMsg存在
bool GlobalMsgQueueMap::RemoveMsgQueue()
{
		if(!IsExisted())//如果本线程根本不存在队列,则返回false;
		{
				//注意,这里因为如果不存在也不会有另外的线程在此时向整个表中加入这个队列,
				//因为当前线程并未结束，不可能有相同的id的线程执行RegisterMsgQueue
				//RegisterMsgQueue和这个函数的只能操作本线程的相关的队列
				return false;

		}else
		{
				//线程ID不一定依赖系统本身，例如可以是
				//static int count = 0;
				//return count++;
				//主要是在同一时间内不可有两个线程存在相同ID；

				t_ulong tid = Thread::GetCurrentThreadId();		
				
				//MutexType::ScopeLock lock(m_mutex);
				//这个调用是写操作,必须和所有读操作互斥
				SCOPE_GUARD(&m_mutex, &MutexType::WaitToWrite, &MutexType::Done);
				m_map.erase(tid);//remove之后就不会有线程在向这个队列投递消息了;
				//此时，可能有线程向这个队列发送消息，如果有，
				//那么那一个sendmessage/postmessage的线程要么检测存在这个队列,
				//并发加入到这个队列中来,要么等待这个删除tid的操作结束，
				//那么那个发送消息的线程将返回false;,发送失败,
				//所以,执行完这个操作之后的将没有线程再向本队列发送消息;
				//在这里时不会有任何线程访问到这个队列 因为所有的send请求要么失败，
				//要么就已经发送到这个队列中了
				return true;
		}
}
#endif








#if(0)
bool GlobalMsgQueueMap::PushMsg(t_ulong tid, MsgCarrier *pmsg, t_ulong priority)
{
		/*
		这里是一个很大的问题,如果先置换出m_map[tid]的queue,那么这个queue所属的线程
		如果在这个时候将这个queue删除了,那么会core dump,
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