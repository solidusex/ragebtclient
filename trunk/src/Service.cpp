/*
 * The Rage Library
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
 */#include "Service.h"


namespace Rage {

///////////////////////////////////////////////////NetService//////////////////////////



NetService::NetService() : m_is_started(false)
{


}

NetService::~NetService()
{
		assert(!IsStarted());

}


bool NetService::IsStarted()const
{
		return m_is_started;
}
void NetService::worker_thread(SelectorT* pselector)
{
		pselector->Run(SELECTOR_CHECK_TIME);

}

bool NetService::Start(size_t count)
{
		if(IsStarted())
		{
				assert(0);
				return false;
		}

		if(count < 1) count = 1;
		NetInit();
		assert(m_selector_set.empty());
		assert(m_worker_set.empty());
		
		for(size_t i = 0; i < count; ++i)
		{
				m_selector_set.push_back(new SelectorT());
		}

		for(size_t i = 0; i < count; ++i)
		{
				m_worker_set.push_back(new ThreadT(this, &NetService::worker_thread, m_selector_set[i]));

		}
		
		m_is_started = true;
		return true;
}

void NetService::Stop()
{
		if(!IsStarted())
		{
				assert(0);
				return;
		}

		assert(!m_selector_set.empty());
		assert(!m_worker_set.empty());
		assert(m_selector_set.size() == m_worker_set.size());

		for(size_t i = 0; i < m_selector_set.size(); ++i)
		{
				m_selector_set[i]->Stop();
		}

		for(size_t i = 0; i < m_worker_set.size(); ++i)
		{
				m_worker_set[i]->Join();
				delete m_worker_set[i];

		}

		for(size_t i = 0; i < m_selector_set.size(); ++i)
		{
				m_selector_set[i]->RemoveAll();
				delete m_selector_set[i];
		}

		m_selector_set.clear();
		m_worker_set.clear();
		
		
		m_is_started = false;

}


bool NetService::RegisterHandler(NetSpace::NetEventHandler *handler)
{
		assert(IsStarted());
		assert(handler != 0);
		SelectorT *pselector = m_selector_set[0];
		assert(pselector != 0);

		for(size_t i = 1; i < m_selector_set.size(); ++i)
		{
				if(pselector->Count() > m_selector_set[i]->Count())
				{
						pselector = m_selector_set[i];
				}
		}
		
		assert(pselector != 0);
		return pselector->RegisterHandler(handler);
}

bool NetService::RemoveHandler(NetSpace::NetEventHandler *handler)
{
		assert(IsStarted());
		assert(handler != 0);
		SelectorT *pselector = 0;
		size_t min_size = 0;

		for(size_t i = 0; i < m_selector_set.size(); ++i)
		{
				assert(m_selector_set[i] != 0);
				if(m_selector_set[i]->RemoveHandler(handler))
				{
						return true;
				}
		}

		return false;
}





////////////////////////////////////////////AsyncService///////////////////////




AsyncService::AsyncService() 
		: m_is_started(false)
{



}

AsyncService::~AsyncService()
{
		if(IsStarted())
		{
				Stop();
		}

}

bool AsyncService::Start(size_t num_thd)
{
		if(IsStarted())
		{
				return false;
		}

		if(num_thd < 1) num_thd = 1;

		
		if(!m_pool.Start(num_thd))
		{
				return false;
		}

		m_is_started = true;
		for(size_t i = 0; i < num_thd; ++i)
		{
				m_pool.Execute(this, &AsyncService::worker_thread);
		}

		return true;
}

bool AsyncService::IsStarted()const
{
		return m_is_started;
}

#define ASYNC_SERV_CHECK_INTERVAL   200 //ºÁÃë
void AsyncService::Stop()
{
		if(IsStarted())
		{
				m_is_started = false;
				m_pool.Stop();

				while(!m_queue.IsEmpty())
				{
						void *ptr = 0;
						m_queue.WaitDequeue(ptr);
						assert(ptr != 0);
						((AsyncRequest*)ptr)->Release();
				}
		}

}

void AsyncService::PostRequest(AsyncRequest *preq)
{
		if(!IsStarted())
		{
				assert(0);
				return;
		}

		assert(preq != 0);
		preq->Duplicate();
		m_queue.Enqueue(preq);
		
}

void AsyncService::worker_thread()
{
		while(m_is_started)
		{
				void *ptr_res = 0;

				if(m_queue.WaitDequeue(ptr_res, ASYNC_SERV_CHECK_INTERVAL))
				{
						((AsyncRequest*)ptr_res)->Run();
						((AsyncRequest*)ptr_res)->Release();
				}else
				{
						//DEBUG_PRINT0("NO REQUEST POST IN\n");
						continue;
				}
		}
}



////////////////////////////////ServiceManager/////////////////////////////////////////////






NetService& ServiceManager::GetNetService()
{
		return m_net_serv;

}

TimerService& ServiceManager::GetTimerService()
{
		return m_timer_serv;
}

AsyncService& ServiceManager::GetAsyncService()
{
		return m_async_req_serv;
}

bool ServiceManager::IsStarted()const
{
		return m_is_started;
}

ServiceManager::ServiceManager() : m_is_started(false)
{


}

ServiceManager::~ServiceManager()
{
		if(IsStarted())
		{
				Stop();
		}
}


bool ServiceManager::Start()
{
		if(IsStarted())
		{
				return false;
		}else
		{
				m_is_started = true;
				m_timer_serv.Start(TIMER_MIN_RESOLUTION);
				m_async_req_serv.Start(3);
				m_net_serv.Start(WORKER_THREAD_SIZE);
				
				return true;
		}
};

void ServiceManager::Stop()
{
		if(IsStarted())
		{
				m_is_started = false;
				m_timer_serv.Stop();
				m_async_req_serv.Stop();
				m_net_serv.Stop();
				
		}
}





















#if(0)
////////////////////////////////ServiceManager///////////////////////////////////////


NetService& ServiceManager::GetNetService()
{
		assert(IsStarted());
		return m_net_serv;
}

AsyncRequestService& ServiceManager::GetAsyncReqService()
{
		assert(IsStarted());
		return m_async_req_serv;
}
TimerService& ServiceManager::GetTimerService()
{
		assert(IsStarted());

		return m_timer_serv;
}


ServiceManager::ServiceManager() : m_is_started(false)
{

}

ServiceManager::~ServiceManager()
{
		assert(!IsStarted());

}


bool ServiceManager::Start()
{
		if(!IsStarted())
		{
				m_net_serv.Start();
				m_async_req_serv.Start();
				m_timer_serv.Start(20);
				m_is_started = true;
				return true;
		}else
		{
				return false;
		}

}

bool ServiceManager::IsStarted()const
{
		return m_is_started;
}

void ServiceManager::Stop()
{
		if(IsStarted())
		{
				m_net_serv.Stop();
				m_async_req_serv.Stop();
				m_timer_serv.Stop();
				m_is_started = false;
		}
}
#if(0)

#if(0)

class RageAPI NetService : private NonCopyable
{
private:
		typedef NetSpace::Selector		SelectorT;
		typedef ThreadSpace::Thread		ThreadT;
private:
		SelectorT				m_selector;
		ThreadT					*m_pworker_thd;
private:
		void worker_thread();
public:
		NetService();

		~NetService();
public:
		bool Start();

		void Stop();

		bool IsStarted()const;

		bool RegisterHandler(NetSpace::NetEventHandler *handler);

		bool RemoveHandler(NetSpace::NetEventHandler *handler);
};

#endif
NetService::NetService() : m_pworker_thd(0)
{
		


}


NetService::~NetService()
{
		if(IsStarted())
		{
				Stop();
		}
}


bool NetService::Start()
{
		if(IsStarted())
		{
				return false;
		}else
		{
				NetInit();
				m_pworker_thd = new ThreadT(this, &NetService::worker_thread);
				return true;
		}
}

void NetService::worker_thread()
{
		m_selector.Run(SELECTOR_CHECK_TIME);
}


void NetService::Stop()
{
		if(IsStarted())
		{
				m_selector.Stop();
				m_pworker_thd->Join();
				delete m_pworker_thd;
				m_pworker_thd = 0;
		}
}

bool NetService::IsStarted()const
{
		return (m_pworker_thd != 0);
}

bool NetService::RegisterHandler(NetSpace::NetEventHandler *handler)
{
		assert(handler != 0);
		return m_selector.RegisterHandler(handler);
}

bool NetService::RemoveHandler(NetSpace::NetEventHandler *handler)
{
		assert(handler != 0);
		return m_selector.RemoveHandler(handler);
}

#endif


#endif



}