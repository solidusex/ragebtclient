
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
#pragma once

#include "ByteStream.h"



namespace StreamSpace {


typedef AbstractByteStreamBuffer  AbstractDataStreamBuffer;

/********************************************DataInputStream*********************/
#define READ_FUNC(Type, Name) Type Read##Name()
		
#define READ_ARRAY_FUNC(Type, Name) size_t Read##Name##Array(Type *arr, size_t len)

#define DECLARE_FUNC(Type, Name)\
		READ_FUNC(Type, Name);\
		READ_ARRAY_FUNC(Type, Name)


//class FoundationAPI DataInputStream : public virtual ByteInputStream
class FoundationAPI DataInputStream : public ByteInputStream
{
public:
		DataInputStream(AbstractDataStreamBuffer  *pbuf);
		
		virtual ~DataInputStream();
public:
		DECLARE_FUNC(bool, Bool);
		
		DECLARE_FUNC(float, Float);
		DECLARE_FUNC(double, Double);
		DECLARE_FUNC(t_wchar, WChar);
		DECLARE_FUNC(t_byte,Byte);
		
		DECLARE_FUNC(t_char,Char);
		DECLARE_FUNC(t_uchar,UChar);
		DECLARE_FUNC(t_int8, Int8);
		DECLARE_FUNC(t_uint8, UInt8);
		DECLARE_FUNC(t_int16, Int16);
		DECLARE_FUNC(t_uint16,UInt16);
		DECLARE_FUNC(t_int32, Int32);
		DECLARE_FUNC(t_uint32, UInt32);
		DECLARE_FUNC(t_int64, Int64);
		DECLARE_FUNC(t_uint64,UInt64);
};

#undef READ_FUNC
#undef READ_ARRAY_FUNC
#undef DECLARE_FUNC

/********************************************DataOutputStream*********************/

#define WRITE_FUNC(Type, Name) void Write##Name(const Type &item)

#define WRITE_ARRAY_FUNC(Type, Name)\
		void Write##Name##Array(const Type *arr, size_t len)
		

#define DLC_FUNC(Type, Name)\
		WRITE_FUNC(Type, Name);\
		WRITE_ARRAY_FUNC(Type, Name)


//class FoundationAPI DataOutputStream : public virtual ByteOutputStream
class FoundationAPI DataOutputStream : public ByteOutputStream
{
public:
		DataOutputStream(AbstractDataStreamBuffer  *pbuf);
		virtual ~DataOutputStream();
public:
		DLC_FUNC(bool, Bool);
		DLC_FUNC(float, Float);
		DLC_FUNC(double, Double);
		DLC_FUNC(t_wchar, WChar);
		DLC_FUNC(t_byte,Byte);
		DLC_FUNC(t_char,Char);
		DLC_FUNC(t_uchar,UChar);
		DLC_FUNC(t_int8, Int8);
		DLC_FUNC(t_uint8, UInt8);
		DLC_FUNC(t_int16, Int16);
		DLC_FUNC(t_uint16,UInt16);
		DLC_FUNC(t_int32, Int32);
		DLC_FUNC(t_uint32, UInt32);
		DLC_FUNC(t_int64, Int64);
		DLC_FUNC(t_uint64,UInt64);
};

#undef WRITE_FUNC
#undef WRITE_ARRAY_FUNC
#undef DLC_FUNC


///////////////////////////////////////////////////////////////////////////////



template<class DataStreamBuffer>
class DataIOStream : public DataInputStream, public DataOutputStream
{
private:
		DataStreamBuffer				*m_pbuf;
public:
		DataIOStream(DataStreamBuffer *pbuf = new DataStreamBuffer())
				: DataInputStream(pbuf)
				, DataOutputStream(pbuf)
				,m_pbuf(pbuf)
		{


		}

		virtual ~DataIOStream()
		{
				delete m_pbuf;
		}
};





}