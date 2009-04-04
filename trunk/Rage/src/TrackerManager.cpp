#include "TrackerManager.h"
#include "PeerManager.h"

namespace Rage {


TrackerManager::TrackerManager(ServiceManager &serv_manager, PeerManager &peer_manager)
				: m_serv_manager(serv_manager)
				, m_peer_manager(peer_manager)
{


}



TrackerManager::~TrackerManager()
{

		Clear();
}

void TrackerManager::Clear()
{
		for(TrackerMap::iterator it = m_tracker_map.begin(); it != m_tracker_map.end(); ++it)
		{
				delete it->second;

		}
		m_tracker_map.clear();
}

bool TrackerManager::InsertTrackerUrl(const std::string &url)
{
		UrlParser upr;

		if(!upr.Parse(url)) return false;

		if(m_tracker_map.count(url) == 1)return false;

		Tracker *pt = new Tracker(url, m_serv_manager, m_peer_manager);

		m_tracker_map[url] = pt;
		return true;
}

bool TrackerManager::RemoveTrackerUrl(const std::string &url)
{
		TrackerMap::iterator it = m_tracker_map.find(url);
		if(it == m_tracker_map.end()) return false;
		
		delete it->second;
		m_tracker_map.erase(it);
		return true;
}



void TrackerManager::SecondTick(const RequestInfo &req_info)
{
		for(TrackerMap::iterator it = m_tracker_map.begin(); it != m_tracker_map.end(); ++it)
		{
				it->second->SecondTick(req_info);

		}
}


std::string TrackerManager::GetTrackerStatus(const std::string &url)const
{
		TrackerMap::const_iterator it = m_tracker_map.find(url);
		if(it == m_tracker_map.end()) return "";
		return it->second->GetTrackerStatus();
}



TrackerStatus TrackerManager::GetTrackerStatus()const
{
		TrackerStatus res(m_tracker_map.size());

		for(TrackerMap::const_iterator it = m_tracker_map.begin(); it != m_tracker_map.end(); ++it)
		{
				res.push_back(std::make_pair(it->first, it->second->GetTrackerStatus()));
		}
		
		return res;

}


}
