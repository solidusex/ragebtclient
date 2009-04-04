
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
#include "Connector.h"
#include "Utility.h"



#if defined(OS_FAMILY_WINDOWS)
		#include "Win/ConnectorImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace NetSpace {





bool Connector::ConnectNB(SockStream &sock_stream, const InetAddress &addr)
{
		assert(sock_stream.IsValid());

		if(!sock_stream.Enable(NET_NONBLOCK)) return false;
		
		return (::connect(sock_stream.GetHandle(), addr.Saddr(), addr.Length()) == 0);
}





///////////////////////////////////ConnectorEx/////////////////////////


ConnectorEx::~ConnectorEx()
{
		//DEBUG_PRINT0("On ConnectorEx::~ConnectorEx()\n");
}




void ConnectorEx::OnTimer()
{
		if(m_ts.ElapsedMillisecond() >= m_timeout)
		{
				on_timeout();
				GetSelector()->RemoveHandler(this);
		}
}

NetHandle ConnectorEx::GetHandle()const
{
		return m_sock.GetHandle();
}

const InetAddress& ConnectorEx::GetRemoteAddr()const
{
		return m_addr;
}


}











