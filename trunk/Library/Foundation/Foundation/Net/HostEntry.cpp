
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
#include "HostEntry.h"



namespace NetSpace {


ThreadSpace::FastMutex HostEntry::GlobalMutex;

HostEntry::HostEntry()
{

}

HostEntry::HostEntry(const std::string &host) : m_canonical_name(""), m_host("")
{
		Reset(host);
}

bool HostEntry::Reset(const std::string &host)
{
		m_alias_list.clear();

		m_addr_list.clear();

		MutexLock lock(GlobalMutex);

		

		::hostent *phs =::gethostbyname(host.c_str());

		


		if(phs == 0)
		{
				DEBUG_PRINT1("last error %ld\n", ::WSAGetLastError());
				return false;
		}
		
		char **alias = phs->h_aliases;

		while(*alias)
		{
				m_alias_list.push_back(*alias);
				++alias;
		}


		char **addr_list = phs->h_addr_list;

		while(*addr_list)
		{
				sockaddr_in sa;
				memcpy((void*)&sa.sin_addr, (void*)(*addr_list), sizeof(in_addr));
				sa.sin_family = AF_INET;
				sa.sin_port = 0;
				InetAddress inet(sa);
				m_addr_list.push_back(inet);
				++addr_list;
		}

		m_canonical_name = phs->h_name;

		m_host = host;

		return true;


}



}












#if(0)
bool HostEntry::Set(const InetAddress &inet, int socktype)
{
		return Set(inet.Str(), inet.Port(), socktype);
}

bool HostEntry::Set(const std::string &host, t_ulong port, int socktype)
{
		
		addrinfo		hint,	*result = 0;
		
		memset((void*)&hint, 0, sizeof(addrinfo));
		hint.ai_flags = AI_CANONNAME;
		hint.ai_family = AF_INET;
		hint.ai_socktype = socktype;
		
		std::string serv = FormatSpace::IntConverter((t_int64)port).ToAnsi(10, true);
		
		int r = ::getaddrinfo(host.c_str(), serv.c_str(), &hint, &result);

		if(r != 0)
		{
				::freeaddrinfo(result);
				return false;
		}
		
		m_canonical_name = result->ai_canonname;
		
		for(addrinfo *p = result; p != 0; p = p->ai_next)
		{
				try{
						sockaddr_in *psa = (sockaddr_in*)(p->ai_addr);
						InetAddress inet(*psa);
						m_addr_list.push_back(inet);
				}
				catch(...)
				{
						continue;
				}
		}
		
		::freeaddrinfo(result);

		m_host = host;
		return true;
}





#endif