#pragma once

#include "Comn.h"

namespace PiggyFly {

#define INVALID_PATH_NAME				0x10000

#define INVALID_SAVE_PATH_ENTRY			0x10003
#define INVALID_FILE_PATH_ENTRY			0x10004


#define INCREASE_FILE_FAILED			0x20003
#define TRUNCATE_FILE_FAILED			0x20004
#define READ_PIECE_FAILED				0x20005
#define WRITE_PIECE_FAILED				0X20006


#define OPEN_FILE_ERROR					0x30000
#define CREATE_SAVE_PATH_ERROR			0x30001
#define CREATE_FILE_ERROR				0x30004
#define READ_FILE_ERROR					0x30005
#define WRITE_FILE_ERROR				0x30006


class StorageException : public ExceptionSpace::RuntimeException
{
public:
		StorageException(const std::string &msg, t_long code) 
				: ExceptionSpace::RuntimeException(msg, code)
		{


		}
};


enum OpenMode {READ_ONLY = 0, READ_WRITE = 1 };

struct FilesInfo
{
		std::wstring	relative_path;
		t_uint64		length;
		bool			is_filtered;
};


}