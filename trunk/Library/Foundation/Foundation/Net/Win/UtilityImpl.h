
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

#include "../Utility.h"

namespace NetSpace {


int Utility::Select(fd_set *prd, fd_set *pwd, fd_set *pexd, const t_uint64 *timeout)
{
		::timeval tv;
		tv.tv_sec = 0;
		
		::timeval *ptv = (timeout != 0 ? &tv : 0);

		if(ptv != 0)
		{
				ptv->tv_usec = (*timeout * 1000);
		}

		int res = ::select(0, prd, pwd, pexd, ptv);
		
		if(res == 0)
		{
				NetSetLastError(ETIME);
		}

		return res;
}

int Utility::Select(SocketSet *rd, SocketSet *wd, SocketSet *exd, const t_uint64 *timeout)
{
		::fd_set *prd = 0, *pwd = 0, *pexd = 0;

		if(rd != 0)
		{
				rd->Sync();
				prd = rd->Fdset();
		}

		if(wd != 0)
		{
				wd->Sync();
				pwd = wd->Fdset();
		}

		if(exd != 0)
		{
				exd->Sync();
				pexd = exd->Fdset();
		}
		return Select(prd, pwd, pexd, timeout);
}



}