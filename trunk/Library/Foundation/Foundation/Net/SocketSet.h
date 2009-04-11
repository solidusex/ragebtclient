
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

#include "TypeDef.h"
#include "NetSock.h"
#include <set>

namespace NetSpace {


class FoundationAPI FdSetIterator : NonCopyable
{
private:
		fd_set			m_set;
		size_t			m_curr;
public:
		FdSetIterator(const fd_set & sock_set);

		~FdSetIterator();
public:
		void Reset(const fd_set & sock_set);

		void First();
		
		void Next();

		bool IsDone()const;

		NetHandle Current()const;
};




class FoundationAPI SocketSet
{
public:
		typedef std::vector<NetHandle>		ActiveSockSet;
public:
		SocketSet();
		
		~SocketSet();
		
		SocketSet(const fd_set &mask);

		SocketSet(const SocketSet& other);

		SocketSet& operator=(const SocketSet& other);

public:
		void Reset (void);

		void Reset(const fd_set &mask);

		bool ISSet(NetHandle sock) const;

		void Set(NetHandle sock);

		void Clr (NetHandle sock);

		NetHandle MaxSocket()const;
		
		size_t NumSet() const;
		
		//被select完之后，如果完成处理操作之后，那么下一次select之前要调用Sync()
		void Sync();

		//operator fd_set*();

		fd_set *Fdset(void);
public:
		ActiveSockSet GetActive()const;

private:
		fd_set					m_mask;
		std::set<NetHandle>		m_set;
};






}
