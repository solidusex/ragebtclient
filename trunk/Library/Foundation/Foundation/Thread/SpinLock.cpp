
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
#include "SpinLock.h"
#include "Thread.h"


namespace ThreadSpace {

#define MAX_SPIN_COUNT  400

SpinLock::ScopeLock::ScopeLock(SpinLock &lock) :m_lock(lock)
{
		m_lock.Lock();
}



SpinLock::ScopeLock::~ScopeLock()
{
		m_lock.UnLock();
}


SpinLock::SpinLock() : m_state(UNLOCK)
{

}

SpinLock::~SpinLock()
{
		assert(m_state == UNLOCK);
}

void SpinLock::Lock()
{
		size_t count =  0;

		while(AtomicF::CompareExchange(&m_state, LOCK, UNLOCK) != UNLOCK)
		{
				if(++count >= MAX_SPIN_COUNT)
				{
						Thread::YieldThread();
						count = 0;
				}
		}
}

void SpinLock::UnLock()
{
		assert(m_state == LOCK);
		AtomicF::CompareExchange(&m_state, UNLOCK, LOCK);
}



///////////////////////////////////////////////////////////////////////////////////

RecursiveSpinLock::ScopeLock::ScopeLock(RecursiveSpinLock &lock) :m_lock(lock)
{
		m_lock.Lock();
}



RecursiveSpinLock::ScopeLock::~ScopeLock()
{
		m_lock.UnLock();
}



RecursiveSpinLock::RecursiveSpinLock() 
: m_state(UNLOCK), m_tid(0), m_recu_count(0)
{

}

RecursiveSpinLock::~RecursiveSpinLock()
{
		assert(m_state == UNLOCK);
		assert(m_tid == 0);
		assert(m_recu_count == 0);
}

bool RecursiveSpinLock::IsOwner()const
{
		t_ulong curr_tid = ThreadSpace::Thread::GetCurrentThreadId();
		return m_tid == curr_tid;
}

bool RecursiveSpinLock::TryLock()
{
		t_ulong curr_tid = ThreadSpace::Thread::GetCurrentThreadId();
		if(m_tid == curr_tid)
		{
				assert(m_state == LOCK);
				assert(m_recu_count >= 1);
				m_recu_count++;
				return true;
		}else
		{
				if(AtomicF::CompareExchange(&m_state, LOCK, UNLOCK) == UNLOCK)
				{
						assert(m_recu_count == 0);
						assert(m_tid == 0);
						m_tid = curr_tid;
						m_recu_count++;
						return true;
				}else
				{
						return false;
				}
		}
}

void RecursiveSpinLock::Lock()
{
		size_t count = 0;

		while(!TryLock())
		{
				if(++count >= MAX_SPIN_COUNT)
				{
						Thread::YieldThread();
						count = 0;
				}
		}

}

void RecursiveSpinLock::UnLock()
{
		assert(m_state == LOCK);
		assert(m_recu_count > 0);
		t_ulong curr_tid = ThreadSpace::Thread::GetCurrentThreadId();
		assert(m_tid == curr_tid);
		if(curr_tid != m_tid) return;

		if(--m_recu_count == 0)
		{
				m_tid = 0;
				AtomicF::CompareExchange(&m_state, UNLOCK, LOCK);
		}
}





}


#undef MAX_SPIN_COUNT






/*t_ulong curr_tid = ThreadSpace::Thread::GetCurrentThreadId();
		assert(curr_tid != 0);
		if(m_tid == curr_tid)
		{
				assert(m_state == LOCK);
				assert(m_recu_count >= 1);
				m_recu_count++;
		}else
		{
				while(AtomicF::CompareExchange(&m_state, LOCK, UNLOCK) != UNLOCK)
				{
						Thread::YieldThread();
				}
				assert(m_recu_count == 0);
				assert(m_tid == 0);
				m_tid = curr_tid;
				m_recu_count++;
		}*/
























#if(0)


SpinLock::ScopeLock::ScopeLock(ThreadSpace::SpinLock &lock) :m_lock(lock)
{
		m_lock.Lock();
}



SpinLock::ScopeLock::~ScopeLock()
{
		m_lock.UnLock();
}


bool SpinLock::TryLock(size_t count)
{

		if(AtomicF::CompareExchange((t_long*)&m_locked_count, 1, 0) == 0)
		{
				m_owner_tid = Thread::GetCurrentThreadId();
				return true;
				//注意,如果在一个同一个线程锁两次,那么这个TID必然是自己的,如果不是
				//否则,不论TID是什么,都不能++m_locked_count;因此,没必要锁m_owner_tid;
		}else if(Thread::GetCurrentThreadId() == m_owner_tid)
		{
				AtomicF::Increment((t_long*)&m_locked_count);
				return true;
		}else
		{
				while(count--)
				{
						if(AtomicF::CompareExchange((t_long*)&m_locked_count, 1, 0) == 0)
						{
								m_owner_tid = Thread::GetCurrentThreadId();
								return true;
						}else
						{
								Thread::YieldThread();
								
						}
				}

				return false;
		}
}





void SpinLock::Lock()
{

		if(AtomicF::CompareExchange((t_long*)&m_locked_count, 1, 0) == 0)
		{
				m_owner_tid = Thread::GetCurrentThreadId();
				//printf("acquire tid == %d\n", m_owner_tid);
				return;
				//注意,如果在一个同一个线程锁两次,那么这个TID必然是自己的,如果不是
				//否则,不论TID是什么,都不能++m_locked_count;因此,没必要锁m_owner_tid;
		}else if(Thread::GetCurrentThreadId() == m_owner_tid)
		{
				//printf("tid == %d locked count == ++%d\n", m_owner_tid, m_locked_count);
				AtomicF::Increment((t_long*)&m_locked_count);
		}else
		{
				while(true)
				{
						if(AtomicF::CompareExchange((t_long*)&m_locked_count, 1, 0) == 0)
						{
								m_owner_tid = Thread::GetCurrentThreadId();
						//		printf("acquire tid == %d\n", m_owner_tid);
								return;
						}else
						{
								Thread::YieldThread();
						}
				}
		}

}

//同上,如果线程已经获得, 则可m_tid一定等于curr tid;
//否则,必然错误
void SpinLock::UnLock()
{
		
		if(Thread::GetCurrentThreadId() == m_owner_tid)
		{
				//printf("release tid == %d\n", m_owner_tid);
				if(m_locked_count == 1)
				{
						m_owner_tid = 0;

						assert(m_owner_tid == 0);
						assert(m_locked_count == 1);
				}
				

				AtomicF::Decrement((t_long*)&m_locked_count);
		}

}



SpinLock::SpinLock() : m_owner_tid(0), m_locked_count(0)
{
		
		

}

SpinLock::~SpinLock()
{
#if defined(FOUNDATION_DEBUG)
		if(m_owner_tid != 0)
		{
				assert(0);
		}
#endif


}
#endif