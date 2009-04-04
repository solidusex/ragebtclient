#pragma once


#include "TypeDef.h"





namespace PiggyFly {



class FileHandle : NonCopyable
{
private:
		FileSysSpace::File			m_file;
		FileSysSpace::Path			m_path;
		t_uint64					m_need_length;	//��torrent�ļ�ָ����ʵ�ʳ��ȣ�����ڵ�ǰ�ĳ��ȶ���
		OpenMode					m_open_mode;
public:
		FileHandle(const std::wstring &path, t_uint64 need_length);

		~FileHandle();
public:
		bool Open(OpenMode mode);

		void Close();

		//�ӵ�ǰ��������size���ֽ�,�����ǰsize + size����m_need_length���׳��쳣
		//�������ʧ�ܻ�᷵��false
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