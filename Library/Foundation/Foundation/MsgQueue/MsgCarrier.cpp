
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
#include "MsgCarrier.h"


namespace MsgSpace {




SyncMsgEvent::SyncMsgEvent() : m_res(0), m_event(false)
{

}

SyncMsgEvent::~SyncMsgEvent()
{
		
}


void SyncMsgEvent::SetResult(void *res)
{
		m_res = res;
}

bool SyncMsgEvent::Wait(t_ulong timeout)
{
		return m_event.Wait(timeout);
}

void SyncMsgEvent::Wait()
{
		m_event.Wait();
}

void SyncMsgEvent::Set()
{
		m_event.Set();
}

void* SyncMsgEvent::GetResult()const
{
		return m_res;
}


/**********************************Message****************************/


MsgCarrier::MsgCarrier() : m_msg_event(0), pmsg(0)
{
		
}

MsgCarrier::~MsgCarrier()
{
		try{
				delete m_msg_event;
				//pmsg->Delete();
				delete pmsg;
				
		}catch(const std::exception &expt)
		{
				assert(0);
				DEBUG_PRINT1("Some exception throw in MsgCarrier::~MsgCarrier() msg == %s\n", expt.what());
		}
		
}


}