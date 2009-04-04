
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

#include "SocketSet.h"
#include "TypeDef.h"


namespace NetSpace {



class FoundationAPI Utility
{
public:
		//timeout ¶¼Îªmillisecond
		static int Select(::fd_set *prd, ::fd_set *pwd, ::fd_set *pexd, const t_uint64 *timeout);

		static int Select(SocketSet *rd, SocketSet *wd, SocketSet *exd, const t_uint64 *timeout);
		
		
		static int HandleReady(NetHandle sock, const t_uint64 *timeout = 0, 
				bool read = true, bool write = false, bool expt = false);

		static int HandleReadReady (NetHandle sock, const t_uint64 *timeout = 0);

		static int HandleWriteReady (NetHandle sock, const t_uint64 *timeout = 0);

		static int HandleExceptionReady (NetHandle sock, const t_uint64 *timeout = 0);
		
};











}