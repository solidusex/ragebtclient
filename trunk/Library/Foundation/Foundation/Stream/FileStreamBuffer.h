
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

#include "ByteStream.h"
#include "../FileSystem/File.h"
#include <string>
namespace StreamSpace {


enum SEEK_POS { F_BEG = 0, F_CUR = 1, F_END = 2 };


class FoundationAPI DefaultFileOPImpl
{
private:
		FileSysSpace::File		m_file;
public:
		DefaultFileOPImpl();
		~DefaultFileOPImpl();
		
		void Write(const t_byte *arr, size_t len);
		
		size_t Read(t_byte *arr, size_t len);

		t_uint64 Seek(const t_uint64 &offset, SEEK_POS method, bool forward);

		t_uint64  Available()const;

		t_uint64 GetLength()const;
		
		void Flush();
		
		t_uint64 GetPos()const;

		bool Open(const std::string &fname);
		
		bool Open(const std::wstring &fname);
		
		void Close();
};



template<class FileOPImpl>
class FileStreamBuffer : public AbstractByteStreamBuffer, private NonCopyable
{
private:
		FileOPImpl		m_file;
public:
		FileStreamBuffer();
		virtual ~FileStreamBuffer();
public:
		bool Open(std::string &fname);
		bool Open(std::wstring &fname); //一定为独占 rw模式;
		
		void Close();

		t_uint64 Seek(const t_uint64 &offset, SEEK_POS method, bool forward);
		
		void SeekToEnd();

		void SeekToBeg();
public:
		virtual void Put(const t_byte *arr, size_t len);
		
		virtual size_t Get(t_byte *arr, size_t n);
		
		virtual t_uint64  Available()const;

		t_uint64 GetPos()const;

		t_uint64 GetLength()const;

		void Flush();
public:
		virtual void Put(const t_byte &b);
		
		virtual void Get(t_byte &b);
		
		virtual void Clear();

public:					//一下函数为空实现;
		virtual const t_byte* GetContent()const;

		virtual size_t Skip(size_t n);
};


template<class FileImpl>
t_uint64 FileStreamBuffer<FileImpl>::GetLength()const
{
		return m_file.GetLength();
}

template<class FileImpl>
t_uint64 FileStreamBuffer<FileImpl>::GetPos()const
{
		return m_file.GetPos();
}

template<class FileImpl>
bool FileStreamBuffer<FileImpl>::Open(std::string &fname)
{
		return m_file.Open(fname);
}

template<class FileImpl>
bool FileStreamBuffer<FileImpl>::Open(std::wstring &fname) //一定为独占 rw模式;
{
		return m_file.Open(fname);
}

template<class FileImpl>
void FileStreamBuffer<FileImpl>::Close()
{
		m_file.Close();
}

template<class FileImpl>
t_uint64 FileStreamBuffer<FileImpl>::Seek(const t_uint64 &offset, SEEK_POS method, bool forward)
{
		return m_file.Seek(offset, method, forward);
}

template<class FileImpl>
void FileStreamBuffer<FileImpl>::SeekToEnd()
{
		Seek(0, F_END, true);
}

template<class FileImpl>
void FileStreamBuffer<FileImpl>::SeekToBeg()
{
		Seek(0, F_BEG, true);
}
template<class FileImpl>
void FileStreamBuffer<FileImpl>::Put(const t_byte *arr, size_t len)
{
		m_file.Write(arr, len);
}

template<class FileImpl>
size_t FileStreamBuffer<FileImpl>::Get(t_byte *arr, size_t n)
{
		return m_file.Read(arr, n);
}

template<class FileImpl>
t_uint64  FileStreamBuffer<FileImpl>::Available()const
{
		return m_file.Available();
}

template<class FileImpl>
void FileStreamBuffer<FileImpl>::Put(const t_byte &b)
{
		m_file.Write(&b, 1);
}

template<class FileImpl>
void FileStreamBuffer<FileImpl>::Get(t_byte &b)
{
		m_file.Read(&b, 1);
}

template<class FileImpl>
void FileStreamBuffer<FileImpl>::Clear()
{
		Close();
}

template<class FileImpl>
const t_byte* FileStreamBuffer<FileImpl>::GetContent()const
{
		return 0;
}

template<class FileImpl>
size_t FileStreamBuffer<FileImpl>::Skip(size_t n)
{
		return 0;
}

template<class FileImpl>
FileStreamBuffer<FileImpl>::FileStreamBuffer()
{

}

template<class FileImpl>
FileStreamBuffer<FileImpl>::~FileStreamBuffer()
{
		
}

template<class FileImpl>
void FileStreamBuffer<FileImpl>::Flush()
{
		m_file.Flush();

}


typedef FileStreamBuffer<DefaultFileOPImpl> DefaultFileStreamBuffer;


}




/*
class FoundationAPI AbstractFileAdapter	 : private NonCopyable
{
public:
		virtual bool Open(const std::string &fname) = 0;
		
		virtual bool Open(const std::wstring &fname) = 0;
		
		virtual void Close() = 0;

		virtual void Write(const t_byte *arr, t_ulong len) = 0;
		
		virtual t_ulong Read(t_byte *arr, t_ulong len) = 0;

		virtual t_uint64 Seek(const t_uint64 &offset, SEEK_POS method, bool forward) = 0;

		virtual t_uint64  Available()const = 0;//可读取的剩余
		
		virtual t_uint64 GetPos()const = 0;

		virtual t_uint64 GetLength()const = 0;

		virtual void Flush() = 0;

		virtual ~AbstractFileAdapter() = 0
		{
				
		}

};*/