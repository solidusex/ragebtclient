
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
#include "../Common/ComnHdr.h"

#include "../Utility/UtilityHdr.h"


namespace ThreadSpace {

class EventImpl;

//注意,这个实现保证的是,在任何平台下,当is_auto== false 时侯,任何的一次setevent操作之后,如果
//不调用reset操作,那么之后的wait函数,都必须立即提示等待成功,这个主要是因为pthread的条件变量
//和win32中的event定义不相同,pthread_cond_signal如果没有发现有人等待,会直接返回,下次的等待
//会造成严重的bug,影响到本库很多部件,因此,个版本的实现必须保证signal和wait的次序无关性

class  FoundationAPI  Event : private NonCopyable
{
private:
		EventImpl	*m_pimpl;
public:
		Event(bool is_auto = false);
		~Event();
		void Wait();
		bool Wait(t_uint64 timeout);
		bool TryWait();
		void Set();
		void Reset();
};



}
