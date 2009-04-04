#pragma once


#include "TypeDef.h"
#include "PeerEntry.h"
#include "PeerLink.h"
#include "TrackerManager.h"
#include "Res.h"
#include "Service.h"
#include "NotifyEvent.h"

namespace Rage {


enum DownloaderState
{
		DS_RUNNING = 0x600A,
		DS_STOP	   = 0x600B,
		DS_HASH_CHECKING = 0x600C,
		DS_DOWN_ABORT = 0x600D,
		DS_HASH_ABORT = 0x600E
};




struct DownStatus
{
		t_uint64		total_down;
		t_uint64		total_up;
		float			down_rate;
		float			up_rate;
		float			finished_degree;
		PeerList		connected_peers;
		PeerList		connecting_peers;
		t_uint32		num_peer;
		t_uint32		num_seed;
		t_uint32		num_unused_peer;
		t_uint64		elapsed_time;
		t_uint64		remain_time;
		
		std::vector<PeerStatus> peers_status;
		TrackerStatus			trackers_status;
		void Clear()
		{
				total_down = 0;
				total_up = 0;
				down_rate = 0.0f;
				up_rate = 0.0f;
				finished_degree = 0.0f;
				connected_peers.clear();
				connecting_peers.clear();
				num_peer = 0;
				num_seed = 0;
				num_unused_peer = 0;
				elapsed_time = 0;
				remain_time = 0;
				
				peers_status.clear();
				trackers_status.clear();

		}
};

struct FileStatus
{
		t_uint32 fidx;
		float	 finished_degree;
};

typedef std::vector<FileStatus> FileStatusList;


struct HashingStatus
{
		bool is_done;
		float finished_degree;
};



class RageAPI Downloader : public NonCopyable
{
private:
		typedef ThreadSpace::Thread		ThreadT;
		typedef ThreadSpace::Barrier	BarrierT;

		typedef ThreadSpace::FastMutex	MutexT;
		typedef MutexT::ScopeLock		LockT;
		
private:
		ServiceManager			&m_serv_manager;
		ResourceManager			&m_res_manager;
		const GlobalSetting		&m_global_setting;

private:
		ThreadT					*m_work_thd;
		
		BarrierT				m_barrier;
		TaskID					m_task_id;//StartDownloading之后才会有效，其他任何时间都为INVALID_TASK_ID
private:
		bool					m_is_open;

		mutable MutexT			m_status_mutex;
		BitField				m_bitfield;
		std::wstring			m_save_path;
		TorrentFile				m_torrent_file;
		
		DownloaderState			m_state;
		DownStatus				m_down_status;
		HashingStatus			m_hash_status;
private:
		void download_thread();

		void hash_thread();
public:
		Downloader(		const GlobalSetting &global_setting,	
						ServiceManager  &serv_manager,
						ResourceManager &res_manager
				  );

		~Downloader();
public:
		DownStatus GetDownStatus()const;
		
		HashingStatus GetHashStatus()const;

		DownloaderState GetDownloaderState()const;

		BitField GetBitField()const;
		
		float GetFinishedDegree()const;
		
		FileStatusList GetFilesStatus()const;

public:
		void ReLoadSetting();

		void AddPeers(const PeerList &peers);

		const TorrentFile& GetTorrentFile()const;

		const std::wstring& GetSavePath()const;
public:
		bool StartDownload();

		void StopDownload();

		bool StartHashChecing();

		void StopHashChecking();
public:
		bool Open(const std::wstring &torrent_path, const std::wstring &save_path);
		
		bool SetOwnPieces(const BitField &bitfield);

		bool ResetSavePath(const std::wstring &save_path);

		bool IsOpen()const;

		void Close();

};

/*

std::string Downloader::GetDownloaderStateStr()const
{
		LockT lock(m_status_mutex);
		assert(IsOpen() && m_state == DS_RUNNING);

		return 
}
*/













}