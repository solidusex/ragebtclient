/*
* The Rage Library
* Copyright (c) 2007 by Solidus
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* provided that the above copyright notice appear in all copies and
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*
*/
#include "PeerEntry.h"

namespace Rage {


PeerEntry::PeerEntry() : addr(0), port(0)
{


}


PeerEntry::PeerEntry(t_uint32 ip, t_uint16 _port)
{
		addr = ip;
		port = _port;
}

PeerEntry::PeerEntry(const PeerEntry &other)
{
		*this = other;
}

PeerEntry& PeerEntry::operator=(const PeerEntry &other)
{
		if(this != &other)
		{
				addr = other.addr;
				port = other.port;
		}

		return *this;
}

PeerEntry::~PeerEntry()
{

}

bool PeerEntry::operator==(const PeerEntry &other)const
{
		return (addr == other.addr && port == other.port);

}

bool PeerEntry::operator<(const PeerEntry &other)const
{
		return (addr < other.addr);
}


/*
bool PeerEntry::operator<(const PeerEntry &other)const
{
		const t_uint8 *pthis			= (const t_uint8*)&addr;
		const t_uint8 *pother			=  (const t_uint8*)&(other.addr);

		for(size_t i = 0; i < 4; ++i)
		{
				if(pthis[i] < pother[i])
				{
						return true;
				}else if(pthis[i] > pother[i])
				{
						return false;
				}
		}
		return false;
}*/

}