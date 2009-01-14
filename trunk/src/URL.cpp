/*
 * The Rage Library
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
 */#include "URL.h"


namespace Rage {

		
std::string URLEncoder::Encoder(const std::string &str)
{
		assert(str.size() != 0);
		
		std::string	 out;
		
		size_t n = str.size();
		for(size_t i = 0; i < n; ++i)
		{
				if(is_digi_alpha(str[i]))
				{
						out += str[i];
				}else 
				{
						out += url_encode_char(str[i]);
				}
		}
		return out;
}

std::string URLEncoder::Encoder(const char *url, size_t len)
{
		assert(url != 0);
		assert(len != 0);
		
		std::string	 out;
		
		for(size_t i = 0; i < len; ++i)
		{
				if(is_digi_alpha(url[i]))
				{
						out += url[i];
				}else 
				{
						out += url_encode_char(url[i]);
				}
		}
		return out;

}



std::string URLEncoder::url_encode_char(char c)
{
		static const char HEX_TABLE[] = "0123456789ABCDEF";
		char b[4];
		b[0] = '%';
		b[1] = HEX_TABLE[(c >> 4) & 0x0F];
		b[2] = HEX_TABLE[c & 0x0F];
		b[3] = '\0';
		return std::string(b);
}

bool URLEncoder::is_digi_alpha(char c)
{
		int t = (unsigned char)c;
				if( ((47 < t) && (t < 58))  || (( t > 64) && (t < 91)) || (( t > 96) && (t < 123)) )
						return true;
				else
						return false;
}











std::string URLDecoder::Decode(const std::string &str)
{
		
		int len = str.length();
		assert(len != 0);
		char* buff = new char[len + 1];
		strcpy(buff,str.c_str());
		std::string ret = "";
		for(int i=0;i<len;i++) {
				if(buff[i] == '+') {
						ret = ret + " ";
				}else if(buff[i] == '%') {
						char tmp[4];
						char hex[4];			
						hex[0] = buff[++i];
						hex[1] = buff[++i];
						hex[2] = '\0';		
						sprintf(tmp,"%c",convert_to_dec(hex));
						ret = ret + tmp;
				}else {
						ret = ret + buff[i];
				}
		}
		delete[] buff;
		return ret;
}

int URLDecoder::convert_to_dec(const char* hex) 
{
		char buff[12];
		sprintf(buff,"%s",hex);
		int ret = 0;
		int len = strlen(buff);
		for(int i=0;i<len;i++) 
		{
				char tmp[4];
				tmp[0] = buff[i];
				tmp[1] = '\0';
				get_asdec(tmp);
				int tmp_i = atoi(tmp);
				int rs = 1;
				for(int j=i;j<(len-1);j++) 
				{
						rs *= 16;
				}
				ret += (rs * tmp_i);
		}
		return ret;
}

void URLDecoder::get_asdec(char* hex)
{
		char tmp = tolower(hex[0]);
		if(tmp == 'a') {
				strcpy(hex,"10");
		}else if(tmp == 'b') {
				strcpy(hex,"11");
		}else if(tmp == 'c') {
				strcpy(hex,"12");
		}else if(tmp == 'd') {
				strcpy(hex,"13");
		}else if(tmp == 'e') {
				strcpy(hex,"14");
		}else if(tmp == 'f') {
				strcpy(hex,"15");
		}else if(tmp == 'g') {
				strcpy(hex,"16");
		}
}





#define TYPE_FMT "\\b[Hh][Tt]{2}[Pp]://|\\b[Uu][Dd][Pp]://"
#define HOST_FMT "[\\w,:,/,\\.]+/"
#define PORT_FMT ":[0-9]{1,5}"
#define INFO_FMT "[^/]/[^/][\\w,\\./]+\\b"

bool UrlParser::Parse(const std::string &url)
{

		using namespace RegexSpace;

		CRegexpT<char> regex(TYPE_FMT);
		
		MatchResult res = regex.Match(url.c_str());

		if(!res.IsMatched()) 
		{
				return false;
		}

		std::string url_type = url.substr(0, res.GetEnd() - 3);
		
		
		std::transform(url_type.begin(), url_type.end(), url_type.begin(), tolower);
		if(url_type == "http")
		{
				type = HTTP;
		}else if(url_type == "udp")
		{
				type = UDP;
		}else
		{
				return false;
		}

		host = url.substr(res.GetEnd());
		
		regex.Compile(HOST_FMT);
		res = regex.Match(host.c_str());
		host = host.substr(0, res.GetEnd() - 1);

		regex.Compile(PORT_FMT);
		res = regex.Match(host.c_str());
		
		if(!res.IsMatched())
		{
				port = 80;
		}else
		{
				std::string port_str = host.substr(res.GetStart() + 1);
				port = ::atoi(port_str.c_str());
				host = host.substr(0, res.GetStart());
		}
		
		regex.Compile(INFO_FMT);
		res = regex.Match(url.c_str());
		if(!res.IsMatched())
		{
				path = "/";
		}
		else
		{
				path = url.substr(res.GetStart() + 1);
		}

		return true;
}		


#undef TYPE_FMT
#undef HOST_FMT
#undef PORT_FMT
#undef INFO_FMT


}