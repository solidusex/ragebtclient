
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
#include "InetAddress.h"




namespace NetSpace {



InetAddress::InetAddress(const std::string &ip, t_uint16 port)
{		
		Clear();
		
		SetPort(port);

		if(ip == "")return;
		
		SetAddr(ip);
}


InetAddress::InetAddress(const sockaddr_in &sa)
{
		memcpy((void*)&m_sa, (void*)&sa, sizeof(m_sa));
}

InetAddress::InetAddress(const InetAddress &other)
{
		*this = other;
}

InetAddress& InetAddress::operator=(const InetAddress &other)
{
		memcpy((void*)&m_sa, (void*)&other.m_sa, sizeof(m_sa));
		
		return *this;
}

const SA* InetAddress::Saddr()const { return (SA*)&m_sa; }

InetAddress::InetAddress()
{
		Clear();
}

SA* InetAddress::Saddr()
{
		return (SA*)&m_sa;
}

t_uint32 InetAddress::IP()const 
{ 
		return m_sa.sin_addr.s_addr;
}


std::string InetAddress::Str() const 	//返回 IP 地址字符串（以文本表现形式);
{
		const char *pstr = ::inet_ntoa(m_sa.sin_addr);
		if(pstr != 0)
		{
				return std::string(pstr);
		}else
		{
				return "";
		}

}


t_uint16 InetAddress::Port()const
{
		return ntohs(m_sa.sin_port);
		//return m_sa.sin_port;
}

SockLen InetAddress::Length() const
{
		return sizeof(m_sa);
}


void InetAddress::SetAddr(const std::string &ip)
{				
		m_sa.sin_addr.s_addr = inet_addr(ip.c_str());
}

void InetAddress::SetPort(t_uint16 port)
{
		m_sa.sin_port = htons(port);
	
}

void InetAddress::SetAddr(t_uint32 addr)
{
		m_sa.sin_addr.s_addr = addr;
}


void InetAddress::Clear()
{
		memset(&m_sa, 0, sizeof(m_sa));
		m_sa.sin_family = AF_INET;
		m_sa.sin_port = 0;
		m_sa.sin_addr.s_addr = htonl(INADDR_ANY);
}

bool InetAddress::operator== (const InetAddress &other)const
{
		return (memcmp((void*)&m_sa, (const void*)&other.m_sa, sizeof(other.m_sa)) == 0);

}

bool InetAddress::operator!= (const InetAddress &other)const
{
		return (!(*this == other));

}

}

