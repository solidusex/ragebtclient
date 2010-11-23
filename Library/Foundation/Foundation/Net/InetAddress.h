
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



#include <vector>
#include <string>

namespace NetSpace {


class  FoundationAPI InetAddress
{
public:
		void Clear();

		std::string Str() const; 	//���� IP ��ַ�ַ��������ı�������ʽ);

		SockLen Length() const;

		void SetAddr(t_uint32 addr);//��Ϊ�����ֽ���,����Ϊ�����ֽ���
		
		void SetAddr(const std::string &ip);
		
		void SetPort(t_uint16 port);//portΪ�����ֽ���
		
		t_uint32 IP()const;

		t_uint16 Port()const;//���ر����ֽ���

		SA* Saddr();

		const SA* Saddr()const;
		
public:
		InetAddress(const sockaddr_in &psa);

		InetAddress();

		InetAddress(const InetAddress &other);

		InetAddress& operator=(const InetAddress &other);

		InetAddress(const std::string &ip, t_uint16 port);//portΪ�����ֽ���
		
		~InetAddress() {   }
public:
		bool operator== (const InetAddress &other)const;
		bool operator!= (const InetAddress &other)const;
private:
		sockaddr_in		m_sa;
};








}