#pragma once
#include "TypeDef.h"
#include "FileHandle.h"


namespace PiggyFly {



class Storage : NonCopyable
{
private:
		typedef FileSysSpace::Fragmentizer				FileToPieceMap;
		typedef FileToPieceMap::FileRecord				FileRecord;
		typedef FileSysSpace::Fragmentizer::FilePos		FilePos;
private:
		FileToPieceMap					m_file_pieces_map;
		std::vector<FileHandle*>		m_file_set;
public:
		typedef std::list<FilesInfo>	FileList;
public:

		Storage(t_uint32 piece_len);

		~Storage();

		bool Open(const std::wstring &save_path, const FileList &files, OpenMode mode);
		
		void Close();

		bool FullAllocate();

		bool IsOpen()const;
public:
		bool WasPreAllocated(size_t piece_idx)const;

		bool AllocatePiece(size_t piece_idx)const;

		size_t Read(t_byte *buf, size_t len, size_t piece_idx, size_t offset)const;

		size_t Write(const t_byte *buf, size_t len, size_t piece_idx, size_t offset);
public:
		bool SetFileOpenMode(size_t file_idx, OpenMode mode);

		ReverseBitSet GetActivePieces()const;

		
};












}