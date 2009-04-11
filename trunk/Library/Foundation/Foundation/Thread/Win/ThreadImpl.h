
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


#include "../TypeDef.h"
#include "../ThreadCallBack.h"

#include <windows.h>

namespace ThreadSpace {

typedef void *HANDLE;

/************************************************************************************
注意,如果析构之前不调用detach 则析构函数会调用terminate函数,并等待线程结束

************************************************************************************/
class ThreadImpl : private NonCopyable
{
protected:
		volatile HANDLE					m_hdl;
		
		volatile t_ulong				m_tid;

		volatile ThreadCallBackAbs		*m_ptcb;
private:
		static t_uint32 __stdcall ThreadFuncInstance(void *param);
				
protected:
		//static t_ulong unique_id();
public:
		static void Sleep(t_ulong milliseconds);
		
		static void YieldThread();

		static t_ulong GetCurrentThreadId();
public:
		ThreadImpl(ThreadCallBackAbs *ptcb);

		~ThreadImpl();

		bool Join(t_ulong timeout);

		void Join();
		
		void Terminate();

		Priority GetPriority()const;
		
		bool SetPriority(Priority p);
		
		t_ulong Tid()const;

		bool IsActive()const;

		void* GetThreadHandle()const;

		void Detach();
};



#if(0)

class ThreadImplBase : private NonCopyable
{
protected:
		volatile HANDLE					m_hdl;
		
		volatile t_ulong				m_tid;
protected:
		static t_ulong unique_id();
public:
		static void Sleep(t_ulong milliseconds);
		
		static void YieldThread();
public:
		virtual ~ThreadImplBase();

		void Join(t_ulong timeout);
		
		void Terminate();

		Priority GetPriority()const;
		
		bool SetPriority(Priority p);
		
		t_long Tid()const;

		bool IsActive()const;
};



/****************************************************************************************/
template<class Type, class MFun>
class ThreadImplMF : public ThreadImplBase
{

public:
		ThreadImplMF(Type *ptype, MFun  mfun);
		virtual ~ThreadImplMF();
public:
	//	virtual bool Start();//ThreadFunc  func, void *context);
		
private:
		static t_uint32 __stdcall ThreadFuncInstance(void *param);
private:
		Type	*m_ptype;
		MFun	m_mfun;

};

/*
template<class Type, class MFun>
bool ThreadImplMF<Type, MFun>::Start()
{
		t_uint32 x;
		m_hdl = (HANDLE)_beginthreadex(0, 0, ThreadImplMF<Type, MFun>::ThreadFuncInstance, (void*)this, 0, &x);
		
		if(m_hdl == 0) return false;
		
		m_tid = x;//unique_id();
		
		return true;

}
*/

template<class Type, class MFun>
t_uint32 ThreadImplMF<Type, MFun>::ThreadFuncInstance(void *param)
{
		ThreadImplMF<Type, MFun> *pthis = reinterpret_cast<ThreadImplMF<Type, MFun>*>(param);
		
		Type & type = (*(pthis->m_ptype));
		MFun fun = pthis->m_mfun;

		return (type.*fun)();

}

template<class Type, class MFun>
ThreadImplMF<Type, MFun>::ThreadImplMF(Type *ptype, MFun  mfun)
{
		m_hdl = 0;
		m_tid = 0;

		m_ptype = ptype;
		m_mfun = mfun;

		t_uint32 x;
		m_hdl = (HANDLE)_beginthreadex(0, 0, ThreadImplMF<Type, MFun>::ThreadFuncInstance, (void*)this, 0, &x);
		
		if(m_hdl == 0)
		{
				ThrowThreadException(::GetLastError(), "Can not create thread");
		}
		
		m_tid = x;
}

template<class Type, class MFun>
ThreadImplMF<Type, MFun>::~ThreadImplMF() { }



class ThreadImpl  : public ThreadImplBase
{
public:
		//virtual bool Start();//ThreadFunc  func, void *context);
		
private:
		static t_uint32 __stdcall ThreadFuncInstance(void *param);

public:
		ThreadImpl(ThreadFunction proc, void *context);
		virtual ~ThreadImpl();

private:
		
		ThreadFunction			m_proc;

		void					*m_context;
};














#endif




}