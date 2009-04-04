#pragma once

#include "Foundation.h"

namespace SeriSpace {





enum SeriType{ BINARY_TYPE = 0x0000, S_INTEGER_TYPE, U_INTEGER_TYPE, FLOAT_TYPE, LIST_TYPE, MAP_TYPE };

class TypeBase : NonCopyable
{
private:
		SeriType		m_type;
public:
		TypeBase(SeriType type) : m_type(type)
		{

		}

		virtual ~TypeBase()
		{

		}

		SeriType GetType()const { return m_type; }
};


class Binary : public TypeBase
{
private:
		DSSpace::SimpleBuffer	m_buf;
public:
		void Set(const t_byte *data, size_t len);
		
		void Clear();
		
		const t_byte* Get()const;
		
		size_t Size()const;
public:
		Binary();
		Binary(const t_byte *data, t_uint32 len);
		virtual ~Binary();
};


class SignedInteger : public TypeBase
{
private:
		t_int64		m_num;
public:
		void Set(t_int64 num);
		t_int64 Get()const;
		
public:
		SignedInteger();
		
		explicit SignedInteger(t_int64 num);
		
		virtual ~SignedInteger();
};

class UnSignedInteger : public TypeBase
{
private:
		t_uint64	m_num;
public:
		void Set(t_uint64 num);
		
		t_uint64 Get()const;
public:
		UnSignedInteger();
		
		explicit UnSignedInteger(t_uint64 num);
		
		virtual ~UnSignedInteger();
};


class Float : public TypeBase
{
private:
		double m_num;
public:
		void Set(double num);
		
		double Get()const;
public:
		Float();
		Float(double num);
		virtual ~Float();
};



class List : public TypeBase
{
public:
		typedef std::vector<TypeBase*>	ListType;
		typedef ListType::const_iterator	CIterator;
		typedef ListType::iterator			Iterator;
private:
		ListType				m_list;
public:
		bool Insert(TypeBase *ptype);
		bool Remove(TypeBase *ptype);
		void Clear();
public:
		CIterator Begin()const;
		CIterator End()const;

		Iterator Begin();
		Iterator End();
public:
		List();
		virtual ~List();
};

class Map : public TypeBase
{
public:
		typedef std::map<std::string, TypeBase*> MapType;
		typedef MapType::const_iterator			CIterator;
		typedef MapType::iterator				Iterator;
private:
		MapType			m_map;
public:
		bool Insert(const std::string &key, TypeBase *ptype);
		
		TypeBase* Remove(const std::string &key);

		void Clear();
public:
		CIterator Begin()const;
		CIterator End()const;

		Iterator Begin();
		Iterator End();
public:
		Map();
		virtual ~Map();
};








}