
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
#include "../StrConv.h"



#include <windows.h>


namespace FormatSpace {


#define CHECK_AND_RETURN(cond, res)				\
		do{										\
				if(!(cond)) return res;			\
		}while(0)


std::wstring AnsiConverter::ToUnicode(const std::string &sour)
{
		int len = MultiByteToWideChar(CP_ACP, 0, sour.c_str(), -1, 0, 0);
		
		CHECK_AND_RETURN(len != 0, L"");
		
		std::vector<wchar_t> wstr(len + 1);
		int len2 = MultiByteToWideChar(CP_ACP, 0, sour.c_str(), -1, &wstr[0], len);
		
		CHECK_AND_RETURN(len2 == len, L"");
		
		wstr[len2 - 1] = L'\0';
		std::wstring dest = &wstr[0];
		return dest;

}

std::string AnsiConverter::ToUTF8(const std::string &sour)
{
		std::wstring unicode = ToUnicode(sour);
		return UnicodeConverter::ToUTF8(unicode);
}



std::string	UnicodeConverter::ToAnsi(const std::wstring &sour)
{
		int len = WideCharToMultiByte(CP_ACP, 0, sour.c_str(), sour.size(), 0, 0, 0, 0);
		
		CHECK_AND_RETURN(len != 0, "");
		
		std::vector<char> data(len + 1, '\0');
		
		int len2 = WideCharToMultiByte(CP_ACP, 0, sour.c_str(), sour.size(), &data[0], len, 0, 0);
		
		data[len2] = '\0';

		CHECK_AND_RETURN(len2 == len, "");

		return std::string(&data[0]);
}
		
std::string UnicodeConverter::ToUTF8(const std::wstring &sour)
{
		std::vector<char> dest(0);
		int len = WideCharToMultiByte(CP_UTF8, 0, sour.c_str(), -1, 0, 0, 0, 0);
		CHECK_AND_RETURN(len != 0, "");
		dest.resize(len + 1);
		int len2 = WideCharToMultiByte(CP_UTF8, 0, sour.c_str(), -1, (char*)&dest[0], (int)dest.size(), 0, 0);
		CHECK_AND_RETURN(len == len2, "");
		dest[len2] = '\0';
		return std::string(&dest[0]);
}






std::string UTF8Converter::ToAnsi(const std::string &sour)
{
		std::wstring wsour = ToUnicode(sour);
		return UnicodeConverter::ToAnsi(wsour);


}



std::wstring UTF8Converter::ToUnicode(const std::string &sour)
{
		CHECK_AND_RETURN(sour != "", L"");
		
		int len = MultiByteToWideChar(CP_UTF8, 0, sour.c_str(), -1, 0, 0);

		CHECK_AND_RETURN(len != 0, L"");
		
		std::vector<wchar_t> wstr(len + 1, L'\0');
		
		int len2 = MultiByteToWideChar(CP_UTF8, 0, sour.c_str(), -1, &wstr[0], len);
		
		CHECK_AND_RETURN(len2 == len, L"");
		wstr[len2] = L'\0';
		return std::wstring(&wstr[0]);
}



}