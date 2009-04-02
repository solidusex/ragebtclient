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
#pragma once

#include "Config.h"
#include "Sha1Hash.h"
#include "PeerEntry.h"
namespace Rage {


#define MIN_PIECE_LENGTH		(32 *KB)

#define MAX_PIECE_LENGTH		(16 *MB)

struct FileInfo
{
		std::wstring	path;
		t_uint64		length;
};


class RageAPI TorrentFile
{
private:
		typedef std::vector<FileInfo>			FileInfoArray;
		typedef std::vector<std::string>		AnnounceArray;
		typedef std::vector<PeerEntry>			NodeArray;
private:
		std::wstring			m_topdir;
		FileInfoArray			m_files;
		t_uint64				m_total_length;
		AnnounceArray			m_announces;
		t_uint32				m_piece_length;
		t_uint32				m_piece_count;
		std::vector<t_byte>		m_pieces_hash_val;
		Sha1Hash				m_infohash;
		NodeArray				m_nodes;

		t_uint64				m_creation_date;
		std::wstring			m_comment;
		std::wstring			m_creator;
public:
		TorrentFile();

		TorrentFile(const TorrentFile &other);

		TorrentFile& operator=(const TorrentFile &other);

		~TorrentFile();

		bool Load(const std::wstring &file_name);

		void Clear();
public:
		std::wstring GetTorrentName()const;

		std::wstring GetTopDir()const;

		t_uint32 GetFileCount()const;
		FileInfo GetFileInfo(t_uint32 idx)const;
		t_uint64 GetTotalSize()const;

		Sha1Hash GetInfoHash()const;
		t_uint32 GetPieceLength()const;
		t_uint32 GetPieceLength(size_t idx)const;
		t_uint32 GetPieceCount()const;
		Sha1Hash GetPieceHash(t_uint32 idx)const;

		t_uint32 GetAnnounceCount()const;
		std::string GetAnnounce(size_t idx)const;

		t_uint32 GetNodeCount()const;
		PeerEntry GetNode(size_t idx)const;

public:
		t_uint64 GetCreationDate()const;
		std::wstring GetComment()const;
		std::wstring GetCreator()const;

public:
		bool MapFileToPieces(size_t file_idx, t_uint32 &piece_beg, t_uint32 &piece_end)const;
};











}