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
 */#include "Res.h"


namespace Rage {


////////////////////////////////////////////////////////////////////////////

IPFilter::IPFilter()
{

}
		
IPFilter::~IPFilter()
{



}

void IPFilter::BandAddress(const std::string &ip, t_uint16 port)
{

		NetSpace::InetAddress addr(ip, port);

		PeerEntry pe(addr.IP(), addr.Port());
		LockT lock(m_mutex, false);
		m_set.insert(pe);
}

void IPFilter::BandAddress(const PeerEntry &pe)
{
		LockT lock(m_mutex, false);
		m_set.insert(pe);

}
		
void IPFilter::UnBandAddress(const std::string &ip, t_uint16 port)
{
		NetSpace::InetAddress addr(ip, port);
		PeerEntry pe(addr.IP(), addr.Port());
		LockT lock(m_mutex, false);
		m_set.erase(pe);
}

void IPFilter::UnBandAddress(const PeerEntry &pe)
{
		LockT lock(m_mutex, false);
		m_set.erase(pe);
}

size_t IPFilter::GetBandAddrCount()const
{
		LockT lock(m_mutex, true);
		return m_set.size();

}

		
bool IPFilter::IsBand(const std::string &ip, t_uint16 port)const
{
		NetSpace::InetAddress addr(ip, port);
		PeerEntry pe(addr.IP(), addr.Port());
		LockT lock(m_mutex, true);
		return (m_set.count(pe) == 1);
}


bool IPFilter::IsBand(const PeerEntry &pe)const
{
		LockT lock(m_mutex, true);
		return (m_set.count(pe) == 1);

}


void IPFilter::Clear()
{
		LockT lock(m_mutex, false);
		m_set.clear();
}


////////////////////////////////////////////////////////////////////////////////////



DownloaderRecorder::DownloaderRecorder()
{

}

DownloaderRecorder::~DownloaderRecorder()
{

}


bool DownloaderRecorder::IsExisted(const Sha1Hash &info_hash)
{
		LockT lock(m_mutex, true);
		return (m_map.count(info_hash) == 1);
}

bool DownloaderRecorder::GetRecord(const Sha1Hash &info_hash, DownloaderInfo &dl_info)
{
		LockT lock(m_mutex, true);

		RecordMap::iterator it = m_map.find(info_hash);

		if(it != m_map.end())
		{
				dl_info = it->second;
				return true;
		}else
		{
				return false;
		}

}

bool DownloaderRecorder::RegisterRecord(const Sha1Hash &info_hash, const PeerID &peer_id, TaskID task_id)
{
		assert(task_id != INVALID_TASK_ID);
		LockT lock(m_mutex, false);
		
		if(m_map.count(info_hash) == 1) return false;

		DownloaderInfo dinfo;
		dinfo.info_hash = info_hash;
		dinfo.peer_id = peer_id;
		dinfo.task_id = task_id;
		m_map[info_hash] = dinfo;
		return true;
}

bool DownloaderRecorder::RemoveRecord(const Sha1Hash &info_hash)
{
		LockT lock(m_mutex, false);
		RecordMap::iterator it = m_map.find(info_hash);
		if(it == m_map.end()) return false;
		m_map.erase(it);
		return true;
}


















//////////////////////////////////////////////////////////////////////////////////

DownloaderRecorder& ResourceManager::GetDownloaderRecorder()
{
		return m_recorder;
}

IPFilter& ResourceManager::GetIPFilter()
{
		return m_ip_filter;
}




}