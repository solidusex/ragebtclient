
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
#include "Utility.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/UtilityImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace NetSpace {





int Utility::HandleReady(NetHandle sock, const t_uint64 *timeout, bool read, bool write, bool expt)
{
		assert(sock != NET_INVALID_SOCKET);
		SocketSet s_set;
		s_set.Set(sock);

		return Select( (read ? &s_set : 0),
				(write ? &s_set : 0),
				(expt ? &s_set : 0),
				timeout
				);
		
}


int Utility::HandleReadReady (NetHandle sock, const t_uint64 *timeout)
{
		assert(sock != NET_INVALID_SOCKET);
		return HandleReady(sock, timeout, true, false, false);
}

  
int Utility::HandleWriteReady (NetHandle sock, const t_uint64 *timeout)
{
		assert(sock != NET_INVALID_SOCKET);
		return HandleReady(sock, timeout, false, true, false);

}

  
int Utility::HandleExceptionReady (NetHandle sock, const t_uint64 *timeout)
{
		assert(sock != NET_INVALID_SOCKET);
		return HandleReady(sock, timeout, false, false, true);

}
























}

