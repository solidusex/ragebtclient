#include "SeriWriter.h"


namespace SeriSpace{


SeriWriter::SeriWriter(DSSpace::SimpleBuffer &buf) : m_buf(buf)
{

}
		
SeriWriter::~SeriWriter()
{

}


using namespace FormatSpace;

bool SeriWriter::write_binary(const t_byte *pdata, size_t len)
{
		assert(pdata != NULL && len > 0);
		
		std::string num = NumberFormatter::Format((t_uint32)len);
		assert(!num.empty());
		num += ':';
		m_buf.Insert((const t_byte*)num.c_str(), num.size());
		m_buf.Insert(pdata, len);
		return true;
}

bool SeriWriter::write_int(t_int64 num)
{
		std::string str = "s";

		str += NumberFormatter::Format(num);
		assert(str.size() > 1);
		str += "e";
		m_buf.Insert((const t_byte*)str.c_str(), str.size());
		return true;
}

bool SeriWriter::write_uint(t_uint64 num)
{
		std::string str = "u";

		str += NumberFormatter::Format(num);
		assert(str.size() > 1);
		str += "e";
		m_buf.Insert((const t_byte*)str.c_str(), str.size());
		return true;

}

bool SeriWriter::write_float(double num)
{
		std::string str = "f";

		str += NumberFormatter::Format(num);
		assert(str.size() > 1);
		str += "e";
		m_buf.Insert((const t_byte*)str.c_str(), str.size());

		return true;
}

bool SeriWriter::write_type(const TypeBase *ptype)
{
		assert(ptype != NULL);

		switch(ptype->GetType())
		{
		case BINARY_TYPE:
		{
				const Binary *pb = (const Binary *)ptype;
				return write_binary(pb->Get(), pb->Size());
		}
		break;
		case S_INTEGER_TYPE:
		{
				const SignedInteger *psi = (const SignedInteger*)ptype;
				return write_int(psi->Get());
		}
		break;
		case U_INTEGER_TYPE:
		{
				const UnSignedInteger *pui = (const UnSignedInteger*)ptype;
				return write_uint(pui->Get());
		}
		break;
		case FLOAT_TYPE:
		{
				const Float *pf = (const Float*)ptype;
				return write_float(pf->Get());
		}
		break;
		case LIST_TYPE:
		{
				return write_list((const List*)ptype);
		}
		break;
		case MAP_TYPE:
		{
				return write_map((const Map*)ptype);
		}
		break;
		default:
				assert(0);
				return false;
		}
}


bool SeriWriter::write_list(const List *plist)
{
		assert(plist != NULL);

		t_byte b = (t_byte)'l';
		t_byte e = (t_byte)'e';

		m_buf.Insert(&b, 1);

		for(List::CIterator cit = plist->Begin(); cit != plist->End(); ++cit)
		{
				assert(*cit != NULL);
				if(!write_type(*cit)) return false;
				
		}
		
		m_buf.Insert(&e, 1);
		return true;
}

bool SeriWriter::write_map(const Map *pmap)
{
		t_byte b = (t_byte)'m';
		t_byte e = (t_byte)'e';

		m_buf.Insert(&b, 1);

		for(Map::CIterator cit = pmap->Begin(); cit != pmap->End(); ++cit)
		{
				assert(cit->second != NULL);
				assert(!cit->first.empty());

				if(!write_binary((const t_byte*)cit->first.c_str(), cit->first.size())) return false;
				if(!write_type(cit->second)) return false;
		}
		
		m_buf.Insert(&e, 1);
		return true;
}

bool SeriWriter::Generate(const TypeBase *ptype)
{
		assert(ptype != NULL);
		Clear();
		return write_type(ptype);
}


void SeriWriter::Clear()
{
		m_buf.Clear();
}


const t_byte* SeriWriter::Get()const
{
		return m_buf.RawData();
}
		
size_t SeriWriter::Size()const
{
		return m_buf.RawBufSize();
}











}