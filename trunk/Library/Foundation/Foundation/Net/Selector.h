
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
#include "NetEventHandler.h"


namespace NetSpace {


//我暂时会采用这样一种timeout的策略，每次达到Run的timeout参数指定的时间都会响应
//所有已注册TIMER_MASK标记的handler的OnTimer函数,以检测到底过了多少时间,
//我认为这个timeout的值在250毫秒或者更低为好

/*这个类完全不能用,因为其基于一个未考虑到的事情，就是当前socket被另一个线程remove并关闭后，内部存留一个正要被处理的socket,此时注册进来一个值相同的socket句柄*/

class FoundationAPI Selector : NonCopyable
{
private:
		typedef std::map<NetHandle, NetEventHandler*>	HandlerMap;
		typedef ThreadSpace::FastMutex				MutexType;
		//typedef ThreadSpace::RecursiveSpinLock			MutexType;
		typedef MutexType::ScopeLock					Lock;
private:
		HandlerMap						m_map;
		volatile bool					m_is_running;
		mutable MutexType				m_mutex;
private:
		//这几个函数都要防止一种很XX的现象，就是调用时丫把m_map给改了
		//（注册或删除自身或其它handler)
		bool dispatch_read(const NetHandle &nhdl);

		bool dispatch_write(const NetHandle &nhdl);

		bool dispatch_expt(const NetHandle  &nhdl);

		bool dispatch_timer(const NetHandle  &nhdl);

		void on_timer();
public:
		void Run(t_uint64 timer_interval = 250); //必须有一个超时值存在,毫秒
		
		void Stop();

		bool IsRunning()const;

		bool RegisterHandler(NetEventHandler *handler);
		
		//它的语意应该是在此之后selector绝不会调用其任何成员函数了
		bool RemoveHandler(NetEventHandler *handler);

		void RemoveAll();

		size_t Count()const;
};

}