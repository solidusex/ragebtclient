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