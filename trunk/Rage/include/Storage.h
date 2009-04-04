#pragma once

#include "TypeDef.h"
#include "StorageCache.h"


namespace Rage{

class TorrentFile;

class FileHandle;

class RageAPI Storage :  private NonCopyable
{
public:
		typedef FileSysSpace::Path				PathT;
		typedef std::vector<FileHandle*>		FileHandleSet;
private:
		FileHandleSet			m_hdl_set;
		std::wstring			m_save_path;
		bool					m_is_open;
private:
		const TorrentFile		&m_torrent_file;
private:
		struct FilePosInfo
		{
				size_t			file_idx;
				t_uint64		start_pos;
				t_uint64		need_len;
		};

		std::vector<FilePosInfo> get_files_by_piece(t_uint32 piece_idx, t_uint32 offset, t_uint32 need_len)const;

private:
		mutable StorageCache			m_read_cache;
		mutable StorageCache			m_write_cache;
public:
		Storage(const std::wstring &save_path, const TorrentFile &tf);
		
		~Storage();
public:
		bool Open();

		void Close();

		bool IsOpen()const;
		
		const std::wstring& GetSavePath()const;

private:
		bool write_data_to_disk(const t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset);

		bool read_data_from_disk(t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset)const;

		bool check_piece_for_disk(t_uint32 piece_idx)const;
public:
		bool WriteData(const t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset);
		
		bool ReadData(t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset)const;
		
		bool WritePiece(const t_byte *buf, t_uint32 piece_idx);

		bool ReadPiece(t_byte *buf, t_uint32 piece_idx)const;

		
		void FlushPiece(t_uint32 piece_idx);

		void SetCacheSize(size_t cache_size);


		t_uint64 GetTotalLength()const;

		bool CheckPiece(t_uint32 piece_idx)const;
		
		//t_uint32 GetPieceLength(t_uint32 piece_idx)const;
};

//例如某个链接断开时候要调用此函数，将此下载中的片重新标记为未请求
		//void MarkPieceFree(t_uint32 piece_idx);






}