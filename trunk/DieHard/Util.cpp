#include "stdafx.h"
#include "Util.h"



CString StrConv(const std::wstring &wstr)
{

		CString cstring;

#if defined(_UNICODE)
				cstring = wstr.c_str();
#else																	
				cstring = FormatSpace::UnicodeConverter::ToAnsi((wstr)).c_str();
#endif
				return cstring;
}


std::wstring StrConv(const CString &cstring)
{
		std::wstring res;

#if defined(_UNICODE)
		res = cstring;
#else
		res = FormatSpace::AnsiConverter::ToUnicode(std::string((const char*)cstring));
#endif


		return res;

}