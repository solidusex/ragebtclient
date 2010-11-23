
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

#include "TypeDef.h"
#include "File.h"
namespace FileSysSpace{

class MemMapFile;

class FoundationAPI MemMapFileView : NonCopyable
{
private:
		friend class MemMapFile;
private:
		t_byte			*m_base_addr;	//ʵ�ʴ򿪵ĵ�ַ
		t_byte			*m_mapped_addr;	//������ӳ��ĵ�ַ
		size_t			m_mapped_len;	//����ӳ��ĳ���
private:
		MemMapFileView(t_byte *baddr, t_byte *mapped_addr, size_t mapped_len); 
		~MemMapFileView();
public:
		t_byte* GetAddr();
		size_t GetLength()const;
};




class MemMapFileImpl;

class FoundationAPI MemMapFile : NonCopyable
{
private:
		MemMapFileImpl	*m_pimpl;
public:
		MemMapFile();
		
		~MemMapFile();
public:
		bool Open(void* fhandle, t_uint64 fsize, t_ulong mode);
		
		void Close();

		bool IsOpen()const;
		
		//mapview�ĳ��Ȳ��ܴ���offset+len�ļ����ȣ���������δ֪����
		MemMapFileView* MapViewOfFile(t_uint64 offset, size_t len, t_ulong mode);

		void UnmapViewOfFile(MemMapFileView* pmv);

		bool Flush(MemMapFileView* pmv);
};



}