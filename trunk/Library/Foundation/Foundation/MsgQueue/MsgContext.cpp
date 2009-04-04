
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
		
		//�����Messageʹ�ý���֮ǰ�������Ե�����һ��GetMessageEx()
		Message* GetMessageEx();

		Message* GetMessageEx(t_uint32 timeout); //����

		const Message* PeekMessageEx();


		//ǿ���Դ�����Ϣ,
		//�����sendmessage�����,���ϣ�����ؽ��,�������ReplyMessageEx�Ĳ���
		//res����������
		//�����ǰ��Ϣ��postmessageex�����,��ɾ����Ϣ;m_curr_msg = 0;
		bool ReplyMessageEx(void *res = 0);

};





const Message* MsgContextImpl::PeekMessageEx()
{
		//assert(IsExisted());
		ReplyMessageEx(0);//��ǰһ����Ϣ�����,���û������Ļ�,������������Ļ�,����ν��ô��;

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
		ReplyMessageEx(0);//��ǰһ����Ϣ�����,���û������Ļ�
		
		void *ptr = 0;
		m_msg_queue.WaitDequeue(ptr);//����
		assert(ptr != 0);
		m_curr_msg = (MsgCarrier*)ptr;
		assert(m_curr_msg != 0);						//����Ϊ��
		return m_curr_msg->pmsg;
}

Message* MsgContextImpl::GetMessageEx(t_uint32 timeout)
{
		//assert(IsExisted());
		ReplyMessageEx(0);						//��ǰһ����Ϣ�����,���û������Ļ�
		
		void *ptr = 0;
		if(m_msg_queue.WaitDequeue(ptr, timeout))
		{
				assert(ptr != 0);
				m_curr_msg = (MsgCarrier*)ptr;
				assert(m_curr_msg != 0);				//����Ϊ��
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
						//��֮,������ô�msg release����֮��Ż�����,��������
						//m_msg_event�����ᱻdelete
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
				//�����������֮��,���������κ��߳��ٴν�Msgע�ᵽ����̵߳Ķ�����
				while(PeekMessageEx() != 0)
				{
						GetMessageEx();//�趨m_curr_msg;
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
		pmsg_carrier->Duplicate();	//����һ�����ü���,�Դ�Ҳ��֤��pmsg_carrier->m_msg_event

		assert(pmsg_carrier->Count() == 2);	//�����ڴ����߳��б�delete,��Ϊ��~Message()��ִ��delete
		
		bool has_queue = GlobalMsgQueueMap::g_msg_map->PushMsg(tid, pmsg_carrier, priority);
		if(has_queue)
		{
				t_ulong res = 0;
				
				if(pmsg_carrier->m_msg_event->Wait(timeout))
				{
						//�����������,��õ����ؽ��
						res = pmsg_carrier->m_msg_event->GetResult();
				}else
				{
						//���򷵻�0;
						res = 0;
				}

				//����ִ��֮ǰ�������������msg��pmsg->m_msg_event;,ִ��֮��
				//���ܱ�֤pmsg���ױ�ɾ����û��,��˭����õ�release��
				pmsg_carrier->Release();
				return res;
				
		}else
		{
				pmsg_carrier->Release();		//������Ҫ�����ͷ�,��Ϊ����û��ѹ��������,����û�д�������
												//����release,�ʴ���Ҫ���ٵ�;
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










