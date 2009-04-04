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