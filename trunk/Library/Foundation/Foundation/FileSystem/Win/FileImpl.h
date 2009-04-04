
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
#pragma once


#include "../TypeDef.h"
#include <string>
#include <cassert>
#include <windows.h>

namespace FileSysSpace {


class FileImpl
{
private:
		HANDLE			m_fhdl;
		std::wstring	m_fname;
public:
		void* get_handle()const { return (void*)m_fhdl; }
public:
		FileImpl(); 
		~FileImpl();
public:
		void Close();
		
		bool IsOpen()const;
		
		size_t Write(const t_byte *buf, size_t len);
		size_t Read(t_byte *buf, size_t len);
public:
		t_uint64 GetPos()const;
		void Flush();
		t_uint64 GetLength() const;
		t_uint64 Seek(const t_uint64 &offset, SeekPosition method, bool forward);
		t_uint64 SeekToEnd();
		t_uint64 SeekToBeg();
		void SetLength(const t_uint64 &newlen);
public:
		void LockRange(t_uint64 pos, t_uint64 length);
		void UnlockRange(t_uint64 pos, t_uint64 length);
public:
		bool GetFileName(std::string &name);
		bool GetFileName(std::wstring &name);
		bool GetFilePath(std::string &path);
		bool GetFilePath(std::wstring &path);
public:
		static bool FileCopyTo(std::wstring sour, std::wstring dest);

private:
		bool Open(const std::wstring &path, t_ulong desired_access, int op_type, t_ulong share_mode);
public:
		bool Open(const std::wstring &path, t_uint32 o_mode);

		bool Open(const std::string &path, t_uint32 o_mode);
};												



}


/*
private:
		void flush();
		t_ulong get_file_type();								
		void set_bufsize(t_ulong size);
		bool set_file_name(const std::wstring &name);
		bool set_file_name(const std::string &name);			//*/