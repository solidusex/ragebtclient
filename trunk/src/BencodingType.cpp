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
 */#include "BencodingType.h"



namespace Rage {

/***************************************************************************************************************/
#if(0)

StringType::StringType(const std::wstring &wstr) : TypeBase(STRING_TYPE)
{
		assign(wstr);
}

void StringType::assign(const std::wstring &wstr)
{
		m_str.clear();
		if(wstr.empty())
		{
				return;
		}

		m_str.resize(wstr.size() * 2);
		memcpy(&m_str[0], wstr.c_str(), m_str.size());
}

void StringType::get_str(std::wstring &wstr)const
{
		wstr = L"";
		if(m_str.size() < 2) return;
		assert((m_str.size() % 2) == 0);
		wchar_t *pwstr = (wchar_t*)::malloc(m_str.size() + 2);
		assert(pwstr != 0);
		memcpy(pwstr, &m_str[0], m_str.size());
		pwstr[m_str.size() / 2] = L'\0';
		wstr = pwstr;
		::free(pwstr);
}

#endif

std::string StringType::get_str() const
{
		std::string str;
		for(size_t i = 0; i < m_str.size(); ++i)
		{
				str += m_str[i];
		}
		return str;
}

void StringType::get_str(std::vector<t_byte> &str) const
{
		str.clear();
		str = m_str; 
}

void StringType::get_str(char *buf, t_ulong len)const
{
		assert(len <= m_str.size());
		memcpy(buf, &m_str[0], len);
}


void StringType::assign(const std::vector<t_byte> &str)
{
		m_str = str;
}
		
//void StringType::assign(const char* str, t_ulong len);
void StringType::assign(const std::string &str)
{
		std::vector<t_byte> vstr(str.size());

		for(size_t i = 0; i < str.size(); ++i)
		{
				vstr[i] = str[i];
		}
		m_str.clear();
		assign(vstr);
}

void StringType::assign(const t_byte *pbuf, t_ulong len)
{
		assert(pbuf != 0);
		assert(len != 0);
		clear();
		m_str.resize(len);
		memcpy(&m_str[0], pbuf, len);

}



StringType::StringType(const std::vector<t_byte> &str) : TypeBase(STRING_TYPE)
{
		assign(str);
}

StringType::StringType(const std::string &str) : TypeBase(STRING_TYPE)
{
		assign(str);
}


StringType::StringType(const t_byte *pdata, size_t len) : TypeBase(STRING_TYPE)
{
		assign(pdata, len);

}

StringType& StringType::operator= (const std::vector<t_byte> &str)
{
		assign(str);
		return *this;
}



StringType::StringType() : TypeBase(STRING_TYPE), m_str(0) 
{

}

StringType::StringType(const StringType &other) : TypeBase(STRING_TYPE)
{
		assign(other.m_str); 
}

StringType& StringType::operator= (const StringType &other) 
{
		if(this != &other)
				assign(other.m_str);
		return *this;
}


/*****************************************************************************************************/

IntegerType::IntegerType() : TypeBase(INTEGER_TYPE), m_int(0) {}

IntegerType& IntegerType::operator=(const t_uint64 n) { m_int = n; return *this; }

IntegerType::IntegerType(const IntegerType& other) : TypeBase(INTEGER_TYPE)
{
		m_int = other.m_int;
}

IntegerType::IntegerType(t_uint64 n) : TypeBase(INTEGER_TYPE), m_int(n) {}

IntegerType& IntegerType::operator=(const IntegerType &other)
{
		if(this != &other)
				m_int = other.m_int;
		return *this;
}




/*****************************************************************************************************/



ListType::ListType() : TypeBase(LIST_TYPE) {}

ListType::~ListType() { clear(); }
void ListType::clear()
{
		ThisIterator beg = m_list_data.begin();
		ThisIterator end = m_list_data.end();

		for(ThisIterator it = beg; it != end; ++it)
				delete (*it);
		
		m_list_data.clear();
}

void ListType::insert(TypeBase *ptype)
{
		m_list_data.push_back(ptype);
}



/*****************************************************************************************************/



TypeBase* DictType::get_value(const std::string &key)const
{
		C_Iterator beg = m_map_data.begin();
		C_Iterator end = m_map_data.end();
		
		for(C_Iterator it = beg; it != end; ++it)
		{
				if((it->first->get_str()) == key)
				{
						return (it->second);
				}
		}
		return NULL;
}

bool DictType::remove_value(const std::string &key)
{
		for(ThisIterator it = m_map_data.begin(); it != m_map_data.end(); ++it)
		{
				if(it->first->get_str()  == key)
				{
						delete it->first;
						delete it->second;
						m_map_data.erase(it);
						return true;
				}
		}

		return false;
}

bool DictType::set_value(const std::string &key, TypeBase* ptype_bt)
{
		assert(ptype_bt != NULL);
		for(ThisIterator it = m_map_data.begin(); it != m_map_data.end(); ++it)
		{
				if(it->first->get_str()  == key)
				{
						return false;
				}
		}
		StringType *pstr = new StringType(key);
		assert(pstr);
		m_map_data.push_back(std::make_pair(pstr, ptype_bt));
		return true;		

}

#if(0)
bool DictType::set_value(StringType* pstr_bt, TypeBase* ptype_bt)
{
		assert(pstr_bt != NULL);
		assert(ptype_bt != NULL);
		for(ThisIterator it = m_map_data.begin(); it != m_map_data.end(); ++it)
		{
				if(it->first->get_str()  == pstr_bt->get_str())
				{
						return false;
				}
		}
		m_map_data.push_back(std::make_pair(pstr_bt, ptype_bt));
		return true;
}
#endif

void DictType::clear()
{
		ThisIterator	beg = m_map_data.begin();
		ThisIterator	end = m_map_data.end();
		for(ThisIterator it = beg; it != end; ++it)
		{
				delete (it->first);
				delete (it->second);
		}
		m_map_data.clear();
}
				
/****************************************************************************************************/









}