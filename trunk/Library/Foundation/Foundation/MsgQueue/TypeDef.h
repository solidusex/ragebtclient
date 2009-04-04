
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


#include "../Thread/ThreadHdr.h"



namespace MsgSpace {




/*
#define DECL_USE_MSG_POOL(ClassType)\
		static MemorySpace::FixedPool	msg_pool##ClassType;\
		DECL_NEW_WITH_FIXEDPOOL_IN_CLASS(ClassType, msg_pool##ClassType)


//必须在.cpp文件中声明
#define DEFINE_MSG_POOL(ClassType)\
		MemorySpace::FixedPool	ClassType::msg_pool##ClassType(sizeof(ClassType), sizeof(ClassType)*4);

*/


}