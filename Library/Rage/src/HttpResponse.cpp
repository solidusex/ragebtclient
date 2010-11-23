
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

#include "HttpResponse.h"


namespace Rage{





bool HttpResponse::parse_header_content(std::istringstream &istr)
{
		static const int eof = std::char_traits<char>::eof();
		int ch = istr.get();

		while (ch != eof && ch != '\r' && ch != '\n')
		{
				std::string name;
				std::string value;
				while (ch != eof && ch != ':' && name.length() < MAX_NAME_LENGTH) 
				{ 
						name += ch; 
						ch = istr.get(); 
				}

				if (ch != ':') return false;

				if (ch != eof) ch = istr.get(); // ':'

				while (isspace(ch)) ch = istr.get();

				while (ch != eof && ch != '\r' && ch != '\n' && value.length() < MAX_VALUE_LENGTH)
				{ 
						value += ch; 
						ch = istr.get(); 
				}

				if (ch == '\r') ch = istr.get();

				if (ch == '\n') 
				{
						ch = istr.get();
				}else if(ch != eof)
				{
						return false;
				}

				while (ch == ' ' || ch == '\t') // folding
				{
						while (ch != eof && ch != '\r' && ch != '\n' && value.length() < MAX_VALUE_LENGTH)
						{ 
								value += ch; 
								ch = istr.get(); 
						}

						if (ch == '\r')
						{
								ch = istr.get();
						}

						if (ch == '\n')
						{
								ch = istr.get();

						}else if (ch != eof)
						{
								return false;
						}
				}
				m_nv_map[name] =  value;
		}
		istr.putback(ch);

		return true;
}


bool HttpResponse::Parse(std::istringstream &istr)
{
		static const int eof = std::char_traits<char>::eof();

		std::string version;
		std::string status;
		std::string reason;

		int ch =  istr.get();
		while (isspace(ch))
		{
				ch = istr.get();
		}

		if (ch == eof) return false;

		while (!isspace(ch) && ch != eof && version.length() < MAX_VERSION_LENGTH) 
		{
				version += (char) ch; 
				ch = istr.get(); 
		}

		if (!isspace(ch)) return false;


		while(isspace(ch))
		{
				ch = istr.get();
		}

		while (!isspace(ch) && ch != eof && status.length() < MAX_STATUS_LENGTH) 
		{ 
				status += (char) ch;
				ch = istr.get(); 
		}

		if (!isspace(ch)) return false;

		while (isspace(ch)) ch = istr.get();


		while (ch != '\r' && ch != '\n' && ch != eof && reason.length() < MAX_REASON_LENGTH) 
		{ 
				reason += (char) ch; 
				ch = istr.get(); 
		}

		if (!isspace(ch)) return false;

		if (ch == '\r')	ch = istr.get();


		if(!parse_header_content(istr)) return false;

		ch = istr.get();

		while (ch != '\n' && ch != eof)
		{
				ch = istr.get();
		}
		m_version = version;
		m_status = status;
		m_reason = reason;

		
		return true;

}


void HttpResponse::Clear()
{
		m_nv_map.clear();
		m_status = "";
		m_reason = "";
		m_version = "";

}

std::string HttpResponse::Version()const
{
		return m_version;
}

std::string HttpResponse::Status()const
{
		return m_status;
}

std::string HttpResponse::Reason()const
{
		return m_reason;
}

std::string HttpResponse::Value(const std::string &name)const
{
		NameValueMap::const_iterator cit = m_nv_map.find(name);
		if(cit != m_nv_map.end())
		{
				return cit->second;
		}else
		{
				return "";
		}
}
		




















}