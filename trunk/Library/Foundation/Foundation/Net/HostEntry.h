
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



#include "InetAddress.h"

namespace NetSpace {



class FoundationAPI HostEntry : NonCopyable
{
public:
		typedef std::vector<InetAddress>		AddrList;

		typedef std::vector<std::string>		AliasList;
		
		typedef ThreadSpace::FastMutex::ScopeLock  MutexLock;
public:
		HostEntry();

		HostEntry(const std::string &host);

		bool Reset(const std::string &host);
		
		~HostEntry()  {  }
		
		std::string GetCanonicalName()const
		{
				return m_canonical_name; 
		}

		AddrList	GetAddrList()const 
		{
				return m_addr_list; 
		}

		std::string GetHostName()const 
		{
				return m_host; 
		}

		AliasList	GetAliasList()const 
		{
				return m_alias_list; 
		}

private:
		AddrList				m_addr_list;

		AliasList				m_alias_list;

		std::string				m_canonical_name;

		std::string				m_host;

private:
		static ThreadSpace::FastMutex			GlobalMutex;
};



}