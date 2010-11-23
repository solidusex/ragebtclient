
/*
 * The Rage Library
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

#include "Config.h"
#include <sstream>







namespace Rage{


class HttpResponse : NonCopyable
{
private:
		enum Limits
		{
				MAX_VERSION_LENGTH = 8,
				MAX_STATUS_LENGTH  = 3,
				MAX_REASON_LENGTH  = 512,
				MAX_NAME_LENGTH  = 256,
				MAX_VALUE_LENGTH = 4096
		};
private:
		typedef std::map<std::string, std::string>	NameValueMap;
private:
		std::string		m_version;
		std::string		m_status;
		std::string		m_reason;
		NameValueMap	m_nv_map;
private:
		bool parse_header_content(std::istringstream &istr);

public:
		bool Parse(std::istringstream &istr);

public:
		void Clear();

		std::string Version()const;

		std::string Status()const;

		std::string Reason()const;

		std::string Value(const std::string &name)const;
};
























}