#include "Storage.h"

#include<sstream>

namespace PiggyFly {

using namespace FileSysSpace;



#define CHECK_AND_THROW(cond, msg, err_code)\
		do{									\
				if(!cond)					\
				{							\
						assert(0);			\
						throw StorageException(msg, err_code);\
				}							\
		}while(0)


Storage::Storage(t_uint32 piece_len) : m_file_pieces_map(piece_len)
{
		

}

Storage::~Storage()
{


}

bool Storage::SetFileOpenMode(size_t file_idx, OpenMode mode)
{
		assert(IsOpen());
		assert(file_idx  < m_file_set.size());
		assert(m_file_set[file_idx] != 0);
		assert(m_file_set[file_idx]->IsOpen());

		m_file_set[file_idx]->Close();
		return m_file_set[file_idx]->Open(mode);
}

void Storage::Close()
{
		for(size_t idx = 0; idx < m_file_set.size(); ++idx)
		{
				if(m_file_set[idx] != 0)
				{
						m_file_set[idx]->Close();
						delete m_file_set[idx];
				}
		}

		m_file_set.clear();
		m_file_pieces_map.Clear();
}

bool Storage::IsOpen()const
{
		return !m_file_set.empty();
}

bool Storage::FullAllocate()
{
		assert(IsOpen());

		for(size_t idx = 0; idx < m_file_set.size(); ++idx)
		{
				if(!m_file_set[idx]->FullAllocate())
				{
						return false;
				}
		}

		return true;
}


bool Storage::Open(const std::wstring &save_path, const FileList &files, OpenMode mode)
{
		assert(!IsOpen() && !files.empty());
		
		Path path(save_path);

		m_file_set = std::vector<FileHandle*>(files.size(), 0);

		if((path.GetPathType() == PATH_FILE) || (path.GetPathType() == INVALID_PATH))
		{
				return false;
		}
		
		
		for(FileList::const_iterator it = files.begin(); it != files.end(); ++it)
		{
				std::wstring tmp_path = save_path;
				tmp_path += Path::PATH_SP_W;
				tmp_path += it->relative_path;

				path.Reset(tmp_path);
				if(path.GetPathType() != PATH_FILE) return false;

				m_file_pieces_map.AddFile(tmp_path, it->length, it->is_filtered);
				
				FileHandle *pfh = new FileHandle(tmp_path, it->length);
				m_file_set.push_back(pfh);
		}
		
		if(mode == READ_ONLY)
		{
				for(size_t idx = 0; idx < m_file_set.size(); ++idx)
				{
						if(!m_file_set[idx]->IsExisted())
						{
								return false;
						}else
						{
								if(!m_file_set[idx]->Open(mode)) return false;
						}
				}
		}else
		{
				for(size_t idx = 0; idx < m_file_set.size(); ++idx)
				{
						bool is_filtered = m_file_pieces_map.GetFileByIdx(idx)->is_filtered;

						if(!is_filtered)
						{
								if(!m_file_set[idx]->IsExisted())
								{
										 m_file_set[idx]->Make();
								}

								if(m_file_set[idx]->Open(mode))
								{
										if(m_file_set[idx]->GetRealLength() > m_file_set[idx]->GetNeedLength())
										{
												t_uint64 trunc_size = m_file_set[idx]->GetRealLength() - m_file_set[idx]->GetRealLength();
												m_file_set[idx]->TruncateFileSize(trunc_size);
										}
								}else
								{
										return false;
								}
						}else
						{
								m_file_set[idx] = 0;
						}
				}
		}
		
		return true;
}


bool Storage::WasPreAllocated(size_t piece_idx)const
{
		std::vector<FilePos>	vfs = m_file_pieces_map.MapFragToFiles(piece_idx);

		assert(!vfs.empty());

		for(size_t idx = 0; idx < vfs.size(); ++idx)
		{
				size_t file_idx = vfs[idx].file_idx;

				if(m_file_pieces_map.GetFileByIdx(file_idx)->is_filtered)
				{
						continue;
				}else
				{
						t_uint64 need_len = vfs[idx].start_pos + vfs[idx].len;
						if( need_len > m_file_set[file_idx]->GetRealLength())
						{
								return false;
						}
				}
		}
		return true;
}

bool Storage::AllocatePiece(size_t piece_idx)const
{
		std::vector<FilePos>	vfs = m_file_pieces_map.MapFragToFiles(piece_idx);

		assert(!vfs.empty());
		
		for(size_t idx = 0; idx < vfs.size(); ++idx)
		{
				size_t file_idx = vfs[idx].file_idx;
				
				if(m_file_pieces_map.GetFileByIdx(file_idx)->is_filtered)
				{
						continue;
				}else
				{
						t_uint64 need_len = vfs[idx].start_pos + vfs[idx].len;
						t_uint64 real_len = m_file_set[file_idx]->GetRealLength();
						
						assert(m_file_set[file_idx]->GetOpenMode() == READ_WRITE);

						if(real_len < need_len)
						{
								if(!m_file_set[file_idx]->IncreaseFileSize(need_len - real_len))
								{
										return false;
								}
						}
				}
		}
		
		return true;
		
}

size_t Storage::Write(const t_byte *buf, size_t len, size_t piece_idx, size_t offset)
{
		assert(WasPreAllocated(piece_idx));
		
		std::vector<FilePos>	vfs = m_file_pieces_map.MapFragToFiles(piece_idx, offset);
		assert(!vfs.empty());
		
		assert(buf != 0);
		assert(len != 0);
		assert(piece_idx < m_file_pieces_map.FragsCount());

		assert((offset + len) <= m_file_pieces_map.FragLength(piece_idx));

		const t_byte *pbuf = buf;
		
		t_uint64 remain_len = len;

		for(size_t idx = 0; idx < vfs.size(); ++idx)
		{
				size_t file_idx = vfs[idx].file_idx;
				
				if(m_file_pieces_map.GetFileByIdx(file_idx)->is_filtered)
				{
						continue;
				}else
				{
						if(vfs[idx].start_pos + vfs[idx].len	> m_file_set[file_idx]->GetRealLength())
						{
								return false;
						}
						t_uint64 this_len =  std::min<t_uint64>(remain_len, vfs[idx].len);
				
						if(this_len != 0)
						{
								size_t rn = m_file_set[file_idx]->Write(pbuf, this_len,	vfs[idx].start_pos);

								remain_len -= this_len;
								assert(rn == this_len);
								pbuf += rn;
						}
		}
		assert(remain_len == 0);
		return len;
}

size_t Storage::Read(t_byte *buf, size_t len, size_t piece_idx, size_t offset)const
{
		assert(WasPreAllocated(piece_idx));

		std::vector<FilePos>	vfs = m_file_pieces_map.MapFragToFiles(piece_idx, offset);
		assert(!vfs.empty());
		
		assert(buf != 0);
		assert(len != 0);
		assert(piece_idx < m_file_pieces_map.FragsCount());

		assert((offset + len) <= m_file_pieces_map.FragLength(piece_idx));

		t_byte *pbuf = buf;
		
		t_uint64 remain_len = len;

		for(size_t idx = 0; idx < vfs.size(); ++idx)
		{
				size_t file_idx = vfs[idx].file_idx;
				
				CHECK_AND_THROW((vfs[idx].start_pos + vfs[idx].len	<= m_file_set[file_idx]->GetRealLength()),
								"logic error this pieces not exist",
								READ_PIECE_FAILED
								);
				
				
				t_uint64 this_len =  std::min<t_uint64>(remain_len, vfs[idx].len);
				
				if(this_len != 0)
				{
						size_t rn = m_file_set[file_idx]->Read(pbuf, this_len,	vfs[idx].start_pos);

						remain_len -= this_len;
						assert(rn == this_len);
						pbuf += rn;
				}
		}
		assert(remain_len == 0);
		return len;
}



ReverseBitSet Storage::GetActivePieces()const
{
		assert(IsOpen());

		return m_file_pieces_map.CalcAvailableFrags();

}





}