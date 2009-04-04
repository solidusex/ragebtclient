#pragma once
#include "SeriType.h"

namespace SeriSpace {



/*
		Binary					: len : data
		Signed Integer			: s 12345 e
		UnSigned Integer		: u 12345 e
		Float					: f 12345 e
		List					: l item e
		Map						: m Binary(key) item e
*/



class SeriWriter : NonCopyable
{
private:
		DSSpace::SimpleBuffer			&m_buf;
private:
		bool write_type(const TypeBase *ptype);
		bool write_binary(const t_byte *pdata, size_t len);
		bool write_int(t_int64 num);
		bool write_uint(t_uint64 num);
		bool write_float(double num);
		
		bool write_list(const List *plist);
		bool write_map(const Map *pmap);

public:
		bool Generate(const TypeBase *ptype);
		
		const t_byte* Get()const;
		
		size_t Size()const;

		void Clear();
public:
		SeriWriter(DSSpace::SimpleBuffer &buf);
		
		~SeriWriter();

};



}