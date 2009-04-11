
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
#include "RefCountedObject.h"
#include "../Thread/AtomicFunction.h"
#include <cassert>

using namespace ThreadSpace;

RefCountedObject::RefCountedObject() : m_rc(1)
{

}

RefCountedObject::~RefCountedObject()
{


}


bool RefCountedObject::Duplicate()const
{
		assert(m_rc >= 1);
		for(;;)
		{
				volatile size_t tmp = m_rc;
				
				if(tmp < 1) return false;
				if(AtomicFunction::CompareExchange(&m_rc, tmp + 1, tmp) == tmp)
				{
						return true;
				}
		}

		//return false;
}



void RefCountedObject::Release()const
{
		assert(m_rc >= 1);
		
		
		for(;;)
		{
				volatile size_t tmp = m_rc;
				if(AtomicFunction::CompareExchange(&m_rc, tmp - 1, tmp) == tmp)
				{
						if(tmp == 1)
						{
								delete this;
						}
						return;
				}
		}

}


size_t RefCountedObject::Count()const
{
		assert(m_rc >= 1);
		for(;;)
		{
				volatile size_t tmp = m_rc;
				assert(tmp >= 1);
				if(AtomicFunction::CompareExchange(&m_rc, tmp, tmp) == tmp)
				{
						return tmp;
				}
		}

		
}


