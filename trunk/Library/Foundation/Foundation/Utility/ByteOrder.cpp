/*
 * The Foundation Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  It is provided "as is" without express
 * or implied warranty.
 */
#include "ByteOrder.h"
#include <cassert>
#include <stdexcept>



const ByteOrder::NATIVE_BYTE_ORDER ByteOrder::NativeByteOrder = ByteOrder::GetLocalByteOrder();
				


ByteOrder::NATIVE_BYTE_ORDER ByteOrder::GetLocalByteOrder ()
{
		assert(sizeof(t_uint16) == 2);

		union
		{
				t_uint16 s;
				t_byte c[sizeof(t_uint16)];
		} un;
		un.s = 0x0201;
		
		if ( ( 2 == un.c[0] ) && ( 1 == un.c[1] ) )
		{
				return BIG_ENDIAN;
		}
		else if ( ( 1 == un.c[0] ) && ( 2 == un.c[1] ) )
		{
				return LITTLE_ENDIAN;
		}
		else
		{
				assert(false);
				throw std::runtime_error("Unknow Byte Order");
		}
		
}


t_uint16 ByteOrder::FlipBytes(t_uint16 val)
{
		return (((val >> 8) & 0x00FF) | ((val << 8) & 0xFF00));
}

t_uint32 ByteOrder::FlipBytes(t_uint32 val)
{
		return (
				((val >> 24) &0x000000FF)|
				((val >> 8) & 0x0000FF00)|
				((val << 8) & 0x00FF0000)|
				((val << 24) & 0xFF000000)
				);
}

t_uint64 ByteOrder::FlipBytes(t_uint64 val)
{
		t_uint32 hi = t_uint32( val >> 32);
		t_uint32 lo = t_uint32(val  & 0xFFFFFFFF);

		t_uint64 fhi = t_uint64(FlipBytes(hi));
		t_uint64 flo = t_uint64(FlipBytes(lo));

		flo <<= 32;
		return (flo | fhi);

}


t_int64 ByteOrder::FlipBytes(t_int64 val)
{
		return FlipBytes(t_uint64(val));
}

t_int32 ByteOrder::FlipBytes(t_int32 val)
{
		return FlipBytes((t_uint32)val);
}

t_int16 ByteOrder::FlipBytes(t_int16 val)
{
		return FlipBytes((t_uint16)val);
}


#define DEFINE_NATIVE_T_NET(type)										\
		type ByteOrder::FromNativeOrderToNetOrder(type val)				\
		{																\
				if(NativeByteOrder != BIG_ENDIAN)						\
				{														\
						val = FlipBytes(val);							\
				}														\
				return val;												\
		}																\


DEFINE_NATIVE_T_NET(t_int16)
DEFINE_NATIVE_T_NET(t_uint16)

DEFINE_NATIVE_T_NET(t_int32)
DEFINE_NATIVE_T_NET(t_uint32)


DEFINE_NATIVE_T_NET(t_int64)
DEFINE_NATIVE_T_NET(t_uint64)







#define DEFINE_NET_T_NATIVE(type)										\
		type ByteOrder::FromNetOrderToNativeOrder(type val)				\
		{																\
				if(NativeByteOrder != BIG_ENDIAN)					\
				{														\
						val = FlipBytes(val);							\
				}														\
				return val;												\
		}																\


DEFINE_NET_T_NATIVE(t_int16)
DEFINE_NET_T_NATIVE(t_uint16)

DEFINE_NET_T_NATIVE(t_int32)
DEFINE_NET_T_NATIVE(t_uint32)


DEFINE_NET_T_NATIVE(t_int64)
DEFINE_NET_T_NATIVE(t_uint64)
