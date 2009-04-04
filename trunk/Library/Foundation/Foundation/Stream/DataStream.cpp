
/*
 * The Foundation Library
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
 */
#include "DataStream.h"



namespace StreamSpace {


/**************************DataInputStream***************************************/


DataInputStream::DataInputStream(AbstractDataStreamBuffer  *pbuf) :ByteInputStream(pbuf)
{

}
		
DataInputStream::~DataInputStream()
{

}
/*
#define READ_FUNCTION(Type, Name)								\
Type DataInputStream::Read##Name()								\
{																\
		if(Available() < sizeof(Type))							\
		{														\
				throw ExceptionSpace::DataException("buf : overflow");\
		}else													\
		{														\
				t_byte buf[sizeof(Type)];						\
				Read(buf, sizeof(Type));						\
				Type tmp;										\
				memcpy((void*)&tmp, buf, sizeof(Type));			\
				return tmp;										\
		}														\
}*/

#define READ_FUNCTION(Type, Name)								\
Type DataInputStream::Read##Name()								\
{																\
		if(Available() < sizeof(Type))							\
		{														\
				throw ExceptionSpace::DataException("buf : overflow");\
		}else													\
		{														\
				Type tmp;										\
				Read((t_byte*)&tmp, sizeof(Type));				\
				return tmp;										\
		}														\
}

#define READ_ARRAY_FUNCTION(Type, Name)\
size_t DataInputStream::Read##Name##Array(Type *arr, size_t len)		\
{																		\
		if((Available() < (sizeof(Type) * len)))						\
		{																\
				throw ExceptionSpace::DataException("buf : overflow");	\
		}																\
		for(size_t i = 0; i < len; ++i)									\
		{																\
				Read((t_byte*)&arr[i], sizeof(Type));					\
		}																\
		return len;														\
}

#define DEFINE_FUNC(Type, Name)\
		READ_FUNCTION(Type,Name)\
		READ_ARRAY_FUNCTION(Type, Name)
		

		DEFINE_FUNC(bool, Bool)
		DEFINE_FUNC(t_wchar, WChar)
		DEFINE_FUNC(t_char,Char)
		DEFINE_FUNC(t_uchar,UChar)

		DEFINE_FUNC(float, Float)
		DEFINE_FUNC(double, Double)

		DEFINE_FUNC(t_byte,Byte)

		DEFINE_FUNC(t_int8, Int8)
		DEFINE_FUNC(t_uint8, UInt8)
		DEFINE_FUNC(t_int16, Int16)
		DEFINE_FUNC(t_uint16,UInt16)
		DEFINE_FUNC(t_int32, Int32)
		DEFINE_FUNC(t_uint32, UInt32)
		DEFINE_FUNC(t_int64, Int64)
		DEFINE_FUNC(t_uint64,UInt64)

		

#undef READ_FUNCTION

#undef READ_ARRAY_FUNCTION

#undef DEFINE_FUNC

/********************************************DataOutputStream*********************/


DataOutputStream::DataOutputStream(AbstractDataStreamBuffer  *pbuf) : ByteOutputStream(pbuf)
{

}

		
DataOutputStream::~DataOutputStream()
{


}



#define WRITE_DEF_F(Type, Name)								\
void DataOutputStream::Write##Name(const Type &item)			\
{																\
		Write((const t_byte*)&item, sizeof(Type));				\
}												


#define WRITE_ARRAY_DEF_F(Type, Name)\
void DataOutputStream::Write##Name##Array(const Type *arr, size_t len)\
{																		\
																		\
		Write((const t_byte*)arr, (sizeof(Type) * len));				\
}																										



#define DEF_FUNC(Type, Name)\
		WRITE_DEF_F(Type, Name)\
		WRITE_ARRAY_DEF_F(Type, Name)




		DEF_FUNC(bool, Bool)
		DEF_FUNC(t_wchar, WChar)
		DEF_FUNC(t_char,Char)
		DEF_FUNC(t_uchar,UChar)

		DEF_FUNC(float, Float)
		DEF_FUNC(double, Double)

		DEF_FUNC(t_byte,Byte)

		DEF_FUNC(t_int8, Int8)
		DEF_FUNC(t_uint8, UInt8)
		DEF_FUNC(t_int16, Int16)
		DEF_FUNC(t_uint16,UInt16)
		DEF_FUNC(t_int32, Int32)
		DEF_FUNC(t_uint32, UInt32)
		DEF_FUNC(t_int64, Int64)
		DEF_FUNC(t_uint64,UInt64)





#undef WRITE_DEF_F

#undef WRITE_ARRAY_DEF_F

#undef DEF_FUNC




}