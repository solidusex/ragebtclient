/*
 * The Rage Library
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
 */#pragma once

#include "Config.h"
#include "Sha1Hash.h"
namespace Rage {




struct RageAPI PeerEntry
{
		t_uint32		addr;//网络字节序
		t_uint16		port;//本地字节序
		
		PeerEntry();

		PeerEntry(t_uint32 ip, t_uint16 _port);

		PeerEntry(const PeerEntry &other);

		PeerEntry& operator=(const PeerEntry &other);

		~PeerEntry();

		bool operator==(const PeerEntry &other)const;

		bool operator<(const PeerEntry &other)const;
};




typedef std::list<PeerEntry>					PeerList;












}