

#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "test.h"

BitSet& BitSet::clear(bool tagClear) 
{
    if (tagClear)
        memset(m_pBits, 0, m_size * BLOCK_BYTES);
    else {
			memset(m_pBits, std::numeric_limits<unsigned char>::max(), m_size * BLOCK_BYTES);
        m_pBits[m_size-1] &= m_mask;  // filter the redundant bits
    }
    return (*this);
}

//--- bit operation ----------------------------------------
void BitSet::leftShift(size_type num)
{
    // function: left-shift num bits
		if (num >= m_bitNum) {
				// when the number of bits asked to shift is more than
				// that possessed by the object, just clear all the bits.
				clear();
				return ;
		}

		// compute the corresponding number of 
		// elements(ULONG) and the remaining bits 
		size_type eleNum = num / BLOCK_BITS; 
		size_type bitNum = num % BLOCK_BITS;

		// first left-shift eleNum elements 
		if (eleNum != 0) {
				block_type* pTmp = new block_type[m_size];
				if (pTmp == NULL)
						throw std::bad_alloc();
				memcpy(pTmp, m_pBits, (m_size - eleNum) * BLOCK_BYTES);
				memcpy(m_pBits + eleNum, pTmp, (m_size - eleNum) * BLOCK_BYTES);
				memset(m_pBits, 0, eleNum * BLOCK_BYTES);
				delete [] pTmp;
		}

		// next, left-shift bitNum bits (1 <= bitNum <= ULONG_BITS - 1)
		if (bitNum != 0) {
				block_type *pTmp  = m_pBits + m_size - 1;
				for ( ; pTmp > m_pBits; --pTmp) {
						*pTmp = (*pTmp << bitNum) | (*(pTmp - 1) >> (BLOCK_BITS - bitNum));
				}
				*pTmp <<= bitNum;
		}
		m_pBits[m_size-1] &= m_mask;      // filter the redundant bits
}

void BitSet::rightShift(size_type num)
{
		// function: right-shift num bits
		if (num >= m_bitNum) {
				// when the number of bits asked to shift is more than
				// that possessed by the object, just clear all the bits.
				clear();
				return ;
		}

		// compute the corresponding number of elements(ULONG) and remaining bits 
		size_type eleNum = num / BLOCK_BITS; 
		size_type bitNum = num % BLOCK_BITS;

		// first right-shift eleNum elements 
		if (eleNum != 0) {
				block_type* pTmp = new block_type[m_size];
				if (pTmp == NULL)
						throw std::bad_alloc();
				memcpy(pTmp, m_pBits + eleNum, (m_size - eleNum) * BLOCK_BYTES);
				memcpy(m_pBits, pTmp, (m_size - eleNum) * BLOCK_BYTES);
				memset(m_pBits + m_size - eleNum, 0, eleNum * BLOCK_BYTES);
				delete [] pTmp;
		}

		// next, right-shift bitNum bits (1 <= bitNum <= ULONG_BITS - 1)
		if (bitNum != 0) {
				block_type *pTmp  = m_pBits;
				for ( ; pTmp < m_pBits + m_size - 1; ++pTmp) {
						*pTmp = (*pTmp >> bitNum) | (*(pTmp + 1) << (BLOCK_BITS - bitNum));
				}
				*pTmp >>= bitNum;
		}
}

BitSet& BitSet::set(size_type pos, bool tagSet) 
{
		// function: set the bit at position pos (beginning from 0) when tagSet is true;
		//           otherwise, clear the bit.
		if (pos >= m_bitNum)
				throw std::out_of_range("Error: out of range in fuction set");

		// compute the corresponding index of elements(ULONG) and remaining bits 
		size_type eleIndex = pos / BLOCK_BITS; 
		size_type bitIndex = pos % BLOCK_BITS;

		block_type mask = 1;
		mask <<= bitIndex;
		if (!tagSet)
				mask = ~mask;
		m_pBits[eleIndex] = tagSet ? (m_pBits[eleIndex] | mask) : (m_pBits[eleIndex] & mask);
		return (*this);
}

BitSet& BitSet::flip(size_type pos) 
{
		// function: flip the bit at position pos
		if (pos >= m_bitNum)
				throw std::out_of_range("Error: out of range in fuction set");

		// compute the corresponding index of elements(ULONG) and remaining bits 
		size_type eleIndex = pos / BLOCK_BITS; 
		size_type bitIndex = pos % BLOCK_BITS;

		block_type mask = 1;
		mask <<= bitIndex;
		m_pBits[eleIndex] = (m_pBits[eleIndex] & mask) ? (m_pBits[eleIndex] & ~mask) : (m_pBits[eleIndex] | mask);
		return (*this);
}

bool BitSet::test(size_type pos) const 
{
		// function: test the bit at position pos. 
		// return:   If the bit is set, then return true; otherwise return false.
		if (pos >= m_bitNum)
				throw std::out_of_range("Error: out of range in fuction set");

		// compute the corresponding index of elements(ULONG) and remaining bits 
		size_type eleIndex = pos / BLOCK_BITS; 
		size_type bitIndex = pos % BLOCK_BITS;

		block_type mask = 1;
		mask <<= bitIndex;
		return m_pBits[eleIndex] & mask;
}
























/*
class TimerService : private NonCopyable
{
private:
		struct NotifyProc
		{
		private:
				TaskID			m_task_id;
				TimerID			m_timer_id;
		public:
				NotifyProc(TaskID task_id, TimerID timer_id) : m_task_id(task_id), m_timer_id(timer_id)
				{

				}
				
				void OnTimer()
				{
						bool res = PostMessageEx(m_task_id, new TimerNotify(m_timer_id));
						assert(res);
				}
		};

		typedef ThreadSpace::Timer		TimerT;

		struct TimerInfo
		{
				TimerT			*timer;
				NotifyProc		*notifier;
		};
public:
		

		typedef ThreadSpace::TimerQueue	TimerQueueT;
		typedef ThreadSpace::FastMutex	MutexT;
		typedef MutexT::ScopeLock		LockT;
private:
		typedef std::map<TimerID, TimerInfo> TimerSet;
		typedef std::map<TaskID, TimerSet> TaskSet;
private:
		bool					m_is_started;
		TimerQueueT				m_queue;
		TaskSet					m_task_set;
		MutexT					m_mutex;
private:
		TimerInfo create_timer_info(TaskID task_id, TimerID timer_id, t_uint64 interval);

public:
		TimerService();
		~TimerService();
public:
		bool Start();
		bool IsStarted()const;
		void Stop();
		void Clear();
public:
		bool RegisterTimer(TaskID task_id, TimerID timer_id, t_uint64 interval);
		bool RemoveTimer(TaskID task_id, TimerID timer_id);
		bool RemoveTask(TaskID task_id);
};

TimerService::TimerService() : m_is_started(false)
{


}

TimerService::~TimerService()
{
		assert(!IsStarted());
}


TimerService::TimerInfo TimerService::create_timer_info(TaskID task_id, TimerID timer_id, t_uint64 interval)
{
		NotifyProc *pn = new NotifyProc(task_id, timer_id);
		TimerT *ptimer = new TimerT(pn, &NotifyProc::OnTimer, &m_queue);

		TimerInfo ti;
		ti.notifier = pn;
		ti.timer = ptimer;
		ti.timer->SetInterval(interval);
		ti.timer->Start();
		return ti;
}

void TimerService::Clear()
{
		assert(IsStarted());
		LockT lock(m_mutex);

		for(TaskSet::iterator it = m_task_set.begin(); it != m_task_set.end(); ++it)
		{
				for(TimerSet::iterator timer_it = it->second.begin(); timer_it != it->second.end(); ++timer_it)
				{
						try{
								timer_it->second.timer->Stop();
						}catch(...)
						{
								DEBUG_PRINT0("Unknow exception : TimerService::Clear()\n");
						}
						delete timer_it->second.timer;
						delete timer_it->second.notifier;
				}
		}
		m_task_set.clear();
}



bool TimerService::RegisterTimer(TaskID task_id, TimerID timer_id, t_uint64 interval)
{
		assert(IsStarted());

		LockT lock(m_mutex);
		TaskSet::iterator task_it = m_task_set.find(task_id);
		
		if(task_it == m_task_set.end())
		{
				try{
						TimerInfo ti = create_timer_info(task_id, timer_id, interval);
						m_task_set[task_id][timer_id] = ti;
				}catch(...)
				{
						return false;
				}
				return true;
		}else
		{
				TimerSet::iterator timer_it = task_it->second.find(timer_id);

				if(timer_it != task_it->second.end())
				{
						return false;
				}else
				{
						try{
								TimerInfo ti = create_timer_info(task_id, timer_id, interval);
								task_it->second[timer_id] = ti;
								return true;
						}catch(...)
						{
								return false;
						}
				}
		}
}

bool TimerService::RemoveTimer(TaskID task_id, TimerID timer_id)
{
		assert(IsStarted());

		LockT lock(m_mutex);
		TaskSet::iterator task_it = m_task_set.find(task_id);
		
		if(task_it == m_task_set.end()) return false;
		
		TimerSet::iterator timer_it = task_it->second.find(timer_id);
		if(timer_it == task_it->second.end())
		{
				return false;
		}else
		{
				try{
						timer_it->second.timer->Stop();
						delete timer_it->second.timer;
						delete timer_it->second.notifier;
				}catch(...)
				{
						DEBUG_PRINT0("UnKnow Exception : TimerService::RemoveTimer\n");
				}
				task_it->second.erase(timer_it);
				if(task_it->second.empty())
				{
						m_task_set.erase(task_it);
				}
				return true;;
		}



}


bool TimerService::RemoveTask(TaskID task_id)
{
		assert(IsStarted());

		LockT lock(m_mutex);

		TaskSet::iterator task_it = m_task_set.find(task_id);

		if(task_it != m_task_set.end())
		{
				for(TimerSet::iterator timer_it = task_it->second.begin(); timer_it != task_it->second.end(); ++timer_it)
				{
						try{
								timer_it->second.timer->Stop();
								delete timer_it->second.timer;
								delete timer_it->second.notifier;
						}catch(...)
						{
								DEBUG_PRINT0("UnKnow exception : TimerService::RemoveTask\n");
						}
				}

				m_task_set.erase(task_it);
				return true;
		}else
		{
				return false;
		}
}



bool TimerService::Start()
{
		if(m_is_started)
		{
				return false;
		}else
		{
				m_is_started = true;
				return true;
		}
}

bool TimerService::IsStarted()const
{
		return m_is_started;
}

void TimerService::Stop()
{
		if(m_is_started)
		{
				Clear();
				m_is_started = false;
		}
}
*/