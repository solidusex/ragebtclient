#pragma once
#include "SeriType.h"
#include <stack>
namespace SeriSpace{




/*
		Binary					: len : data
		Signed Integer			: s 12345 e
		UnSigned Integer		: u 12345 e
		Float					: f 12345 e
		List					: l item e
		Map						: m Binary(key) item e
*/

class SeriReader : NonCopyable
{
private:
		DSSpace::SimpleBuffer			&m_buf;
public:
		SeriReader(DSSpace::SimpleBuffer &buf);
		
		~SeriReader();
public:
		TypeBase* Parse(const t_byte *pdata, size_t len);

private:
		TypeBase* parse_type();

		Binary* parse_binary();

		SignedInteger* parse_int();

		UnSignedInteger* parse_uint();

		Float*	parse_float();

		List*	parse_list();

		Map*	parse_map();

};



}