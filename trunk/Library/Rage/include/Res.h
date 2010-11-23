
/*
 * The Rage Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  It is provided "as is" without express
 * or implied warranty.
 */

#pragma once

#include "TypeDef.h"
#include "PeerEntry.h"

namespace Rage {



class RageAPI IPFilter : private NonCopyable
{
private:
		typedef ThreadSpace::ReaderWriterMutex	MutexT;
		typedef MutexT::ScopeLock				LockT;
		typedef std::set<PeerEntry>				PeerEntrySet;
public:
		mutable MutexT							m_mutex;
		PeerEntrySet							m_set;
public:
		IPFilter();
		
		~IPFilter();
public:
		void BandAddress(const std::string &ip, t_uint16 port);
		void BandAddress(const PeerEntry &pe);
		void UnBandAddress(const std::string &ip, t_uint16 port);
		void UnBandAddress(const PeerEntry &pe);
		bool IsBand(const std::string &ip, t_uint16 port)const;
		bool IsBand(const PeerEntry &pe)const;

		void Clear();
		size_t GetBandAddrCount()const;
};


struct DownloaderInfo
{
		TaskID			task_id;
		Sha1Hash		info_hash;
		PeerID			peer_id;
};

class RageAPI DownloaderRecorder : private NonCopyable
{
private:
		typedef std::map<Sha1Hash, DownloaderInfo>		RecordMap;
		typedef ThreadSpace::ReaderWriterMutex			MutexT;
		typedef MutexT::ScopeLock						LockT;
private:
		RecordMap										m_map;
		MutexT											m_mutex;
public:
		DownloaderRecorder();

		~DownloaderRecorder();
public:
		bool IsExisted(const Sha1Hash &info_hash);

		bool GetRecord(const Sha1Hash &info_hash, DownloaderInfo &dl_info);
		
		bool RegisterRecord(const Sha1Hash &info_hash, const PeerID &peer_id, TaskID task_id);

		bool RemoveRecord(const Sha1Hash &info_hash);
};




class RageAPI ResourceManager : private NonCopyable
{
private:
		IPFilter				m_ip_filter;
		DownloaderRecorder		m_recorder;
public:
		DownloaderRecorder& GetDownloaderRecorder();
		IPFilter& GetIPFilter();
};















}