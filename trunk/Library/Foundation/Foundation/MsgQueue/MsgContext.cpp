
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

#include "MsgCarrier.h"
#include "GlobalMap.h"
#include "MsgContext.h"

namespace MsgSpace {





class MsgContextImpl : NonCopyable
{
private:
		MsgCarrier		*m_curr_msg;
		TLMsgQueue		m_msg_queue;
public:
		MsgContextImpl();

		~MsgContextImpl();
		
		//在这个Message使用结束之前，不可以调用下一个GetMessageEx()
		Message* GetMessageEx();

		Message* GetMessageEx(t_uint32 timeout); //毫秒

		const Message* PeekMessageEx();


		//强制性处理消息,
		//如果是sendmessage传入的,如果希望返回结果,则必须用ReplyMessageEx的参数
		//res返回这个结果
		//如果当前消息是postmessageex传入的,则删除消息;m_curr_msg = 0;
		bool ReplyMessageEx(void *res = 0);

};





const Message* MsgContextImpl::PeekMessageEx()
{
		//assert(IsExisted());
		ReplyMessageEx(0);//将前一个消息处理掉,如果没被处理的话,对于这个操作的话,无所谓这么做;

		void *ptr = 0;
		if(m_msg_queue.PeekQueue(ptr))
		{
				assert(ptr != 0);
				return ((MsgCarrier*)ptr)->pmsg;
		}else
		{
				return 0;
		}
}


Message* MsgContextImpl::GetMessageEx()
{
		//assert(IsExisted());
		ReplyMessageEx(0);//将前一个消息处理掉,如果没被处理的话
		
		void *ptr = 0;
		m_msg_queue.WaitDequeue(ptr);//阻塞
		assert(ptr != 0);
		m_curr_msg = (MsgCarrier*)ptr;
		assert(m_curr_msg != 0);						//永不为空
		return m_curr_msg->pmsg;
}

Message* MsgContextImpl::GetMessageEx(t_uint32 timeout)
{
		//assert(IsExisted());
		ReplyMessageEx(0);						//将前一个消息处理掉,如果没被处理的话
		
		void *ptr = 0;
		if(m_msg_queue.WaitDequeue(ptr, timeout))
		{
				assert(ptr != 0);
				m_curr_msg = (MsgCarrier*)ptr;
				assert(m_curr_msg != 0);				//永不为空
				return m_curr_msg->pmsg;
		}else
		{
				return 0;
		}
}


bool MsgContextImpl::ReplyMessageEx(void *res)
{
		//assert(IsExisted());
		if(m_curr_msg != 0)
		{

				if(m_curr_msg->m_msg_event != 0)
				{
						//总之,这里调用此msg release函数之后才会销毁,所以这里
						//m_msg_event决不会被delete
						m_curr_msg->m_msg_event->SetResult(res);
						m_curr_msg->m_msg_event->Set();
						
				}
				m_curr_msg->Release();
				m_curr_msg = 0;
				return true;

		}else
		{
				return false;
		}
}


MsgContextImpl::MsgContextImpl() : m_curr_msg(0)
{
		
		assert(!IsExisted());

		if(IsExisted())
		{
				throw ExceptionSpace::ExistsException("MsgContextImpl::MsgContextImpl aready existed\n", 0);
		}

		if(!RegisterMsgQueue(&m_msg_queue))
		{
				throw ExceptionSpace::IllegalStateException("Unknow Error : MsgContextImpl::MsgContextImpl : RegisterMsgQueue failed\n", 0);
		}
}

MsgContextImpl::~MsgContextImpl()
{
		try{
				assert(IsExisted());
				ReplyMessageEx(0);
				
				bool res = RemoveMsgQueue();
				assert(res);
				//这个函数调用之后,不可能有任何线程再次将Msg注册到这个线程的队列中
				while(PeekMessageEx() != 0)
				{
						GetMessageEx();//设定m_curr_msg;
						ReplyMessageEx(0);
				}
				assert(m_msg_queue.IsEmpty());
		}catch(const std::exception &expt)
		{
				DEBUG_PRINT1("Unknow exception occur in MsgContextImpl::~MsgContextImpl() error msg = %s\n", expt.what());
				assert(0);
		}
		
}

///////////////////////////////////////////////////////////////////////////////////////


Message* MsgContext::GetMessageEx()
{
		return m_pimpl->GetMessageEx();

}
Message* MsgContext::GetMessageEx(t_uint32 timeout)throw()
{
		return m_pimpl->GetMessageEx(timeout);
}


const Message* MsgContext::PeekMessageEx()
{
		return m_pimpl->PeekMessageEx();
}

bool MsgContext::ReplyMessageEx(void *res)
{
		return m_pimpl->ReplyMessageEx(res);

}


MsgContext::MsgContext() : m_pimpl(new MsgContextImpl())
{
		assert(m_pimpl != NULL);
}
		
MsgContext::~MsgContext()
{
		delete m_pimpl;
}


}











#if(0)
////////////////////////////////////////////////////////////////////////////////////////
t_ulong MsgContextImpl::SendMessageEx(t_ulong tid, Message *pmsg, t_ulong priority )
{
		return SendMessageTimeoutEx(tid, pmsg, INFINITE, priority);
}


t_ulong MsgContextImpl::SendMessageTimeoutEx(t_ulong tid, Message *pmsg, t_ulong timeout, t_ulong priority)
{
		
		assert(GlobalMsgQueueMap::g_msg_map != 0);
		assert(pmsg);
		
		if(tid == Thread::GetCurrentThreadId())
		{
				delete pmsg;
				assert(0);
				return 0;
		}
		MsgCarrier *pmsg_carrier = new MsgCarrier;
		pmsg_carrier->pmsg = pmsg;
		pmsg_carrier->m_msg_event = new SyncMsgEvent();
		pmsg_carrier->Duplicate();	//增加一次引用计数,以此也保证了pmsg_carrier->m_msg_event

		assert(pmsg_carrier->Count() == 2);	//不会在处理线程中被delete,因为在~Message()中执行delete
		
		bool has_queue = GlobalMsgQueueMap::g_msg_map->PushMsg(tid, pmsg_carrier, priority);
		if(has_queue)
		{
				t_ulong res = 0;
				
				if(pmsg_carrier->m_msg_event->Wait(timeout))
				{
						//如果被处理了,则得到返回结果
						res = pmsg_carrier->m_msg_event->GetResult();
				}else
				{
						//否则返回0;
						res = 0;
				}

				//这里执行之前决不会销毁这个msg和pmsg->m_msg_event;,执行之后
				//不能保证pmsg到底被删除了没有,看谁后调用的release了
				pmsg_carrier->Release();
				return res;
				
		}else
		{
				pmsg_carrier->Release();		//这里需要两次释放,因为根本没有压进队列中,所以没有处理方法会
												//调用release,故此需要销毁掉;
				pmsg_carrier->Release();
				return 0;
		}

}

bool MsgContextImpl::PostMessageEx(t_ulong tid, Message *pmsg, t_ulong priority)
{
		assert(pmsg);
		assert(GlobalMsgQueueMap::g_msg_map != 0);
		
		MsgCarrier *pmsg_carrier = new MsgCarrier;
		
		pmsg_carrier->pmsg = pmsg;
		pmsg_carrier->m_msg_event = 0;

		bool has_queue = GlobalMsgQueueMap::g_msg_map->PushMsg(tid, pmsg_carrier, priority);
		if(!has_queue)
		{
				pmsg_carrier->Release();
		}

		return has_queue;
}


#endif










