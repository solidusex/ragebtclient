/*
 * The Foundation Library
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

#include "MsgType.h"
#include "MsgCarrier.h"
#include "../Thread/TSDataPriorityQueue.h"

#include <map>

namespace MsgSpace {



typedef ThreadSpace::TSDataPriorityQueue  TLMsgQueue;


bool IsExisted();
		
bool RegisterMsgQueue(TLMsgQueue *pmq);
		
bool RemoveMsgQueue();

bool PushMsg(t_ulong tid, MsgCarrier *pmc, t_ulong priority);



}