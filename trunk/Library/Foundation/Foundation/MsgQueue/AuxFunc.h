
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

#include "MsgType.h"
#include "TypeDef.h"



namespace MsgSpace {





FoundationAPI bool PostMessageEx(t_ulong tid, Message *pmsg, t_ulong priority = 1)throw();

FoundationAPI bool SendMessageEx(t_ulong tid, Message *pmsg, void *&pres, t_ulong priority = MAX_LONG)throw();

FoundationAPI bool SendMessageTimeoutEx(t_ulong tid, Message *pmsg, t_ulong timeout, void *&pres, t_ulong priority = MAX_LONG)throw();






}

#define PostMsgEx PostMessageEx

#define SendMsgEx SendMessageEx

#define SendMsgTimeoutEx SendMessageTimeoutEx