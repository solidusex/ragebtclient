
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
#include "Condition.h"


namespace ThreadSpace {


void Condition::dequeue()
{
		m_queue.pop_front();
}


void Condition::dequeue(Event &event)
{
		for(WaitQueue::iterator it = m_queue.begin(); it != m_queue.end(); ++it)
		{
				if (*it == &event)
				{
						m_queue.erase(it);
						break;
				}
		}
}

void Condition::enqueue(Event &event)
{
		m_queue.push_back(&event);
}



Condition::Condition()
{

}
		
Condition::~Condition()
{
		assert(m_queue.empty());
}


void Condition::Signal()
{
		LockType lock(m_mutex);
		if(!m_queue.empty())
		{
				m_queue.front()->Set();
				dequeue();
		}
}

void Condition::BroadCast()
{
		LockType lock(m_mutex);
		for(WaitQueue::iterator it = m_queue.begin(); it != m_queue.end(); ++it)
		{
				
				(*it)->Set();
		}

		m_queue.clear();

}

















}