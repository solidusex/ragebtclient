#pragma once

#include "TypeDef.h"
#include "Res.h"
#include "Service.h"
//#include "NotifyEvent.h"



namespace Rage {

typedef NetSpace::AcceptorEx			AcceptorHandler;

class RageAPI PeerAcceptor : private NonCopyable
{
private:
		AcceptorHandler			*m_handler;
		t_uint16				m_listen_port;
		bool					m_is_started;
private:
		ServiceManager			&m_serv_manager;
		ResourceManager			&m_res_manager;
public:
		PeerAcceptor(ServiceManager	&serv, ResourceManager &res_manager);
		
		~PeerAcceptor();
public:
		bool Start(t_uint16 port);

		void Stop();

		bool IsStarted()const;
		
		t_uint16 GetListenPort()const;
};






















}