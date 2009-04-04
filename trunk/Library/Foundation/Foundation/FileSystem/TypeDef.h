
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

#include "../Common/ComnHdr.h"
#include "../Exception/ExptHdr.h"
#include "../Utility/UtilityHdr.h"
#include "../DataStruct/DSHdr.h"

namespace FileSysSpace {

//namespace {



//读写方式
static const t_ulong F_READ_ONLY	=	BIT_MARK(0)	;			//只读
		

static const t_ulong F_WRITE_ONLY =	BIT_MARK(1);				//只写
		

static const t_ulong F_READ_WRITE	=  BIT_MARK(3);		//读写
		
//打开方式----------------------------------------------/
static const t_ulong F_CREATE_NEW = BIT_MARK(11);					//如果存在则失败 否则创建新文件
		

static const t_ulong F_CREATE_ALWAYS = BIT_MARK(12);				//如果存在则截断为0 否则创建新文件
		

static const t_ulong F_OPEN_EXISTED	= BIT_MARK(13);				//如果存在则打开，否则失败
		

static const t_ulong F_OPEN_ALWAYS	= BIT_MARK(14);				//如果存在则打开，否则创建新文件

static const t_ulong F_TRUNC = BIT_MARK(10);						//如果存在则截断为0否则失败
		
//共享模式---------------------------------------------/

static const t_ulong F_SHARED_READ	= BIT_MARK(20);				//共享模式：共享读

static const t_ulong F_SHARED_WRITE = BIT_MARK(21);			//共享写

static const t_ulong F_SHARED_READ_WRITE = BIT_MARK(22);		////共享读写
		

static const t_ulong F_NOSHARED = 	BIT_MARK(23);					//共享模式：独占


static const t_ulong STD_READ = F_READ_ONLY|F_OPEN_EXISTED|F_SHARED_READ;

static const t_ulong STD_WRITE = F_READ_WRITE|F_OPEN_EXISTED|F_SHARED_READ;


//////////////////////////////////内存映射文件的////////////////////////////////
static const t_ulong MF_READ_ONLY = BIT_MARK(0);

static const t_ulong MF_READ_WRITE = BIT_MARK(1);

static const t_ulong MF_WRITE_COPY = BIT_MARK(2);

//------------------------------------------------

static const t_ulong MF_MAP_READ_ONLY = BIT_MARK(10);

static const t_ulong MF_MAP_READ_WRITE = BIT_MARK(11);

static const t_ulong MF_MAP_COPY_ON_WRITE = BIT_MARK(12);

/****************************************************************/
//}

enum SeekPosition { BEG = 0x00, CURR = 0x01, END = 0x02 };


enum PathType { PATH_ROOT = 0x90, PATH_DIR, PATH_FILE, INVALID_PATH };



}























#if(0)
class  FoundationAPI  PathExcetion : public ExceptionBase<std::wstring>
{
public:
		PathExcetion(const std::wstring &msg) : ExceptionBase(msg) {   }
};




#define READ_FILE				GENERIC_READ

#define WRITE_FILE				GENERIC_WRITE

#define TRUNCATE_FILE			CREATE_ALWAYS

#define OPEN_FILE				OPEN_EXISTING





#if defined(OS_FAMILY_WINDOWS)



#define PATH_SP					'\\'
#define PATH_SP_STR				"\\"
#define MAXPATHLEN				1024
#define MAXHOSTNAMELEN			256


#endif


#endif




