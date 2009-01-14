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
 */
#pragma once

#include "TypeDef.h"

namespace Rage {



class RageAPI GlobalSetting : private NonCopyable
{
private:
		typedef ThreadSpace::ReaderWriterMutex	MutexT;
		typedef MutexT::ScopeLock		LockT;
private:
		mutable MutexT					m_mutex;
private:
		t_uint16				m_listen_port;
		std::wstring			m_def_save_path;
private:
		t_uint32				m_max_down_speed;
		t_uint32				m_max_up_speed;
private:
		t_uint32				m_max_request_pending_count;
		t_uint32				m_max_upload_from_local_peer_count;
		t_uint32				m_max_connection_count_per_task; //最大连接数

		size_t					m_max_cache_size;
public:
		t_uint16		GetListenPort()const;
		void		SetListenPort(t_uint16 port);

		t_uint32		GetMaxDownSpeed()const;	//每秒
		void			SetMaxDownSpeed(t_uint32 max_down);

		t_uint32		GetMaxUpSpeed()const;	//每秒
		void			SetMaxUpSpeed(t_uint32 max_up);
		
		void			SetDefaultSavePath(const std::wstring &path);
		std::wstring	GetDefaultSavePath()const;
public:
		t_uint32    GetMaxRequestPendingCount()const;
		void		SetMaxRequestPendingCount(t_uint32 count);

		t_uint32    GetMaxUploadFromLocalPeerCount()const;
		void		SetMaxUploadFromLocalPeerCount(t_uint32 count);

		t_uint32	GetMaxConnectionPerTask()const;
		void		SetMaxConnectionPerTask(t_uint32 max_conn);

		size_t		GetMaxCacheSize()const;
		void		SetMaxCacheSize(size_t size);


private:
		mutable FileSysSpace::File				m_file;
public:
		GlobalSetting(const std::wstring &path_name);
		
		~GlobalSetting();

		void Load();

		void Save()const;
};











}