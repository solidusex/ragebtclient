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
 */#include "StorageCache.h"

namespace Rage {





StorageCache::StorageCache() 
		: m_max_cache_size(MIN_STORAGE_CACHE_SIZE)
		, m_left_cache_size(MIN_STORAGE_CACHE_SIZE)
{

}

StorageCache::~StorageCache()
{
		Clear();
}


void StorageCache::SetMaxCacheSize(size_t max_cache_size)
{
		if(max_cache_size < MIN_STORAGE_CACHE_SIZE)
		{
				max_cache_size = MIN_STORAGE_CACHE_SIZE;
		}

		Clear();

		m_max_cache_size = max_cache_size; 
		m_left_cache_size = m_max_cache_size;
}

void StorageCache::Clear()
{
		for(CacheList::iterator it = m_caches.begin(); it != m_caches.end(); ++it)
		{
				delete it->data;
		}

		m_caches.clear();
		m_left_cache_size = m_max_cache_size;
}


size_t StorageCache::GetLeftCacheSize()const
{
		return m_left_cache_size;
}


		//以下两个函数如果piece_idx不存在则返回NULL;
t_byte* StorageCache::GetCache(t_uint32 piece_idx)
{
		for(CacheList::iterator it = m_caches.begin(); it != m_caches.end(); ++it)
		{
				if(it->piece_idx == piece_idx)
				{
						PieceCacheInfo pci = *it;
						m_caches.erase(it);
						m_caches.push_back(pci);
						return pci.data;
				}
		}

		return 0;

}




		
bool StorageCache::GetLFUCache(t_uint32 &piece_idx)const
{
		if(m_caches.empty()) return false;

		piece_idx = m_caches.begin()->piece_idx;
		return true;
}

std::vector<t_uint32> StorageCache::GetCachePieces()const
{
		std::vector<t_uint32> res;

		for(CacheList::const_iterator it = m_caches.begin(); it != m_caches.end(); ++it)
		{
				res.push_back(it->piece_idx);
		}

		return res;

}


bool StorageCache::AllocateCache(t_uint32 piece_idx, size_t piece_len)
{
		if(IsExisted(piece_idx)) return false;
		
		if(m_left_cache_size < piece_len) return false;

		PieceCacheInfo pci;
		pci.piece_idx = piece_idx;
		pci.piece_len = piece_len;
		pci.data = new t_byte[piece_len];
		m_caches.push_back(pci);
		m_left_cache_size -= piece_len;
		return true;
}

bool StorageCache::RemoveCache(t_uint32 piece_idx)
{
		for(CacheList::iterator it = m_caches.begin(); it != m_caches.end(); ++it)
		{
				if(it->piece_idx == piece_idx)
				{
						delete it->data;
						m_left_cache_size += it->piece_len;
						m_caches.erase(it);
						return true;
				}
		}
		return false;

}
		
bool StorageCache::IsExisted(t_uint32 piece_idx)const
{
		for(CacheList::const_iterator it = m_caches.begin(); it != m_caches.end(); ++it)
		{
				if(it->piece_idx == piece_idx) return true;
		}

		return false;

}
















}