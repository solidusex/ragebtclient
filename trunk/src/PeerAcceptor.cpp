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
 */#include "PeerAcceptor.h"
#include "ConnValidator.h"

namespace Rage {


class AcceptorHandlerImpl : public AcceptorHandler
{
private:
		ResourceManager			&m_res_manager;
		
protected:
		virtual void on_accepted(NetSpace::NetHandle hdl)
		{
				using namespace NetSpace;

				NetSpace::SockStream	sock_ss;
				InetAddress addr;
				sock_ss.Attach(hdl);
				sock_ss.GetRemoteAddr(addr);
				
				PeerEntry pe(addr.IP(), addr.Port());

				if(m_res_manager.GetIPFilter().IsBand(pe))
				{
						DEBUG_PRINT2("Accepted ip == %s:%d in band list\n", addr.Str().c_str(), addr.Port());
				}else
				{
						DEBUG_PRINT2("Accepted ip == %s:%d\n", addr.Str().c_str(), addr.Port());

						
						InConnValidator *picv = new InConnValidator(m_res_manager, sock_ss.Attach(NET_INVALID_SOCKET));
						GetSelector()->RegisterHandler(picv);
						picv->Release();
				}
		}

		virtual void on_error(t_ulong err_code)
		{
				DEBUG_PRINT1("AcceptorHandlerImpl::on_error error code == %d\n", err_code);
				exit(-1);
		}

public:
		AcceptorHandlerImpl(t_uint16 port, ResourceManager &res_manager) 
				: AcceptorHandler(port)
				, m_res_manager(res_manager)
		{
				DEBUG_PRINT0("AcceptorHandlerImpl(t_uint16 port, ResourceManager &res_manager)\n");
				assert(GetSelector() == 0);
		}

		virtual ~AcceptorHandlerImpl()
		{
				DEBUG_PRINT0("Release ~AcceptorHandlerImpl\n");
		}


};


PeerAcceptor::PeerAcceptor(ServiceManager &serv, ResourceManager &res_manager) 
						: m_serv_manager(serv)
						, m_res_manager(res_manager)
						, m_is_started(false)
						, m_listen_port(0)
						, m_handler(0)
{



}

		
PeerAcceptor::~PeerAcceptor()
{

		if(IsStarted())
		{
				Stop();
		}
}

bool PeerAcceptor::IsStarted()const
{
		return m_is_started;
}

t_uint16 PeerAcceptor::GetListenPort() const
{
		return m_listen_port;
}

bool PeerAcceptor::Start(t_uint16 port)
{
		if(IsStarted()) return false;
		assert(m_serv_manager.GetNetService().IsStarted());
		
		AcceptorHandler *p_acceptor = 0;

		try{
				p_acceptor = new AcceptorHandlerImpl(port, m_res_manager);
				if(!m_serv_manager.GetNetService().RegisterHandler(p_acceptor))
				{
						throw std::runtime_error("m_serv_manager.GetNetService().RegisterHandler failed\n");
				}else
				{
						m_listen_port = p_acceptor->GetLocalAddr().Port();
						m_is_started = true;
						m_handler = p_acceptor;
						assert(m_handler->Count() == 2);
						return true;
				}
		}catch(...)
		{
				delete p_acceptor;
				return false;
		}
}

void PeerAcceptor::Stop()
{
		if(IsStarted())
		{
				assert(m_handler != 0);
				bool res = m_serv_manager.GetNetService().RemoveHandler(m_handler);
				assert(res);
				assert(m_handler->Count() == 1);
				m_handler->Release();
				m_handler = 0;
				m_listen_port = 0;
				m_is_started = false;
		}

}


}