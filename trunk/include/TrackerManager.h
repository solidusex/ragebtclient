/*
* The Rage Library
* Copyright (c) 2007 by Solidus
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* provided that the above copyright notice appear in all copies and
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*
*/
#pragma once

#include "TypeDef.h"
#include "Setting.h"
#include "Service.h"
#include "Sha1Hash.h"
#include "Tracker.h"
#include "URL.h"
namespace Rage{


class PeerManager;


typedef std::vector<std::pair<std::string, std::string> > TrackerStatus;

class  TrackerManager : NonCopyable
{
private:
		typedef std::map<std::string, Tracker*>		TrackerMap;
private:
		TrackerMap				m_tracker_map;
private:
		ServiceManager			&m_serv_manager;
		PeerManager				&m_peer_manager;
public:
		TrackerManager(ServiceManager &serv_manager, PeerManager &peer_manager);

		~TrackerManager();
public:
		bool InsertTrackerUrl(const std::string &url);
		bool RemoveTrackerUrl(const std::string &url);
		void SecondTick(const RequestInfo &req_info);
		void Clear();

		std::string GetTrackerStatus(const std::string &url)const;
		
		TrackerStatus GetTrackerStatus()const;
		
};








}