
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

#include "Config.h"

#include <string>
#include <list>
#include <map>
#include <cassert>
#include <cctype>
#include <vector>


namespace Rage {


enum DataType { STRING_TYPE, INTEGER_TYPE, LIST_TYPE, DICT_TYPE };

/************************************************************************************************************
Class Name: TypeBase
Functional: bencoding���������ֻ������͵Ļ���

Member Function			virtual bool clear(void);				//����



************************************************************************************************************/

class TypeBase
{
private:
		DataType		obj_type;
public:
		TypeBase(const DataType &data_type) : obj_type(data_type) {}
		virtual ~TypeBase() {}
public:
		DataType type() const { return obj_type; }
		virtual void clear() = 0;
		
};





/*************************************************************************************************************c

Class name: StringType
Functional: ����һ������4:abcd�������ַ���,Ȼ���4:ȥ��,�õ��ַ���abcd




**************************************************************************************************************/




class StringType : public TypeBase
{
private:
		std::vector<t_byte>	m_str;
public:
		StringType();

		StringType(const StringType& other);

		StringType(const std::vector<t_byte>& str);

		StringType(const t_byte *pdata, size_t len);

		StringType(const std::string &str);

		//StringType(const std::wstring &wstr);

		~StringType(){}

		StringType& operator= (const std::vector<t_byte> &str);

		StringType& operator= (const StringType &other);
public:
		
		
		void assign(const std::vector<t_byte> &str);
		
		void assign(const std::string &str);
		
		void assign(const t_byte *pbuf, t_ulong len);

		size_t size() const
		{
				return m_str.size(); 
		}
		
		//void assign(const std::wstring &wstr);

		//void get_str(std::wstring &wstr)const;

		std::string get_str() const;

		void get_str(std::vector<t_byte> &str) const;
		
		void get_str(char *buf, t_ulong len)const;
public:
		virtual void clear() { m_str.clear(); };
		
};



class IntegerType : public TypeBase
{
private:
		t_uint64			m_int;
public:
		IntegerType();
		IntegerType& operator=(const t_uint64 n);
		IntegerType(const IntegerType& other);
		IntegerType(const t_uint64 n);
		IntegerType& operator=(const IntegerType &other);
		~IntegerType(){}
public:
		t_uint64 get_int()const { return m_int; }
		virtual void clear() {m_int = 0; }
};


/*************************************************************************************************************
Class Name: ListType
Functional: ���Դ洢StringType, IntegerType, ListType, DictType��������


**************************************************************************************************************/


class ListType : public TypeBase
{
public:
		typedef std::list<TypeBase*>::iterator			ThisIterator;
		typedef std::list<TypeBase*>::const_iterator	C_Iterator;
private:
		std::list<TypeBase*>	m_list_data;
private:
		ListType(const ListType& other);
		ListType& operator= (const ListType & other);
public:
		ListType();
		~ListType();
public:
		void insert(TypeBase *ptype); 
		size_t size()const { return m_list_data.size(); }
		ThisIterator begin() { return m_list_data.begin(); }
		ThisIterator end()   { return m_list_data.end(); }
		
		C_Iterator begin() const { return m_list_data.begin(); }
		C_Iterator end() const { return m_list_data.end(); }
		virtual void clear();
};



class DictType : public TypeBase
{
public:
		typedef std::list<std::pair<StringType*, TypeBase*> >::iterator ThisIterator;
		typedef std::list<std::pair<StringType*, TypeBase*> >::const_iterator C_Iterator;
private:
		std::list<std::pair<StringType*, TypeBase*> > m_map_data;//��list����map��Ϊ�˱�֤һ׼˳�򣬶���mapҲ���ʺ����ִ��ģ�Ĳ���ɾ��������
private:
		DictType(const DictType&);
		DictType& operator=(const DictType&);
public:
		DictType() : TypeBase(DICT_TYPE) {}
		~DictType() { clear(); }
public:
		TypeBase* get_value( const std::string& key)const;
		
		bool set_value(const std::string &key, TypeBase* ptype_bt);
		
		bool remove_value(const std::string &key);

		size_t size()const { return m_map_data.size(); }

		C_Iterator begin() const { return m_map_data.begin(); }

		C_Iterator end() const { return m_map_data.end(); }

		ThisIterator begin() { return m_map_data.begin(); }

		ThisIterator end() { return m_map_data.end(); }
public:
		virtual void clear();
		
};










}