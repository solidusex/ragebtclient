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
 */#pragma once


#include "BencodingType.h"


namespace Rage {


class BencodingEncoder
{
private:
		StreamSpace::DataOutputStream	m_content;
private:
		void analyzer(TypeBase *pbase);
public:
		size_t WriteString(StringType &str_bt);//都是返回写了几个字节

		size_t WriteInt(IntegerType &int_bt);

		size_t WriteList(ListType &list_bt);    

		size_t WriteDict(DictType &dict_bt);
public:
		BencodingEncoder();

		~BencodingEncoder();

		bool GenContent(DictType &dict);

		void Clear();

		size_t Size() ;

		const t_byte* Content();
};





}
