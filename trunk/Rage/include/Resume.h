#pragma once

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

