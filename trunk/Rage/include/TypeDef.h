
/*
 * The Rage Library
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

#include "Config.h"

namespace Rage {




#define MAX_SEND_BUFF_QUEUE_SIZE				(512*KB)

#define SLICE_LENGTH							(16 *KB)

#define NONE_PIECE_INDEX						((t_uint32)~0)

typedef t_ulong									TaskID;

#define INVALID_TASK_ID							~0




typedef DSSpace::ReverseBitSet							BitField;


enum ConnMode { LOCAL = 0x000A, REMOTE = 0x000B };

#if defined(OS_FAMILY_WINDOWS)

#define RAGE_PATH_SP_W				L"\\"

#else

#define RAGE_PATH_SP_W				L"//"


#endif

}