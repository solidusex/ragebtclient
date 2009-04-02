/*
* The Rage Library
* Copyright (c) 2007 by Solidus
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* provided that the above copyright notice appear in all copies and
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*
*/
#pragma once

#include "Config.h"

#include <string>

namespace Rage {


class URLEncoder 
{

private:
		static std::string url_encode_char(char c);
		static bool is_digi_alpha(char c);
public:
		static std::string Encoder(const std::string &url);
		static std::string Encoder(const char *url, size_t len);
};



class URLDecoder 
{
public:
		static std::string Decode(const std::string &str);
private:
		static int convert_to_dec(const char* hex);
		static void get_asdec(char* hex);
};


class UrlParser
{
public:
		enum URLType 
		{
				HTTP	=		0x01,
				UDP		=		0x02
		};
public:
		URLType			type;
		std::string		host;
		std::string		path;
		t_uint16		port;
public:
		bool Parse(const std::string &url);

};


}