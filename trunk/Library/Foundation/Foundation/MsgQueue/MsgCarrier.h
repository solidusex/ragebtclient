
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
#include "MsgType.h"

namespace MsgSpace {


class SyncMsgEvent : NonCopyable
{
private:
		typedef ThreadSpace::Event	Event;
private:
		void			*m_res;
		Event			m_event;
public:
		SyncMsgEvent();
		~SyncMsgEvent();
public:
		void SetResult(void *res);
		
		bool Wait(t_ulong timeout);

		void Wait();

		void Set();

		void* GetResult()const;
};



class MsgCarrier : public RefCountedObject
{
public:
		Message			*pmsg;

		SyncMsgEvent	*m_msg_event;
public:
		MsgCarrier();
		
		virtual ~MsgCarrier();

};




}