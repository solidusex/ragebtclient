#include "Storage.h"
#include "TorrentFile.h"

namespace Rage{


class FileHandle : NonCopyable
{
private:
		typedef FileSysSpace::File				FileT;
		typedef FileSysSpace::Path				PathT;
		//typedef FileSysSpace::MemMapFile		MemMapFileT;
		//typedef FileSysSpace::MemMapFileView	MemMapFileViewT;
private:
		FileT						m_file;
		std::wstring				m_path;
		t_uint64					m_need_length;	//被torrent文件指定的实际长度，相对于当前的长度而言
public:
		FileHandle(const std::wstring &path, t_uint64 need_length);

		~FileHandle();
public:
		bool Open();

		void Close();

		bool IsOpen()const;

		//从当前长度增加size个字节,如果当前size + size超过m_need_length会抛出异常
		//如果增加失败会会返回false
		bool IncreaseFileSize(t_uint64 size);

		bool TruncateFileSize(t_uint64  size);

		bool FullAllocate();
public:
		size_t Read(t_byte *buf, size_t len, t_uint64 offset);
		
		size_t Write(const t_byte *buf, size_t len, t_uint64 offset);

		bool IsExisted()const;

		bool Make()const;
public:
		t_uint64 GetRealLength()const;
		
		t_uint64 GetNeedLength()const;

		const std::wstring& GetFullPath()const;
};


FileHandle::FileHandle(const std::wstring &path, t_uint64 need_length)
{
		m_path = path;
		m_need_length = need_length;
}

FileHandle::~FileHandle()
{
		if(IsOpen())
		{
				Close();
		}
}

size_t FileHandle::Read(t_byte *buf, size_t len, t_uint64 offset)
{
		assert(buf != 0);
		assert(len != 0);
		assert(IsOpen());
		
		assert((offset + len) <= GetRealLength());
		
		t_uint64 pos = m_file.Seek(offset, FileSysSpace::BEG, true);
		assert(pos == offset);

		t_uint64 rn = m_file.Read(buf, len);

		assert(rn == len);
		
		return (size_t)rn;
}


size_t FileHandle::Write(const t_byte *buf, size_t len, t_uint64 offset)
{
		assert(IsOpen());
		assert(buf != 0);
		assert(len != 0);
		
		assert((offset + len) <= GetRealLength());
		
		t_uint64 pos = m_file.Seek(offset, FileSysSpace::BEG, true);
		assert(pos == offset);
		t_uint64 wn = m_file.Write(buf, len);
		assert(wn == len);
		
		return (size_t)wn;

}


bool FileHandle::IncreaseFileSize(t_uint64 size)
{
		assert(IsOpen());
		try{
				t_uint64 new_len = m_file.GetLength() + size;
				
				assert(new_len <= m_need_length);

				if(new_len > m_need_length)
				{
						new_len = m_need_length;
				}
				m_file.SetLength(new_len);
				
		}catch(std::exception &expt)
		{
				DEBUG_PRINT1("FileHandle::IncreaseFileSize == %s\n", expt.what());
				return false;

		}
		return true;
}

bool FileHandle::TruncateFileSize(t_uint64  size)
{
		try{
				assert(IsOpen());
				
				t_uint64 new_len = 0;
				
				if(m_file.GetLength() > size)
				{
						new_len = m_file.GetLength() - size;
				}
				m_file.SetLength(new_len);
		}catch(std::exception &expt)
		{
				DEBUG_PRINT1("FileHandle::TruncateFileSize == %s\n", expt.what());
				return false;

		}
		return true;
}


bool FileHandle::FullAllocate()
{
		assert(IsOpen());
		t_uint64 real_len = GetRealLength();
		assert(real_len <= m_need_length);
		return IncreaseFileSize(m_need_length - real_len);


}





bool FileHandle::Open()
{
		if(m_file.IsOpen())
		{
				return false;
		}else
		{
				return m_file.Open(m_path, FileSysSpace::STD_WRITE);
		}
}

void FileHandle::Close()
{
		if(m_file.IsOpen())
		{
				m_file.Close();
		}
}




bool FileHandle::IsOpen()const
{
		return m_file.IsOpen();
}

bool FileHandle::IsExisted()const
{
		PathT _path(m_path);
		assert(_path.GetPathType() == FileSysSpace::PATH_FILE);
		return _path.IsExisted();
}

bool FileHandle::Make()const
{
		if(IsOpen()) return true;
		
		PathT _path(m_path);
		assert(_path.GetPathType() == FileSysSpace::PATH_FILE);
		return _path.Make();
}


t_uint64 FileHandle::GetRealLength()const
{
		return m_file.GetLength();
}
		
t_uint64 FileHandle::GetNeedLength()const
{
		return m_need_length;
}

const std::wstring& FileHandle::GetFullPath()const
{
		return m_path;
}









//////////////////////////////////////////////Storage/////////////////////////////////////////
bool Storage::IsOpen()const
{
		return m_is_open;
}

Storage::Storage(const std::wstring &save_path, const TorrentFile &tf) 
				: m_is_open(false)
				, m_save_path(save_path)
				, m_torrent_file(tf)
{


}
		
Storage::~Storage()
{
		if(IsOpen())
		{
				Close();
		}
}


std::vector<Storage::FilePosInfo> Storage::get_files_by_piece(t_uint32 piece_idx, t_uint32 offset, t_uint32 need_len)const
{
		assert(IsOpen());

		assert(piece_idx < m_torrent_file.GetPieceCount());
		assert(offset < m_torrent_file.GetPieceLength(piece_idx));
		
		t_uint64 pos_in_total = (t_uint64)piece_idx * (t_uint64)m_torrent_file.GetPieceLength() + offset;
		
		size_t file_idx = 0;
		for(; file_idx < m_hdl_set.size(); ++file_idx)
		{
				if(pos_in_total < m_hdl_set[file_idx]->GetNeedLength())
				{
						break;
				}else
				{
						pos_in_total -= m_hdl_set[file_idx]->GetNeedLength();
				}
		}

		std::vector<FilePosInfo> res;
		t_uint64 file_start_pos = pos_in_total;

		while(need_len > 0)
		{
				FilePosInfo fi;
				fi.file_idx = file_idx;
				fi.start_pos = file_start_pos;

				t_uint64 file_remain_len = m_hdl_set[file_idx]->GetNeedLength() - file_start_pos;
				fi.need_len = (file_remain_len <  need_len ? file_remain_len : need_len);
				res.push_back(fi);
				need_len -= (t_uint64)fi.need_len;
				file_start_pos = 0;
				file_idx++;
		}

		return res;


}






bool Storage::write_data_to_disk(const t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset)
{
		assert(buf != 0 && len != 0 && piece_idx < m_torrent_file.GetPieceCount());
		assert(IsOpen());

		std::vector<FilePosInfo> files = get_files_by_piece(piece_idx, offset, len);
		if(files.empty())return false;

		
		const t_byte *pbuf = buf;
		
		for(size_t idx = 0; idx < files.size(); ++idx)
		{
				size_t file_hdl_idx = files[idx].file_idx;
				
				if(files[idx].need_len != 0)
				{
						size_t wn = m_hdl_set[file_hdl_idx]->Write(pbuf, files[idx].need_len, files[idx].start_pos); 
						
						if(wn != files[idx].need_len)
						{
								throw ExceptionSpace::FileException("Storage::write_data_to_disk : Write File Failed");
						}

						pbuf += files[idx].need_len;
				}
		}

		return true;

}

bool Storage::read_data_from_disk(t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset)const
{
		assert(buf != 0 && len != 0 && piece_idx < m_torrent_file.GetPieceCount());
		assert(IsOpen());
		
		std::vector<FilePosInfo> files = get_files_by_piece(piece_idx, offset, len);
		if(files.empty())return false;

		
		t_byte *pbuf = buf;
		
		for(size_t idx = 0; idx < files.size(); ++idx)
		{
				size_t file_hdl_idx = files[idx].file_idx;
				if(files[idx].need_len != 0)
				{
						size_t rn = m_hdl_set[file_hdl_idx]->Read(pbuf, files[idx].need_len, files[idx].start_pos); 
						
						if(rn != files[idx].need_len)
						{
								throw ExceptionSpace::FileException("Storage::read_data_from_disk : Read File Failed");
						}
						pbuf += files[idx].need_len;
				}
		}
		return true;

}

void Storage::SetCacheSize(size_t cache_size)
{
		const size_t piece_len = m_torrent_file.GetPieceLength();

		cache_size  = cache_size / piece_len * piece_len;

		if(cache_size < (piece_len * 3))
		{
				cache_size = (piece_len * 3);
		}

		
		
		std::vector<t_uint32> w_cache_pieces = m_write_cache.GetCachePieces();

		for(size_t i = 0; i < w_cache_pieces.size(); ++i)
		{
				FlushPiece(w_cache_pieces[i]);
		}

		m_write_cache.Clear();
		m_read_cache.Clear();

		m_write_cache.SetMaxCacheSize(cache_size / 3 );
		m_read_cache.SetMaxCacheSize(cache_size / 3 * 2);
}

void Storage::FlushPiece(t_uint32 piece_idx)
{
		if(m_write_cache.IsExisted(piece_idx))
		{
				const t_byte *pbuf = m_write_cache.GetCache(piece_idx);
				assert(pbuf != 0);
				if(!write_data_to_disk(pbuf, m_torrent_file.GetPieceLength(piece_idx), piece_idx, 0))
				{
						assert(0);
						return;
				}
				m_write_cache.RemoveCache(piece_idx);
		}
}

bool Storage::WriteData(const t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset)
{
		if(m_write_cache.IsExisted(piece_idx))
		{
				t_byte *pbuf = m_write_cache.GetCache(piece_idx);
				assert(pbuf != 0);
				memcpy(pbuf + offset, buf, len);
				return true;
		}else
		{
				const size_t piece_len = m_torrent_file.GetPieceLength(piece_idx);
				while(m_write_cache.GetLeftCacheSize() < piece_len)
				{
						t_uint32 drop_pidx = NONE_PIECE_INDEX;
						if(!m_write_cache.GetLFUCache(drop_pidx))
						{
								//按理说流程不会走到这里，
								//因为这意味着cache max size比piece_len要小
								assert(0);
								return write_data_to_disk(buf, len, piece_idx, offset);
						}else
						{
								FlushPiece(drop_pidx);
						}
				}
				
				if(!m_write_cache.AllocateCache(piece_idx, piece_len))
				{
						assert(0);
						return false;
				}
				t_byte *new_cache = m_write_cache.GetCache(piece_idx);
				assert(new_cache != 0);
				
				if(!read_data_from_disk(new_cache, piece_len, piece_idx, 0))
				{
						assert(0);
						return false;
				}
				
				memcpy(new_cache + offset, buf, len);
				return true;
		}
}

		
bool Storage::ReadData(t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset)const
{
		if(m_read_cache.IsExisted(piece_idx))
		{
				t_byte *pbuf = m_read_cache.GetCache(piece_idx);
				assert(pbuf != 0);
				memcpy(buf, pbuf + offset, len);
				return true;
		}else
		{
				size_t piece_len = m_torrent_file.GetPieceLength(piece_idx);
				while(m_read_cache.GetLeftCacheSize() < piece_len)
				{
						t_uint32 drop_pidx = NONE_PIECE_INDEX;
						if(!m_read_cache.GetLFUCache(drop_pidx))
						{
								//按理说流程不会走到这里，
								//因为这意味着cache max size比piece_len要小
								assert(0);
								return read_data_from_disk(buf, len, piece_idx, offset);
						}else
						{
								m_read_cache.RemoveCache(drop_pidx);
						}
				}
				
				if(!m_read_cache.AllocateCache(piece_idx, piece_len))
				{
						assert(0);
						return false;
				}

				t_byte *new_cache = m_read_cache.GetCache(piece_idx);
				assert(new_cache != 0);
				
				if(!read_data_from_disk(new_cache, piece_len, piece_idx, 0))
				{
						return false;
				}
				
				memcpy(buf, new_cache + offset, len);
				return true;
		}
}


bool Storage::WritePiece(const t_byte *buf, t_uint32 piece_idx)
{
		assert(IsOpen());
		assert(piece_idx < m_torrent_file.GetPieceCount());
		return WriteData(buf, m_torrent_file.GetPieceLength(piece_idx), piece_idx, 0);

}

bool Storage::ReadPiece(t_byte *buf, t_uint32 piece_idx)const
{
		assert(IsOpen());
		assert(piece_idx < m_torrent_file.GetPieceCount());
		
		return ReadData(buf, m_torrent_file.GetPieceLength(piece_idx), piece_idx, 0);
}

bool Storage::check_piece_for_disk(t_uint32 piece_idx)const
{
		assert(IsOpen() && piece_idx < m_torrent_file.GetPieceCount());
		
		t_uint32 remain_len = m_torrent_file.GetPieceLength(piece_idx);
		size_t offset = 0;
		t_byte buf[SLICE_LENGTH];

		CryptSpace::ShaHash sha(CryptSpace::ShaHash::SHA160);
		sha.Begin();
		while(remain_len > 0)
		{
				t_uint32 read_len = remain_len > SLICE_LENGTH ? SLICE_LENGTH : remain_len;
				
				if(!read_data_from_disk(buf, read_len, piece_idx, offset)) return false;

				sha.Update(buf, read_len);

				remain_len -= read_len;
				offset += read_len;
		}

		
		Sha1Hash hash_val = m_torrent_file.GetPieceHash(piece_idx);
		
		t_byte sha_val[20];

		sha.End(sha_val);

		Sha1Hash res_val(sha_val);

		return hash_val == res_val;
}

bool Storage::CheckPiece(t_uint32 piece_idx)const
{
		t_byte *data = 0;

		if(m_read_cache.IsExisted(piece_idx))
		{
				data = m_read_cache.GetCache(piece_idx);
				assert(data != 0);
		}
		
		if(m_write_cache.IsExisted(piece_idx))
		{
				data = m_write_cache.GetCache(piece_idx);
				assert(data != 0);
		}

		if(data == 0)
		{
				
				return check_piece_for_disk(piece_idx);
		}else
		{
				CryptSpace::ShaHash sha(CryptSpace::ShaHash::SHA160);
				sha.Begin();
				sha.Update(data, m_torrent_file.GetPieceLength(piece_idx));
				t_byte sha_val[20];
				sha.End(sha_val);
				Sha1Hash hash_val = m_torrent_file.GetPieceHash(piece_idx);
				Sha1Hash res_val(sha_val);

				return hash_val == res_val;
		}
}


#if(0)
bool Storage::CheckPiece(t_uint32 piece_idx)const
{
		assert(IsOpen() && piece_idx < m_torrent_file.GetPieceCount());
		Sha1Hash hash_val = m_torrent_file.GetPieceHash(piece_idx);

		std::vector<t_byte> buf(m_torrent_file.GetPieceLength(piece_idx), 0);
		
		if(!ReadPiece(&buf[0],piece_idx)) return false;

		CryptSpace::ShaHash sha(CryptSpace::ShaHash::SHA160);

		sha.Begin();

		sha.Update(&buf[0], buf.size());
		
		t_byte sha_val[20];

		sha.End(sha_val);

		Sha1Hash res_val(sha_val);

		return hash_val == res_val;
}
#endif




#if(0)
t_uint32 Storage::GetPieceLength(t_uint32 piece_idx)const
{
		return m_torrent_file.GetPieceLength(piece_idx);
}
#endif

t_uint64 Storage::GetTotalLength()const
{
		return m_torrent_file.GetTotalSize();
}


bool Storage::Open()
{
		if(IsOpen()) return false;
		
		m_hdl_set.clear();
		
		std::wstring save_path = m_save_path;

		std::wstring top_dir = m_torrent_file.GetTopDir();
		
		if(top_dir != L"")
		{
				save_path += top_dir;
				save_path += RAGE_PATH_SP_W;
		}



		if(PathT(save_path).GetPathType() != FileSysSpace::PATH_DIR)
		{
				return false;
		}
		
	
		PathT path_file;

		for(size_t idx = 0; idx < m_torrent_file.GetFileCount(); ++idx)
		{
				
				FileInfo finfo = m_torrent_file.GetFileInfo(idx);
				std::wstring file_path = save_path + finfo.path;
				
				path_file.Reset(file_path);
				
				if(path_file.GetPathType() != FileSysSpace::PATH_FILE)
				{
						goto OPEN_FAILED;
				}
				

				if(!path_file.IsExisted())
				{
						
						 if(!path_file.Make())
						 {
								 goto OPEN_FAILED;
						 }
				}

				FileHandle *pfhdl = new FileHandle(file_path, finfo.length);
				m_hdl_set.push_back(pfhdl);

				if(!pfhdl->Open())
				{
						goto OPEN_FAILED;
				}
				
				if(!pfhdl->FullAllocate())
				{
						goto OPEN_FAILED;
				}
		}
		
		m_is_open = true;
		return true;

OPEN_FAILED:
		
		for(size_t i = 0; i < m_hdl_set.size(); ++i)
		{
				delete m_hdl_set[i];
		}
		m_hdl_set.clear();
		return false;
}


void Storage::Close()
{
		if(IsOpen())
		{
				for(size_t i = 0; i < m_hdl_set.size(); ++i)
				{
						delete m_hdl_set[i];
				}
				m_hdl_set.clear();
				m_is_open = false;
		}
}


const std::wstring& Storage::GetSavePath()const
{
		return m_save_path;

}



















#if(0)
float Storage::FinishedDegree()const
{
		size_t piece_count = m_bitfield.NBits();
		size_t own_piece_count = 0;
		
		for(size_t i = 0; i < piece_count; ++i)
		{
				if(m_bitfield.IsSet(i))
				{
						own_piece_count++;
				}
		}

		if(own_piece_count == piece_count)
		{
				return 1.0f;
		}else
		{
				return ((float)own_piece_count / (float)piece_count);
		}
}

t_uint64 Storage::FinishedSize()const
{
		size_t piece_count = m_bitfield.NBits();
		t_uint64 fin_len = 0;
		
		for(size_t i = 0; i < piece_count; ++i)
		{
				if(m_bitfield.IsSet(i))
				{
						fin_len += m_torrent_file.GetPieceLength(i);
				}
		}

		return fin_len;
}





void Storage::ResetBitfield(const BitField &bitfield)
{
		if(bitfield.NBits() == m_bitfield.NBits())
		{
				m_bitfield  = bitfield;
		}

}

const BitField& Storage::GetBitfield()const
{
		return m_bitfield;
}


//////////////////////////////////////////////////////////////////////////////////////
bool Storage::IsFinished()const
{
		return m_bitfield.IsAllSet();
}


void Storage::make_request_range()
{
		t_uint32 next_beg = 0;
		bool is_done = false;

		while(!is_done)
		{
				PieceRange pair;
				pair.beg_piece = next_beg;
				pair.end_piece = next_beg + m_torrent_file.GetPieceCount()/4;

				if(pair.end_piece == 0 || pair.end_piece >= m_torrent_file.GetPieceCount())
				{
						pair.end_piece = m_torrent_file.GetPieceCount() - 1;
						is_done = true;
				}

				next_beg = pair.end_piece+1;
				m_range_list.push_back(pair);
		}
}


bool Storage::get_piece_task_in_range(const PieceRange &range, const BitField &peer_own_bitfield, t_uint32 &result_pidx)
{

		for(result_pidx = range.beg_piece; result_pidx <= range.end_piece; ++result_pidx)
		{
				if(peer_own_bitfield.IsSet(result_pidx) && !m_bitfield.IsSet(result_pidx))
				{
						DownloadingList::iterator iter = m_downloading_list.begin();
						for(; iter!= m_downloading_list.end(); ++iter)
						{
								if(*iter == result_pidx)
								{
										break;
								}
						}

						if(iter != m_downloading_list.end())
						{
								continue;
						}

						m_downloading_list.push_back(result_pidx);
						return true;
				}
		}
		return false;
}

bool Storage::GetNewPieceTask(const BitField &peer_haves, t_uint32 &download_piece)
{
		
		for(size_t i=0; i < m_range_list.size(); ++i)
		{
				PieceRange range = m_range_list.front();
				m_range_list.pop_front();
				m_range_list.push_back(range);
				
				if(get_piece_task_in_range(range, peer_haves, download_piece))
				{
						return true;
				}
		}
		
		for(DownloadingList::iterator it = m_downloading_list.begin(); it != m_downloading_list.end(); ++it)
		{
				if(peer_haves.IsSet(*it))
				{
						download_piece = *it;
						m_downloading_list.erase(it);
						m_downloading_list.push_back(download_piece);
						return true;
				}

		}
		return false;
}


bool Storage::remove_downloading_piece(t_uint32 piece_idx)
{
		DownloadingList::iterator it = std::find(m_downloading_list.begin(), m_downloading_list.end(), piece_idx);

		if(it != m_downloading_list.end())
		{
				m_downloading_list.erase(it);
				return true;
		}else
		{
				return false;
		}


}


void Storage::MarkPieceFinished(t_uint32 piece_idx)
{
		assert(piece_idx < m_torrent_file.GetPieceCount());

		if(m_bitfield.IsSet(piece_idx))
		{
				return;
		}

		if(!remove_downloading_piece(piece_idx)) 
		{
				return ;
		}
		
		m_bitfield.Set(piece_idx);
}

#endif

}