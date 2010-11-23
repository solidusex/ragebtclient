
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


//��������Ӧ���ŵ�ĳ���̵߳�ѭ������,��Ϊ,�κ�һ�ν�����������Ҫ��ȫ�ֵ�MsgQueueMap��
//����/ɾ��һ����Ϣ����,���ķǳ���,���Ӧ����ĳ���߳���Ҫ��Ϣ���е�ʱ���,
//Ȼ�����̻߳��ڽ���ʱ���Զ�ɾ���������


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

		Message* GetMessageEx(t_uint32 timeout)throw(); //����
		
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
