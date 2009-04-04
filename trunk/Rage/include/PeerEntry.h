#pragma once

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