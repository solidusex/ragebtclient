#include "Downloader.h"
#include "Resume.h"
#include "PeerAcceptor.h"

namespace Rage{



typedef std::list<Downloader*> DownloaderList;


class RageAPI DownloaderManager : private NonCopyable
{
private:
		ServiceManager			*m_serv_manager;
		ResourceManager			*m_res_manager;
		GlobalSetting			*m_global_setting;
		PeerAcceptor			*m_acceptor;
public:
		ResumeInfo				*m_resume;
private:
		DownloaderList			m_downloader_list;
		bool					m_is_initialized;
		std::wstring			m_torrent_save_path;
private:
		std::wstring torrent_to_save_name(const TorrentFile &tf);

public:
		bool Init(const std::wstring &resume_save_path, const std::wstring config_save_path, const std::wstring &torrent_save_path);
		void UnInit();

		bool ResetListenPort();

		t_uint16 GetCurrListenPort()const;
		
		const DownloaderList& GetDownloaderList();
		
		bool IsExisted(const Sha1Hash &infohash);

		Downloader* AddTask(const std::wstring &torrent_path);

		bool RemoveTask(const Sha1Hash &infohash);

		void ResumeDataUpdate();

		bool CopyTorrentFileTo(const std::wstring &tp, const TorrentFile &tf);

		void ClearIPFilter();

		void ReloadSetting();
public:
		GlobalSetting& GetGlobalSetting();

		void SaveResumeInfo();
public:
		DownloaderManager();
		~DownloaderManager();
};







}