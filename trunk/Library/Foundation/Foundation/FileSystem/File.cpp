
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
#include "File.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/FileImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace FileSysSpace {

void* File::GetFileHandle()
{
		return (void*)m_pimpl->get_handle();
}

File::File() : m_pimpl(new FileImpl()) { }

File::~File() 
{ 
		try{
				delete m_pimpl; 
		}catch(const std::exception &expt)
		{
				DEBUG_PRINT1("UnKwon Exception == %s\n", expt.what());

		}
}

bool File::Open(const std::wstring &path, t_uint32 o_mode)
{
		return m_pimpl->Open(path, o_mode);
}


bool File::Open(const std::string &path, t_uint32 o_mode)
{
		return m_pimpl->Open(path, o_mode);
}


void File::Close()
{
		m_pimpl->Close();
}

bool File::IsOpen()const
{
		return m_pimpl->IsOpen();
}

void File::Flush()
{
		m_pimpl->Flush();
}

size_t File::Write(const t_byte *buf, size_t len)
{
		return m_pimpl->Write(buf, len);
}

size_t File::Read(t_byte *buf, size_t len)
{
		return m_pimpl->Read(buf, len);
}


t_uint64 File::Seek(const t_uint64 &offset, SeekPosition method, bool forward)
{
		return m_pimpl->Seek(offset, method, forward);
}

t_uint64 File::SeekToEnd()
{
		return m_pimpl->SeekToEnd();
}

t_uint64 File::SeekToBeg()
{
		return m_pimpl->SeekToBeg();
}

void File::SetLength(const t_uint64 &newlen)
{
		m_pimpl->SetLength(newlen);
}

t_uint64 File::GetLength() const
{
		return m_pimpl->GetLength();
}

t_uint64 File::GetPos()const
{
		return m_pimpl->GetPos();
}

void File::LockRange(t_uint64 pos, t_uint64 length)
{
		m_pimpl->LockRange(pos, length);
}

void File::UnlockRange(t_uint64 pos, t_uint64 length)
{
		m_pimpl->UnlockRange(pos, length);
}


bool File::GetFileName(std::string &name)
{
		return m_pimpl->GetFileName(name);
}

bool File::GetFileName(std::wstring &name)
{
		return m_pimpl->GetFileName(name);
}

bool File::GetFilePath(std::string &path)
{
		return m_pimpl->GetFilePath(path);
}

bool File::GetFilePath(std::wstring &path)
{
		return m_pimpl->GetFilePath(path);
}

bool File::FileCopyTo(std::wstring sour, std::wstring dest)
{
		return FileImpl::FileCopyTo(sour, dest);
}

}






/*
bool File::Open(const std::string &path, t_ulong desired_access, int op_type, t_ulong share_mode)
{
		return m_pimpl->Open(path, desired_access, op_type, share_mode);
}

bool File::Open(const std::wstring &path, t_ulong desired_access, int op_type, t_ulong share_mode)
{
		return m_pimpl->Open(path, desired_access, op_type, share_mode);
}*/