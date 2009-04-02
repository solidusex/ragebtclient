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

#include "BencodingDecoder.h"
#include <algorithm>
#include <iterator>
#include <memory>

namespace Rage {

#define ASSERT_POS(pos) do{ /*assert(pos < bfile.size())*/ if(pos >= bfile.size()) return 0;}while(0);
#define ERR_CHECK(num) do{ /*assert(num != 0);*/if(num == 0) return 0; }while(0);






bool BencodingDecoder::SetContent(const t_byte* buf, size_t len)
{
		assert(buf);
		assert(len > 0);
		size_t len_left = len;
		
		size_t i = 0;

		for(i = 0; i < len; ++i)
		{
				if(buf[i] == 'd')
				{
						bfile.resize(len_left);
						memcpy(&bfile[0], &buf[i], len_left);
						return true;
				}
		}

		return false;
}

bool BencodingDecoder::SetContent(const std::vector<t_byte> &meta) 
{
		std::vector<t_byte>::const_iterator it;
		it = std::find(meta.begin(), meta.end(), (t_byte)'d');
		if(it == meta.end()) return false;

		bfile.clear();
		bfile.resize(meta.size());
		std::copy(it, meta.end(), bfile.begin());
		return true;
}

const t_byte* BencodingDecoder::GetContent(size_t pos)const
{
		ASSERT_POS(pos);

		return &bfile[pos];
}

size_t BencodingDecoder::GetContent(size_t b, size_t len, std::vector<t_byte> &content)const
{
		ASSERT_POS(b);
		ASSERT_POS(b + len);
		content.resize(len);
		std::vector<t_byte>::const_iterator beg = bfile.begin() + b;
		std::vector<t_byte>::const_iterator end = beg + len;
		std::copy(beg, end, content.begin());
		return len;
}


size_t BencodingDecoder::analyzer(size_t beg_pos, TypeBase *&pbase)
{
		std::auto_ptr<IntegerType>		pint_auto;
		std::auto_ptr<StringType>		pstr_auto;
		std::auto_ptr<ListType>			plist_auto;
		std::auto_ptr<DictType>			pdict_auto;
		
		ASSERT_POS(beg_pos);
		size_t idx = beg_pos;
		size_t res = 0;
		char ch = bfile[idx];
		
		switch(ch){
		case 'i':
				pint_auto.reset(new IntegerType());
				res = ReadInt(idx, *pint_auto.get());
				if(res != 0) pbase = pint_auto.release();
				return res;
		case 'l':
				plist_auto.reset(new ListType());
				res = ReadList(idx, *plist_auto.get());
				if(res != 0) pbase = plist_auto.release();
				return res;
		case 'd':
				pdict_auto.reset(new DictType());
				res = ReadDict(idx, *pdict_auto.get());
				if(res != 0) pbase = pdict_auto.release();
				return res;
		default:
				pstr_auto.reset(new StringType());
				res = ReadString(idx, *pstr_auto.get());
				if(res != 0) pbase = pstr_auto.release();
				return res;
		}
}


size_t BencodingDecoder::ReadList(size_t beg_pos, ListType &list_bt)
{
		TypeBase *pbase = NULL;
		
		size_t idx = beg_pos + 1;  //跳过'l';
		ASSERT_POS(idx);
		
		while(bfile[idx] != 'e'){
				size_t i = 0;
				i = analyzer(idx, pbase);
				ERR_CHECK(i);
				idx += i;
				ASSERT_POS(idx);
				list_bt.insert(pbase);
		}
		return (idx + 1 - beg_pos);
}


				//pstr = new StringType();
				//i = read_string(idx, *pstr);
				//dict_bt.set_value(pstr, pbase);
//StringType		*pstr = NULL;

size_t BencodingDecoder::ReadDict(size_t beg_pos, DictType &dict_bt)
{

		size_t idx = beg_pos + 1;		//跳过'd';
		ASSERT_POS(idx);

		TypeBase		*pbase = NULL;
		
		while(bfile[idx] != 'e'){
				StringType str_type;
				size_t i = 0;
				i = ReadString(idx, str_type);
				ERR_CHECK(i);
				idx += i;
				ASSERT_POS(idx);
				i = analyzer(idx, pbase);
				ERR_CHECK(i);
				idx += i;
				ASSERT_POS(idx);
				dict_bt.set_value(str_type.get_str(), pbase);
		}

		return (idx + 1 - beg_pos);
}


size_t BencodingDecoder::ReadString(size_t beg_pos, StringType &str_bt)
{
		str_bt.clear();

		std::string				num;			//得到前面的数值 4:abcd的4
		size_t idx			=	beg_pos;
		
		ASSERT_POS(idx);

		while(bfile[idx] != ':'){
				num += bfile[idx];
				++idx;
				ASSERT_POS(idx);
		}
		
		++idx;									//跳过':';

		size_t len = atoi(num.c_str());
		
		ASSERT_POS((idx + len));
		
		std::vector<t_byte>			str(len);

		for(size_t i = 0; i < len; ++i)
				str[i] = bfile[idx + i];
		
		if(str.size() != len) return 0;
		str_bt = str;
		return (idx + len - beg_pos);
}


size_t BencodingDecoder::ReadInt(size_t beg_pos, IntegerType &int_bt)
{
		size_t idx		=		beg_pos + 1;	//跳过'i'
		
		ASSERT_POS(idx);

		if(bfile[idx] == 'e') return 0;		//如果中间无数值数据
		
		std::string				num;
		while(bfile[idx] != 'e')
		{
				char ch = bfile[idx];
				if(!std::isdigit(ch) && ch != '-') return 0;
				num += ch;
				++idx;
				ASSERT_POS(idx);
		}

		t_uint64 val = 0;
		bool res = FormatSpace::NumberParser::TryParseUInt64(num, val);
		if(!res)
		{
				return 0;
		}else
		{
				int_bt = val;
				return (idx - beg_pos + 1);
		}
}

		


size_t BencodingDecoder::LocateKey(size_t beg_pos, const std::string &key)
{
		assert(bfile[beg_pos] == 'd');

		size_t idx = beg_pos + 1;		//跳过'd';
		ASSERT_POS(idx);
		
		TypeBase		*pbase = NULL;
		StringType		key_bt;

		while(bfile[idx] != 'e'){
				size_t i = 0;
				i = ReadString(idx, key_bt);
				ERR_CHECK(i);
				idx += i;
				ASSERT_POS(idx);
				if(key_bt.get_str() == key)
						return idx;

				i = analyzer(idx, pbase);

				if(i == 0 || ((idx + i) >= bfile.size())){
						break;
				}
				
				idx += i;
		}
		return 0;
}


#undef	ASSERT_POS
#undef	ERR_CHECK


}