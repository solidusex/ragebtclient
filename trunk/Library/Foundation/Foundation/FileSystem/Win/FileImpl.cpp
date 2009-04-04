
/*
 * The Foundation Library
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
#include "FileImpl.h"
#include "../TypeDef.h"
#include "../../Format/FormatHdr.h"


namespace FileSysSpace {

#define CHECK_CONDITION( condition, msg_str)											\
		if(!(condition)) {																\
				throw ExceptionSpace::FileException(msg_str, ::GetLastError());			\
		}




FileImpl::FileImpl() : m_fhdl(INVALID_HANDLE_VALUE),  m_fname(L"") { }
		
FileImpl::~FileImpl() 
{
		try{
				Close(); 
		}catch(const std::exception &expt)
		{
				DEBUG_PRINT1("Unknow Exception == %s\n", expt.what());
				assert(0);
		}
}



bool FileImpl::Open(const std::wstring &path, t_ulong desired_access, int op_type, t_ulong share_mode)
{
		assert(m_fhdl ==INVALID_HANDLE_VALUE);

		if(m_fhdl != INVALID_HANDLE_VALUE) return false;
		

		HANDLE fhdl = CreateFileW(	path.c_str(), 
									desired_access,
									share_mode,
									0,op_type,
									0,0
								);
		if(fhdl == INVALID_HANDLE_VALUE) 
		{
				return false;
		}else
		{
				m_fhdl = fhdl;
				m_fname = path;
				return true;
		}
}
		
void FileImpl::Close()
{
		if(IsOpen())
		{
				::CloseHandle(m_fhdl);
				m_fhdl = INVALID_HANDLE_VALUE;
				m_fname = L"";
		}
}
		
bool FileImpl::IsOpen()const
{
		return (m_fhdl != INVALID_HANDLE_VALUE);
}

void FileImpl::Flush()
{
		if(!IsOpen()) return;
		
		CHECK_CONDITION( ::FlushFileBuffers(m_fhdl) == TRUE, "FileImpl::Flush()");
}

size_t FileImpl::Write(const t_byte *buf, size_t len)
{
		if(!IsOpen()) return 0;
		
		t_ulong nwritten = 0;

		CHECK_CONDITION
		(
		::WriteFile(m_fhdl, buf, (DWORD)len, &nwritten, NULL), 
		"FileImpl::Write "
		);
		
		CHECK_CONDITION((nwritten == len), "FileImpl::Write ");
		return len;
		
}

size_t FileImpl::Read(t_byte *buf, size_t len)
{
		if(!IsOpen()) return 0;
		
		t_ulong read_num = 0;

		CHECK_CONDITION
		(
		(::ReadFile(m_fhdl, buf, (DWORD)len, &read_num, NULL)),
		"FileImpl::Read"
		);
		return read_num;
}
		
t_uint64 FileImpl::GetPos()const
{
		assert(IsOpen());
		if(!IsOpen()) return 0;
		LARGE_INTEGER larger_num;
		LARGE_INTEGER beg;
		memset((void*)&beg, 0, sizeof(LARGE_INTEGER));
		bool state = (::SetFilePointerEx( m_fhdl, beg, &larger_num, FILE_CURRENT) != FALSE);
				
		CHECK_CONDITION(state, "FileImpl::GetPos()");
				
		return larger_num.QuadPart;
}

t_uint64 FileImpl::GetLength() const
{
	    assert(IsOpen());
		if(!IsOpen()) return 0;
				
		LARGE_INTEGER larger_num;

		CHECK_CONDITION( ::GetFileSizeEx(m_fhdl, &larger_num), "FileImpl::GetLength()" );
				

		return larger_num.QuadPart;
						
}
						
	

t_uint64 FileImpl::Seek(const t_uint64 &offset, SeekPosition method, bool forward)
{
		if(!IsOpen()) return 0;
		t_uint64 new_pos = 0;

		switch (method)
		{
		case CURR:
		{
				if (forward){
						new_pos = GetPos() + offset;
				}else {
						new_pos = GetPos() - offset;
				}
				break;
		}
		case END:     
		{
				if (forward)
						new_pos = GetLength() + offset; 
				else
						new_pos = GetLength() - offset;
				break;
		}
		default:      
		//case BEG:   
		{
				new_pos = offset;
				break;
		}
		}
		
		::LARGE_INTEGER new_fp;
		::LARGE_INTEGER pos;
		pos.QuadPart = new_pos;
		CHECK_CONDITION( ::SetFilePointerEx(m_fhdl, pos, &new_fp, FILE_BEGIN), "FileImpl::Seek");
		return new_fp.QuadPart;
}


t_uint64 FileImpl::SeekToEnd()
{
		return Seek(0, END, false);
}

t_uint64 FileImpl::SeekToBeg()
{
		return Seek(0, BEG, false);
}

void FileImpl::SetLength(const t_uint64 &newlen)
{
		assert(IsOpen());
		Seek(newlen, BEG, true);
		CHECK_CONDITION(::SetEndOfFile(m_fhdl) , "FileImpl::SetLength");
}
		

void FileImpl::LockRange(t_uint64 pos, t_uint64 length)
{
		assert(IsOpen());
		if(IsOpen()){
				t_ulong pos_low, pos_high;
				SPLIT_INT64(pos, pos_high, pos_low);

				t_ulong length_low, length_high;
				SPLIT_INT64(length, length_high, length_low);
				
				
				CHECK_CONDITION
				(
				::LockFile(m_fhdl, pos_low, pos_high, length_low, length_high),
				"FileImpl::LockRange"
				);
		}

}

void FileImpl::UnlockRange(t_uint64 pos, t_uint64 length)
{
		if(IsOpen()){
				t_ulong pos_low, pos_high;
				SPLIT_INT64(pos, pos_high, pos_low);

				t_ulong length_low, length_high;
				SPLIT_INT64(length, length_high, length_low);

				CHECK_CONDITION
				(
				::UnlockFile(m_fhdl, pos_low, pos_high, length_low, length_high),
				" FileImpl::UnlockRange"
				);
		}
}


bool FileImpl::GetFileName(std::string &name)
{
		std::wstring tmp;
		bool state = GetFileName(tmp);
		name = FormatSpace::UnicodeConverter::ToAnsi(tmp);
		return state;
}


bool FileImpl::GetFilePath(std::string &path)
{
		std::wstring tmp;
		bool state = GetFilePath(tmp);
		path = FormatSpace::UnicodeConverter::ToAnsi(tmp);

		return state;
}

		
bool FileImpl::GetFileName(std::wstring &name)
{
		if(!IsOpen()) return false;
		name= m_fname.substr(m_fname.find_last_of(L'\\') + 1);
		return true;
}
		


bool FileImpl::GetFilePath(std::wstring &path)
{
		if(!IsOpen()) return false;
		size_t pos = (m_fname.size() - 1);
		while(m_fname[pos] != L'\\') --pos;
		path = m_fname.substr(0, ++pos);
		return true;
}

bool FileImpl::FileCopyTo(std::wstring sour, std::wstring dest)
{
		/*Path path(dest);
		if(path.IsExisted())
		{
				if(!path.Remove()) return false;
		}*/
		assert(sour != L"");
		assert(dest != L"");
		assert(sour != dest);
		return (::CopyFileW(sour.c_str(), dest.c_str(), FALSE) == TRUE);

}



bool FileImpl::Open(const std::string &path, t_uint32 o_mode)
{
		std::wstring wstr(FormatSpace::AnsiConverter::ToUnicode(path));
		return Open(wstr, o_mode);
}

bool FileImpl::Open(const std::wstring &path, t_uint32 o_mode)
{
		t_ulong desired_access = 0;
		int op_type = 0;
		t_ulong share_mode = 0;

		if(o_mode & F_READ_ONLY)
		{
				desired_access = GENERIC_READ;
		}else if(o_mode &F_WRITE_ONLY)
		{
				desired_access = GENERIC_WRITE;
		}
		else if(o_mode & F_READ_WRITE)
		{
				desired_access = GENERIC_WRITE|GENERIC_READ;
		}else
		{
				assert(0);
				goto FAILED;
		}

		if(o_mode & F_CREATE_NEW)
		{
				op_type = CREATE_NEW;
		}else if(o_mode & F_CREATE_ALWAYS)
		{
				op_type = CREATE_ALWAYS;
		
		}else if(o_mode & F_OPEN_EXISTED)
		{
				op_type = OPEN_EXISTING;
		
		}else if( o_mode & F_OPEN_ALWAYS)
		{
				op_type = OPEN_ALWAYS;
		
		}else if(o_mode & F_TRUNC)
		{
				op_type = TRUNCATE_EXISTING;
		}else
		{
				assert(0);
				goto FAILED;
		}

		if(o_mode & F_SHARED_READ)
		{
				share_mode = FILE_SHARE_READ;
		
		}else if(o_mode & F_SHARED_WRITE)
		{
				share_mode = FILE_SHARE_WRITE;
		
		}else if(o_mode & F_SHARED_READ_WRITE)
		{
				share_mode = FILE_SHARE_WRITE|FILE_SHARE_READ;
		
		}else if(o_mode & F_NOSHARED)
		{
				share_mode = 0;
		}else
		{
				assert(0);
				goto FAILED;
		}
				
		return Open(path, desired_access, op_type, share_mode);

FAILED:
		throw ExceptionSpace::InvalidArgumentException("open mode invalid");


}

#undef CHECK_CONDITION
				
//#undef BUF_CACHE

}

