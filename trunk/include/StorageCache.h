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

#include "TypeDef.h"

namespace Rage {


#define MIN_STORAGE_CACHE_SIZE	(4*MB)

class StorageCache : private NonCopyable
{
private:
		struct PieceCacheInfo
		{
				t_uint32		piece_idx;
				t_uint32		piece_len;
				t_byte			*data;
		};

		typedef std::list<PieceCacheInfo>  CacheList;
private:
		size_t		m_max_cache_size;
		size_t		m_left_cache_size;
		CacheList	m_caches;
public:
		StorageCache();

		~StorageCache();
public:
		void SetMaxCacheSize(size_t max_cache_size);//Set֮�����Ե�ǰ���ݾ�������

		void Clear();//���ٵ�ǰcache������

		//δĳһƬ����cache�������Ƭ�Ѵ��ڣ��򷵻�false;
		bool AllocateCache(t_uint32 piece_idx, size_t piece_len);

		bool RemoveCache(t_uint32 piece_idx);//���ĳһƬ��cache������Ƭ�����ڣ��򷵻�false;
		
		bool IsExisted(t_uint32 piece_idx)const;//��ѯ��Ƭ�Ƿ���cache��

		size_t GetLeftCacheSize()const;

		//���piece_idx�������򷵻�NULL;
		t_byte* GetCache(t_uint32 piece_idx);

	

		//���������ʹ�õ�Ƭ�����Ϊ�գ��򷵻�false
		bool GetLFUCache(t_uint32 &piece_idx)const;

		std::vector<t_uint32> GetCachePieces()const;

};






}


