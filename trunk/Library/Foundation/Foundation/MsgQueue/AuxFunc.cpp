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


#include "AuxFunc.h"

#include "MsgCarrier.h"

#include "GlobalMap.h"


namespace MsgSpace {


////////////////////////////////////////////////////////////////////////////////////////

bool SendMessageEx(t_ulong tid, Message *pmsg,  void *&pres, t_ulong priority)throw()
{
		assert(pmsg != 0);
		pres = 0;
		if(tid == ThreadSpace::Thread::GetCurrentThreadId())
		{
				delete pmsg;
				assert(0);
				return false;
		}

		MsgCarrier *pmsg_carrier = new MsgCarrier;
		pmsg_carrier->pmsg = pmsg;
		pmsg_carrier->m_msg_event = new SyncMsgEvent();
		pmsg_carrier->Duplicate();
		assert(pmsg_carrier->Count() == 2);
		
		if(!PushMsg(tid, pmsg_carrier, priority))
		{
				pmsg_carrier->Release();
				pmsg_carrier->Release();
				return false;
		}else
		{
				pmsg_carrier->m_msg_event->Wait();
				pres = pmsg_carrier->m_msg_event->GetResult();
				pmsg_carrier->Release();
				return true;
		}
		

}

bool SendMessageTimeoutEx(t_ulong tid, Message *pmsg, t_ulong timeout, void *&pres, t_ulong priority)throw()
{
		pres = 0;
		assert(pmsg != 0);
		
		if(tid == ThreadSpace::Thread::GetCurrentThreadId())
		{
				//pmsg->Delete();
				delete pmsg;
				assert(false);
				return false;
		}
		MsgCarrier *pmsg_carrier = new MsgCarrier;
		pmsg_carrier->pmsg = pmsg;
		pmsg_carrier->m_msg_event = new SyncMsgEvent();
		pmsg_carrier->Duplicate();	//增加一次引用计数,以此也保证了pmsg_carrier->m_msg_event
		assert(pmsg_carrier->Count() == 2);	//不会在处理线程中被delete,因为在~Message()中执行delete

		if(PushMsg(tid, pmsg_carrier, priority))
		{
				if(pmsg_carrier->m_msg_event->Wait(timeout))
				{
						//如果被处理了,则得到返回结果
						pres = pmsg_carrier->m_msg_event->GetResult();
						pmsg_carrier->Release();
						return true;
				}else
				{
						pmsg_carrier->Release();
						return false;
				}
				
		}else
		{
				pmsg_carrier->Release();		
				pmsg_carrier->Release();
				return false;
		}

}

bool PostMessageEx(t_ulong tid, Message *pmsg, t_ulong priority)
{
		assert(pmsg != 0);
		
		MsgCarrier *pmsg_carrier = new MsgCarrier;
		
		pmsg_carrier->pmsg = pmsg;
		pmsg_carrier->m_msg_event = 0;

		bool has_queue = PushMsg(tid, pmsg_carrier, priority);
		if(!has_queue)
		{
				pmsg_carrier->Release();
		}

		return has_queue;
}













}

