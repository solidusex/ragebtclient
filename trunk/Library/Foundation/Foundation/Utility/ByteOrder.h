
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


#include "../Common/ComnHdr.h"
	




class FoundationAPI ByteOrder
{
public:
		enum NATIVE_BYTE_ORDER { BIG_ENDIAN = 0, LITTLE_ENDIAN = 1 };
private:
		static const NATIVE_BYTE_ORDER		NativeByteOrder;
public:
		static NATIVE_BYTE_ORDER GetLocalByteOrder();
		static t_uint16 FlipBytes(t_uint16 val);
		static t_int16 FlipBytes(t_int16 val);
		
		static t_uint32 FlipBytes(t_uint32 val);
		static t_int32 FlipBytes(t_int32 val);
		
		static t_uint64 FlipBytes(t_uint64 val);
		static t_int64 FlipBytes(t_int64 val);
		
		static t_uint16 FromNativeOrderToNetOrder(t_uint16 val);
		static t_int16 FromNativeOrderToNetOrder(t_int16 val);
		
		static t_uint32 FromNativeOrderToNetOrder(t_uint32 val);
		static t_int32 FromNativeOrderToNetOrder(t_int32 val);
	
		static t_uint64 FromNativeOrderToNetOrder(t_uint64 val);
		static t_int64 FromNativeOrderToNetOrder(t_int64 val);
//////////////////////////////////////////////////////////////////////////////////

		static t_uint16 FromNetOrderToNativeOrder(t_uint16 val);
		static t_int16 FromNetOrderToNativeOrder(t_int16 val);
		
		static t_uint32 FromNetOrderToNativeOrder(t_uint32 val);
		static t_int32 FromNetOrderToNativeOrder(t_int32 val);
		
		static t_uint64 FromNetOrderToNativeOrder(t_uint64 val);
		static t_int64 FromNetOrderToNativeOrder(t_int64 val);
};

		











