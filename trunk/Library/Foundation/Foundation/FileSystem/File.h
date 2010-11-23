
/*
 * The Foundation Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  It is provided "as is" without express
 * or implied warranty.
 */
#pragma once 

#include "../Common/ComnHdr.h"
#include "TypeDef.h"
#include <string>

namespace FileSysSpace {

class FileImpl;


class  FoundationAPI  File : private NonCopyable
{
private:
		FileImpl		*m_pimpl;
public:
		File();
		~File();
public:
		bool Open(const std::wstring &path, t_uint32 o_mode);
		bool Open(const std::string &path, t_uint32 o_mode);
		void Close();

		bool IsOpen()const;
public:
		size_t Write(const t_byte *buf, size_t len);
		size_t Read(t_byte *buf, size_t len);
		void Flush();
public:
		t_uint64 Seek(const t_uint64 &offset, SeekPosition method, bool forward);
		t_uint64 SeekToEnd();
		t_uint64 SeekToBeg();
		void SetLength(const t_uint64 &newlen);
		t_uint64 GetLength() const;
		t_uint64 GetPos()const;
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
public:
		void* GetFileHandle();
};


}


/*
private:
		t_ulong get_file_type();
		bool set_file_name(const std::wstring &name);
		bool set_file_name(const std::string &name);


		
		
		bool Open(const std::string &path, t_ulong desired_access, int op_type, t_ulong share_mode);
		bool Open(const std::wstring &path, t_ulong desired_access, int op_type, t_ulong share_mode);
		
*/



