#include "SeriReader.h"

namespace SeriSpace{





SeriReader::SeriReader(DSSpace::SimpleBuffer &buf) : m_buf(buf)
{

}
		
SeriReader::~SeriReader()
{


}


using namespace FormatSpace;

TypeBase* SeriReader::Parse(const t_byte *pdata, size_t len)
{
		m_buf.Clear();

		if(pdata == NULL || len == 0)
		{
				return NULL;
		}else
		{
				m_buf.Insert(pdata, len);
		}

		return parse_type();
}

TypeBase* SeriReader::parse_type()
{
		if(m_buf.Size() == 0) return NULL;

		t_byte c = *m_buf.Data();
		m_buf.Erase(1);

		switch(c)
		{
		case 's':
				return parse_int();
		case 'u':
				return parse_uint();
		case 'f':
				return parse_float();
		case 'l':
				return parse_list();
		case 'm':
				return parse_map();
		default:
				return parse_binary();

		}
}

Binary* SeriReader::parse_binary()
{
		if(m_buf.Size() == 0) return NULL;

		std::string num = "";
		size_t i = 0;
		for(; i < m_buf.Size(); ++i)
		{
				char c = (char)m_buf.Data()[i];

				if(isdigit(c) != 0)
				{
						num += c;
				}else if(c == ':')
				{
						break;
				}else
				{
						return NULL;
				}
		}

		m_buf.Erase(i);
		i = 0;

		t_uint32 rn;
		if(!NumberParser::TryParseUInt32(num, rn)) return NULL;

		if(m_buf.Size() < rn) return NULL;

		Binary *pb = new Binary(m_buf.Data(), rn);
		m_buf.Erase(rn);
		return pb;
}

SignedInteger* SeriReader::parse_int()
{
		if(m_buf.Size() == 0) return NULL;

		size_t i = 0;
		std::string num;

		for(; i < m_buf.Size(); ++i)
		{
				char c = (char)m_buf.Data()[i];
				
				if(c == 'e')
				{
						break;
				}

				switch(c)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
						num += c;
						break;
				case '-':
						if(num.size() == 0)
						{
								num += c;
						}else
						{
								return NULL;
						}
						break;
				default:
						return NULL;
				}
		}

		m_buf.Erase(i);

		t_int64 res;

		if(!NumberParser::TryParseInt64(num,res)) return NULL;

		SignedInteger *psi = new SignedInteger(res);
		return psi;
}

UnSignedInteger* SeriReader::parse_uint()
{
		if(m_buf.Size() == 0) return NULL;
		std::string num;
		size_t i = 0;

		for(; i < m_buf.Size(); ++i)
		{
				char c = (char)m_buf.Data()[i];
				
				if(c == 'e')
				{
						break;
				}

				switch(c)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
						num += c;
						break;
				default:
						return NULL;
				}
		}

		m_buf.Erase(i);

		t_uint64 res;

		if(!NumberParser::TryParseUInt64(num,res)) return NULL;

		UnSignedInteger *pui = new UnSignedInteger(res);
		return pui;
}

Float*	SeriReader::parse_float()
{
		if(m_buf.Size() == 0) return NULL;
		std::string num;
		size_t i = 0;
		bool has_dot = false;

		for(; i < m_buf.Size(); ++i)
		{
				char c = (char)m_buf.Data()[i];
				
				if(c == 'e')
				{
						break;
				}else
				{
						num += c;
				}
		}

		m_buf.Erase(i);

		double res;

		if(!NumberParser::TryParseFloat(num,res)) return NULL;

		Float *pf = new Float(res);
		return pf;
}

List* SeriReader::parse_list()
{
		if(m_buf.Size() == 0) return NULL;

		
		std::auto_ptr<List> plist(new List());
		
		while(m_buf.Size() > 0 && ((char)m_buf.Data()[0] != 'e'))
		{
				TypeBase *ptype = parse_type();
				
				if(ptype == NULL)
				{
						return NULL;
				}else
				{
						plist->Insert(ptype);
				}
		}
		
		if(m_buf.Size() == 0) return NULL;
		assert(m_buf.Data()[0] == 'e');
		m_buf.Erase(1);
		return plist.release();
}

Map* SeriReader::parse_map()
{
		if(m_buf.Size() == 0) return NULL;
		
		std::auto_ptr<Map> pmap(new Map);

		while(m_buf.Size() > 0 && (char)m_buf.Data()[0] != 'e')
		{
				Binary *pb = parse_binary();
				if(pb == NULL) return NULL;
				
				if(pb->Size() == 0) return NULL;

				std::string key(pb->Get(), pb->Get() + pb->Size());
				delete pb;

				TypeBase *ptype = parse_type();
				
				if(ptype == NULL)
				{
						return NULL;
				}else
				{
						pmap->Insert(key, ptype);
				}
		}
		
		if(m_buf.Size() == 0) return NULL;
		assert(m_buf.Data()[0] == 'e');
		m_buf.Erase(1);
		return pmap.release();
}





















}