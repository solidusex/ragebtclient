
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
#include "../Utility/UtilityHdr.h"
#include "../Exception/ExptHdr.h"
#include "../Thread/ThreadHdr.h"
#include "../Time/TimeHdr.h"
#include "../DataStruct/DSHdr.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/TypeDefImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif