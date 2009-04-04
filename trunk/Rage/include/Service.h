#pragma once

#include "TypeDef.h"


namespace Rage {

#define SELECTOR_CHECK_TIME		50		//∫¡√Î
#define TIMER_MIN_RESOLUTION	20		//∫¡√Î

#define WORKER_THREAD_SIZE		3




class RageAPI NetService : private NonCopyable
{
private:
		typedef NetSpace::Selector		SelectorT;
		typedef ThreadSpace::Thread		ThreadT;
private:
		std::vector<SelectorT*>			m_selector_set;
		std::vector<ThreadT*>			m_worker_set;
		bool							m_is_started;
private:
		void worker_thread(SelectorT* pselector);
public:
		NetService();

		~NetService();
public:
		bool Start(size_t count);

		void Stop();

		bool IsStarted()const;

		bool RegisterHandler(NetSpace::NetEventHandler *handler);

		bool RemoveHandler(NetSpace::NetEventHandler *handler);
};







////////////////////////////////////////AsyncService///////////////////////////




class RageAPI AsyncRequest : public ::RefCountedObject
{
public:
		virtual void Run()throw() = 0;

		virtual ~AsyncRequest()throw()
		{

		}
};


class RageAPI AsyncService : private NonCopyable
{
private:
		typedef ThreadSpace::ThreadPool			ThreadPoolT;
		typedef ThreadSpace::TSDataQueue		TSQueueT;
private:
		bool					m_is_started;
		ThreadPoolT				m_pool;
		TSQueueT				m_queue;
private:
		void worker_thread();
public:
		AsyncService();
		~AsyncService();
public:
		bool Start(size_t num_thd = 3);
		bool IsStarted()const;
		void Stop();
		void PostRequest(AsyncRequest *preq);
};









//////////////////////TimerService///////////////////////////////









typedef ThreadSpace::TimerManager		TimerService;
typedef ThreadSpace::Timer				TimerT;






///////////////////////////ServiceManager/////////////////////////////////////////////////

class RageAPI ServiceManager : private NonCopyable
{
private:
		NetService				m_net_serv;
		TimerService			m_timer_serv;
		AsyncService			m_async_req_serv;
		bool					m_is_started;
public:
		NetService& GetNetService();
		TimerService& GetTimerService();
		AsyncService& GetAsyncService();
public:
		ServiceManager();
		~ServiceManager();
public:
		bool Start();
		bool IsStarted()const;
		void Stop();
};




}