#pragma once


#include "TypeDef.h"





namespace PiggyFly {



class FileHandle : NonCopyable
{
private:
		FileSysSpace::File			m_file;
		FileSysSpace::Path			m_path;
		t_uint64					m_need_length;	//被torrent文件指定的实际长度，相对于当前的长度而言
		OpenMode					m_open_mode;
public:
		FileHandle(const std::wstring &path, t_uint64 need_length);

		~FileHandle();
public:
		bool Open(OpenMode mode);

		void Close();

		//从当前长度增加size个字节,如果当前size + size超过m_need_length会抛出异常
		//如果增加失败会会返回false
		bool IncreaseFileSize(t_uint64 size);

		bool TruncateFileSize(t_uint64  size);

		size_t Read(t_byte *buf, size_t len, t_uint64 offset);
		
		size_t Write(const t_byte *buf, size_t len, t_uint64 offset);

		t_uint64 GetRealLength()const;
		
		t_uint64 GetNeedLength()const;

		std::wstring GetFullPath()const;

		bool IsExisted()const;

		bool IsOpen()const;

		bool Make()const;

		OpenMode GetOpenMode()const;

		bool FullAllocate();
};


















}