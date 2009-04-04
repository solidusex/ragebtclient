#pragma once


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
