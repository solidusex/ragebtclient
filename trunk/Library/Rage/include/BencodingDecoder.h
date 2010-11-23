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
#pragma once

#include "BencodingType.h"


namespace Rage {
/********************************************************************************************************
Class Name: BExtractor
Functional: ��ȡ�����ļ���Ϣ;
Member Function: 
				size_t analyzer(size_t beg_pos, TypeBase *&pbase);
				size_t read_string(size_t beg_pos, StringType &str_bt);
				size_t read_int(size_t beg_pos, IntegerType &int_bt);
				size_t read_list(size_t beg_pos, ListType &list_bt);
				size_t read_dict(size_t beg_pos, DictType &dict_bt);
				size_t locate_key(size_t beg_pos, const std::string &key);
				������ȡ������bfile����ռ�ݵĳ��ȣ�ʧ�ܷ���0��
				size_t get_content(size_t b, size_t e, std::string &content);
				��������ȡ��content���� ��ʧ�ܷ���0��
********************************************************************************************************/

class BencodingDecoder  : private NonCopyable
{
private:
		std::vector<t_byte>		bfile;
public:
		size_t analyzer(size_t beg_pos, TypeBase *&pbase);//����read_list��read_dict�ݹ���ȡ����
		size_t ReadString(size_t beg_pos, StringType &str_bt);
		//��ָ��λ�ÿ�ʼλ��(����0)
		//�����ݼ��뵽һ��ListType��,�ڲ�����analyzer����, �ᵼ�µݹ����
		//�κδ��󶼽����·��� 0,ֱ�ӽ���ʧ��
		//���޴���,�򷵻ض�ȡ�����ݳ���,
		size_t ReadInt(size_t beg_pos, IntegerType &int_bt);
		
		size_t ReadList(size_t beg_pos, ListType &list_bt);    
		size_t ReadDict(size_t beg_pos, DictType &dict_bt);
		//����ִ����ʼλ��(����ʼλ�ñ���Ϊd��ͷ��dict����);
		size_t LocateKey(size_t beg_pos, const std::string &key);
		
public:
		 //����һ��Ԫ���������� ��Ȼ��bencoding file, ���δ��������һ��d,�򷵻�false;
		bool SetContent(const std::vector<t_byte> &meta); 
		
		bool SetContent(const t_byte* buf, size_t len);

		size_t GetContent(size_t b, size_t e, std::vector<t_byte> &content)const; //����ִ��λ����ȡһ��Ԫ����

		const t_byte* GetContent(size_t pos)const;

		void Clear() { bfile.clear(); }
public:
		BencodingDecoder(){};
		~BencodingDecoder(){   };
};


}

