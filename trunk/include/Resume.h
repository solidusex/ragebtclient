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
#include "Sha1Hash.h"



namespace Rage {

struct ResumeTaskInfo
{
		Sha1Hash				infohash;
		std::vector<t_byte>		own_pieces;
		std::wstring			save_path;
		std::wstring			torrent_name;
};


class RageAPI ResumeInfo : NonCopyable
{
private:
		typedef std::map<Sha1Hash, ResumeTaskInfo>		TaskInfoMap;
public:
		typedef TaskInfoMap::const_iterator		CIterator;
private:
		const std::wstring				m_info_file_path;
		mutable FileSysSpace::File		m_file;
		TaskInfoMap						m_task_map;
public:
		ResumeInfo(const std::wstring &file_path);
		
		~ResumeInfo();
public:
		void Load();

		bool Save()const;

		bool IsExisted(const Sha1Hash &infohash)const;

		bool Add(const ResumeTaskInfo &tinfo);

		bool Update(const ResumeTaskInfo &tinfo);//如果还没增加进来，则返回false

		bool Remove(const Sha1Hash &infohash);

		void ClearTaskInfo();
public:
		CIterator Begin()const;

		CIterator End()const;



};











}

