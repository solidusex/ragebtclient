#include "FileHandle.h"



namespace PiggyFly {


using namespace FileSysSpace;

#define CHECK_AND_THROW(cond, msg, err_code)\
		do{									\
				if(!cond)					\
				{							\
						assert(0 );			\
						throw StorageException(msg, err_code);\
				}							\
		}while(0)
				


bool FileHandle::IsOpen()const
{
		return m_file.IsOpen();
}

OpenMode FileHandle::GetOpenMode()const
{
		assert(m_file.IsOpen());
		return m_open_mode;

}

bool FileHandle::IsExisted()const
{
		assert(m_path.GetPathType() == PATH_FILE);
		return m_path.IsExisted();
}

bool FileHandle::Make()const
{
		assert(m_path.GetPathType() == PATH_FILE);
		return const_cast<FileHandle*>(this)->m_path.Make();
}

std::wstring FileHandle::GetFullPath()const
{
		std::wstring f_path;

		m_path.GetPath(f_path);
		return f_path;
}

size_t FileHandle::Read(t_byte *buf, size_t len, t_uint64 offset)
{
		assert(buf != 0);
		assert(len != 0);
		assert(m_file.IsOpen());
		
		
		assert((offset + len) <= GetRealLength());
		
		t_uint64 pos = m_file.Seek(offset, FileSysSpace::BEG, true);
		assert(pos == offset);

		t_uint64 rn = m_file.Read(buf, len);

		assert(rn == len);
		
		return rn;
}


size_t FileHandle::Write(const t_byte *buf, size_t len, t_uint64 offset)
{

		assert(m_open_mode == READ_WRITE);
		assert(buf != 0);
		assert(len != 0);
		assert(m_file.IsOpen());
		

		assert((offset + len) <= GetRealLength());
		
		t_uint64 pos = m_file.Seek(offset, FileSysSpace::BEG, true);
		assert(pos == offset);
		t_uint64 wn = m_file.Write(buf, len);
		assert(wn == len);
		
		return wn;

}

FileHandle::FileHandle(const std::wstring &path, t_uint64 need_length)
{
		m_path.Reset(path);
		
		bool cond = m_path.GetPathType() == FileSysSpace::PATH_FILE;
		
		CHECK_AND_THROW(cond, "invalid path name ", INVALID_PATH_NAME);
		m_need_length = need_length;
}

FileHandle::~FileHandle()
{
		Close();
}


t_uint64 FileHandle::GetRealLength()const
{
		assert(m_file.IsOpen());
		return m_file.GetLength();
}
		
t_uint64 FileHandle::GetNeedLength()const
{
		return m_need_length;
}

bool FileHandle::Open(OpenMode mode)
{
		assert(!IsOpen());
		assert(m_path.IsExisted());
		assert(m_path.GetPathType() == PATH_FILE);
		
		std::wstring full_path = L"";
		m_path.GetPath(full_path);

		t_uint32 open_mode = 0;
		t_uint32 map_file_open_mode = 0;

		if(mode == READ_ONLY)
		{
				open_mode = F_READ_ONLY|F_OPEN_EXISTED|F_SHARED_READ;
		}else
		{
				open_mode = F_READ_WRITE|F_OPEN_EXISTED|F_SHARED_READ;
		}
		
		m_open_mode = mode;

		return m_file.Open(full_path, open_mode);
}

void FileHandle::Close()
{
		if(m_file.IsOpen())
		{
				m_file.Close();
		}
}

bool FileHandle::IncreaseFileSize(t_uint64 size)
{
		try{
				assert(m_file.IsOpen());
				assert(m_open_mode == READ_WRITE);
				t_uint64 new_len = m_file.GetLength() + size;
				
				assert(new_len <= m_need_length);

				if(new_len > m_need_length)
				{
						new_len = m_need_length;
				}

				m_file.SetLength(new_len);
		
		}catch(const FileSysSpace::FileSysException & fexpt)
		{
				DEBUG_PRINT1("%s\n", fexpt.What().msg.c_str());
				return false;

		}
		return true;
}

bool FileHandle::TruncateFileSize(t_uint64  size)
{
		try{
				assert(m_file.IsOpen());
				assert(m_open_mode == READ_WRITE);
				
				t_uint64 new_len = 0;

				if(m_file.GetLength() > size)
				{
						new_len = m_file.GetLength() - size;
				}

				m_file.SetLength(new_len);
		
		
		}catch(const FileSysSpace::FileSysException & fexpt)
		{
				DEBUG_PRINT1("%s\n", fexpt.What().msg.c_str());
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




#undef CHECK_AND_THROW












}