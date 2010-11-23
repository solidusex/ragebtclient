
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
#pragma once

#include "MsgType.h"
#include "TypeDef.h"


namespace MsgSpace {


//这个类决不应当放到某个线程的循环里面,因为,任何一次建构和析构都要到全局的MsgQueueMap中
//建立/删除一个消息队列,消耗非常大,最好应该在某个线程需要消息队列的时侯建立,
//然后在线程会在结束时候自动删除这个队列


class MsgContextImpl;

class FoundationAPI MsgContext : NonCopyable
{
private:
		static void* operator new(size_t size, void *ptr);

		static void* operator new(size_t size);
		static void operator delete(void *ptr);
		
		static void* operator new[](size_t size);
		static void operator delete[](void *ptr);
public:
		Message* GetMessageEx()throw();

		Message* GetMessageEx(t_uint32 timeout)throw(); //毫秒
		
		const Message* PeekMessageEx()throw();
		
		bool ReplyMessageEx(void *res = 0)throw();

public:
		MsgContext();
		
		~MsgContext();
private:
		MsgContextImpl			*m_pimpl;

};




}

#if(0)
/*public:
		static bool PostMessageEx(t_ulong tid, Message *pmsg, t_ulong priority = 1);

		static t_ulong SendMessageEx(t_ulong tid, Message *pmsg, t_ulong priority = MAX_LONG);

		static t_ulong SendMessageTimeoutEx(t_ulong tid, Message *pmsg, t_ulong timeout, t_ulong priority = MAX_LONG);*/

/*#define PostMsgEx TLMsgContext::PostMessageEx

#define SendMsgEx TLMsgContext::SendMessageEx

#define SendMsgTimeoutEx TLMsgContext::SendMessageTimeoutEx*/
#endif
