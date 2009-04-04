
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
#include "Selector.h"

#include "SocketSet.h"
#include "SockStream.h"
#include "Utility.h"
namespace NetSpace{

void Selector::RemoveAll()
{
		Lock lock(m_mutex);		//��ֹ�ڴ�ʱ�������߳��޸�m_map;
		for(HandlerMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
		{
				it->second->m_selector = 0;
				it->second->Release();
		}

		m_map.clear();
}

void Selector::Run(t_uint64 timer_interval)
{

		SockStream garbage;
		if(!garbage.Open())
		{
				assert(false);
				DEBUG_PRINT1("Selector::Run failed! ErrorCode == %ld\n", NetGetLastError());
				throw ExceptionSpace::NetException("Selector::Run failed! ErrorCode == %ld\n", NetGetLastError());
		}
		
		TimeSpace::TimeStamp ts;
		t_uint64 time_left = timer_interval;
		
		m_is_running = true;

		::fd_set rd_set, wd_set, ex_set;
		
		while(m_is_running)
		{
				FD_ZERO(&rd_set);
				FD_ZERO(&wd_set);
				FD_ZERO(&ex_set);

				size_t rd_count = 0, wd_count = 0, ex_count = 0;

				{
						Lock lock(m_mutex);		//��ֹ�ڴ�ʱ�������߳��޸�m_map;
					
						for(HandlerMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
						{
								if(it->second->Mask() & INPUT_MASK)
								{
										FD_SET(it->first, &rd_set);
										rd_count++;
								}

								if(it->second->Mask() & OUTPUT_MASK)
								{
										FD_SET(it->first, &wd_set);
										wd_count++;
								}

								if(it->second->Mask() & EXPT_MASK)
								{
										FD_SET(it->first, &ex_set);
										ex_count++;
								}
						}

				}

				if(rd_count == 0 && wd_count == 0 && ex_count == 0)
				{
						rd_count++;
						FD_SET(garbage.GetHandle(), &rd_set);
				}


				fd_set *prd = (rd_count != 0 ? &rd_set : 0);
				fd_set *pwd = (wd_count != 0 ? &wd_set : 0);
				fd_set *pexd = (ex_count != 0 ? &ex_set : 0);
				
				
				//�⼸���п����и����⣬���ǵ�����ִ����FD_SET����֮�����
				//�κ��ѱ�FD_SET��*_set��socket��Remove����֮��ر���socket
				//��ô�����res�᷵��-1����������ѡ��Ĳ����Ǵ�ӡһ����Ϣ��
				//continue��������һ�ο����������������register������
				//socket��remove�͹رյ��ˣ���ô�����ڱ�̴���

				int res = Utility::Select(prd, pwd, pexd, &time_left);

				if(res == 0)
				{
						time_left = timer_interval;
						on_timer();//��������on timerȻ����update,��֤����ʱ����
						ts.Update();

				}else if(res > 0)
				{
						t_uint64 time_eplased = ts.ElapsedMillisecond();
						if(time_eplased >= time_left)
						{
								time_left = timer_interval;
								on_timer();
								ts.Update();
						}else
						{
								time_left -= time_eplased;
						}
						

						if(prd != 0)
						{
								FdSetIterator fd_iter(rd_set);

								for(fd_iter.First(); !fd_iter.IsDone(); fd_iter.Next())
								{
										dispatch_read(fd_iter.Current());
								}
						}
						
						if(pwd != 0)
						{
								FdSetIterator fd_iter(wd_set);
						
								for(fd_iter.First(); !fd_iter.IsDone(); fd_iter.Next())
								{
										dispatch_write(fd_iter.Current());
								}
						}
						
						if(pexd != 0)
						{
								FdSetIterator fd_iter(ex_set);

								for(fd_iter.First(); !fd_iter.IsDone(); fd_iter.Next())
								{
										dispatch_expt(fd_iter.Current());
								}
						}
				}else
				{
						DEBUG_PRINT1("Error code == %ld\n", NetGetLastError());
						continue;
				}
		}

		garbage.Close();
		//RemoveAll();
}




bool Selector::IsRunning()const
{
		return m_is_running;
}

bool Selector::dispatch_read(const NetHandle &nhdl)
{
		Lock lock(m_mutex);

		HandlerMap::iterator it = m_map.find(nhdl);

		if(it != m_map.end() && (it->second->Mask() & INPUT_MASK))
		{
				it->second->OnInput();
				return true;
		}else
		{
				return false;
		}
}

bool Selector::dispatch_write(const NetHandle &nhdl)
{
		Lock lock(m_mutex);

		HandlerMap::iterator it = m_map.find(nhdl);

		if(it != m_map.end() && (it->second->Mask() & OUTPUT_MASK))
		{
				it->second->OnOutput();
				return true;
		}else
		{
				return false;
		}

}

bool Selector::dispatch_expt(const NetHandle  &nhdl)
{
		Lock lock(m_mutex);

		HandlerMap::iterator it = m_map.find(nhdl);

		if(it != m_map.end() && (it->second->Mask() & EXPT_MASK))
		{
				it->second->OnException();
				return true;
		}else
		{
				return false;
		}

}


void Selector::on_timer()
{
		//�������֮����Ա�֤�����߳��в����޸�m_map��(register or remove)
		Lock lock(m_mutex); 
		

		std::vector<NetHandle> v_hdl;//�Ȱѻᱻ���ɵ�handler�ľ��push������

		v_hdl.reserve(m_map.size());

		for(HandlerMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
		{
				if(it->second->Mask() & TIMER_MASK)
				{
						v_hdl.push_back(it->first);
				}
		}
		
		//֮��������������Ϊ�����ĳ��handler��OnTimer������regiser or remove����
		//ĳ��event_handler������������)����ô����Ͳ�����ִ�����
		for(size_t idx = 0; idx < v_hdl.size(); ++idx)
		{
				HandlerMap::iterator it = m_map.find(v_hdl[idx]);

				if(it != m_map.end() && (it->second->Mask() & TIMER_MASK))
				{
						it->second->OnTimer();
				}
		}
}

void Selector::Stop()
{
		m_is_running = false;

}


bool Selector::RegisterHandler(NetEventHandler *handler)
{
		assert( handler != 0);
		assert((handler->Mask() & ALL_EVENT_MASK) != 0);
		assert(handler->m_selector == 0);
		assert(handler->GetHandle() != NET_INVALID_SOCKET);
		
		Lock lock(m_mutex);//����Ҫ������Ϊ��ֹ�����̶߳�m_map���޸�

		HandlerMap::iterator it = m_map.find(handler->GetHandle());

		if(it == m_map.end())
		{
				handler->m_selector = this;
				handler->Duplicate();
				m_map[handler->GetHandle()] = handler;
				return true;
		}else
		{
				return false;
		}
}




bool Selector::RemoveHandler(NetEventHandler *handler)
{
		assert(handler != 0);

		Lock lock(m_mutex);//����Ҫ������Ϊ��ֹ�����̶߳�m_map���޸�

		HandlerMap::iterator it = m_map.find(handler->GetHandle());

		if(it != m_map.end())
		{
				NetEventHandler *phdl = it->second;
				m_map.erase(it);
				//����release����,������ü���Ϊ1�������֮���handler��delete����
				phdl->m_selector = 0;
				phdl->Release();
				return true;
		}else
		{
				return false;
		}

}

size_t Selector::Count()const
{
		Lock lock(m_mutex);//����Ҫ������Ϊ��ֹ�����̶߳�m_map���޸�
		return m_map.size();

}


}

