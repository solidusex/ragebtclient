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
 */#include "BencodingEncoder.h"


namespace Rage {

BencodingEncoder::BencodingEncoder() : m_content(new StreamSpace::MemStreamBuffer<>())
{
		assert(m_content.Buffer() != 0);
}

BencodingEncoder::~BencodingEncoder() 
{   
		assert(m_content.Buffer() != 0);
		delete m_content.Buffer();
}


bool BencodingEncoder::GenContent(DictType &dict)
{
		Clear(); 
		WriteDict(dict);
		return true;
}

void BencodingEncoder::Clear()
{
		m_content.Buffer()->Clear();
}

size_t BencodingEncoder::Size()
{
		return (size_t)m_content.Buffer()->Available(); 
}

const t_byte* BencodingEncoder::Content() 
{ 
		return m_content.Buffer()->GetContent();
}



	
void BencodingEncoder::analyzer(TypeBase *pbase)
{
		//printf("BencodingEncoder::analyzer\n");
		switch(pbase->type())
		{
		case INTEGER_TYPE:
				WriteInt(*((IntegerType*)pbase));
				return;
		case LIST_TYPE:
				WriteList(*((ListType*)pbase));
				return;
		case DICT_TYPE:
				WriteDict(*((DictType*)pbase));
				return;
		default:
				WriteString(*((StringType*)pbase));
				return;
		}
}


size_t BencodingEncoder::WriteDict(DictType &dict_bt)
{
		//printf("BencodingEncoder::WriteDict\n");
		m_content.WriteByte((t_byte)'d');
		DictType::ThisIterator it = dict_bt.begin();
		for(it; it != dict_bt.end(); ++it)
		{
				WriteString(*it->first);
				analyzer(it->second);
		}
		m_content.WriteByte((t_byte)'e');
		return 0;

}


size_t BencodingEncoder::WriteList(ListType &list_bt)
{
		//printf("BencodingEncoder::WriteList\n");
		m_content.WriteByte((t_byte)'l');
		
		for(ListType::C_Iterator it = list_bt.begin(); it != list_bt.end(); ++it)
		{
				analyzer((*it));
		}
		
		m_content.WriteByte((t_byte)'e');

		return 0;
}



size_t BencodingEncoder::WriteInt(IntegerType &int_bt)
{		
		//printf("BencodingEncoder::WriteInt\n");
		m_content.WriteByte((t_byte)'i');
		std::string num = FormatSpace::NumberFormatter::Format(int_bt.get_int());
		
		m_content.Write((const t_byte*)num.c_str(), (t_ulong)num.size());
		m_content.WriteByte((t_byte)'e');
		
		return 0;
}


size_t BencodingEncoder::WriteString(StringType &str_bt)
{
		//printf("BencodingEncoder::WriteString\n");

		std::vector<t_byte> buf;
		str_bt.get_str(buf);
		t_ulong len = (t_ulong)buf.size();
		std::string hdr = FormatSpace::NumberFormatter::Format(len);
		hdr += ":";
		m_content.Write((const t_byte*)hdr.c_str(), (t_ulong)hdr.size());
		if(!buf.empty())
		{
				m_content.Write((const t_byte*)&buf[0], (t_ulong)buf.size());

		}

		return 0;
}










}