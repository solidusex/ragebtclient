#include "Downloader.h"
#include "Storage.h"
#include "PeerManager.h"
#include "TrackerManager.h"

namespace Rage {



void Downloader::ReLoadSetting()
{
		if(!IsOpen())
		{
				assert(false);
		}

		if(m_state == DS_RUNNING)
		{
				MsgSpace::PostMessageEx(m_task_id, new ReLoadSettingEvent(), 1);
		}
}

void Downloader::AddPeers(const std::list<PeerEntry> &peers)
{
		if(peers.empty())return;

		if(!IsOpen())
		{
				assert(false);
		}

		if(m_state == DS_RUNNING)
		{
				AddPeersUserEvent *addpeers = new AddPeersUserEvent();

				for(std::list<PeerEntry>::const_iterator cit = peers.begin(); cit != peers.end(); ++cit)
				{
						addpeers->InsertPeer(*cit);
				}

				MsgSpace::PostMessageEx(m_task_id, addpeers, 1);
		}

		
}


bool Downloader::StartDownload()
{
		if(!IsOpen())
		{
				assert(false);
				return false;
		}

		if(m_state != DS_STOP)
		{
				assert(false);
				return false;
		}
		
		m_down_status.Clear();
		
		m_state = DS_RUNNING;

		m_work_thd = new ThreadT(this, &Downloader::download_thread);
		
		m_barrier.Acquire();

		return true;
}

void Downloader::StopDownload()
{
		if(!IsOpen())
		{
				assert(false);
		}

		if(m_state != DS_RUNNING && m_state != DS_DOWN_ABORT)
		{
				assert(false);
		}
		
		m_state = DS_STOP;
		assert(m_work_thd != NULL);
		m_work_thd->Join();
		delete m_work_thd;
		m_work_thd = 0;
}

bool Downloader::StartHashChecing()
{
		if(!IsOpen())
		{
				assert(false);
				return false;
		}
		
		if(m_state != DS_STOP)
		{
				assert(false);
				return false;
		}
		
		m_state = DS_HASH_CHECKING;

		m_work_thd = new ThreadT(this, &Downloader::hash_thread);
		m_barrier.Acquire();
		
		return true;
}

void Downloader::StopHashChecking()
{

		if(!IsOpen())
		{
				assert(false);
		}

		if(m_state != DS_HASH_CHECKING && m_state != DS_HASH_ABORT)
		{
				assert(false);
		}
		m_state = DS_STOP;
		assert(m_work_thd != NULL);
		m_work_thd->Join();
		delete m_work_thd;
		m_work_thd = 0;
		m_hash_status.finished_degree = 0.0f;
		m_hash_status.is_done = false;
}





Downloader::Downloader(		const GlobalSetting &global_setting,	
						ServiceManager  &serv_manager,
						ResourceManager &res_manager
					) : m_serv_manager(serv_manager)
					  , m_res_manager(res_manager)
					  , m_global_setting(global_setting)
					  , m_work_thd(0)
					  , m_state(DS_STOP)
					  , m_task_id(INVALID_TASK_ID)
					  , m_is_open(false)
					  , m_bitfield(1)
					  ,m_save_path(global_setting.GetDefaultSavePath())
					  ,m_barrier(2)
{
		m_down_status.Clear();


}

Downloader::~Downloader()
{
		try{
				
				if(IsOpen())
				{
						switch(GetDownloaderState())
						{
						case DS_RUNNING:
						case DS_DOWN_ABORT:
								StopDownload();
								break;
						case DS_HASH_CHECKING:
						case DS_HASH_ABORT:
								StopHashChecking();
								break;
						}

						Close();
				}
		}catch(...)
		{
				DEBUG_PRINT0("UnKnow exception throw : Downloader::~Downloader()\n");
		}
		
}


bool Downloader::Open(const std::wstring &torrent_path, const std::wstring &save_path)
{
		if(m_is_open) //如果已经打开了，则此为编程错误
		{
				assert(false);
				return false;
		}

		using namespace FileSysSpace;
		if(Path(m_save_path).GetPathType() != PATH_DIR || !m_torrent_file.Load(torrent_path))
		{
				DEBUG_PRINT1("%s\n", "save_path is invalid");
				return false;
		}

		if(!m_torrent_file.Load(torrent_path))
		{
				DEBUG_PRINT1("%s\n", "torrent file is invalid");
				return false;
		}
		
		m_save_path = save_path;
		m_bitfield.Reset(m_torrent_file.GetPieceCount());

		m_is_open = true;
		return true;
}

bool Downloader::SetOwnPieces(const BitField &bitfield)
{
		if(!m_is_open)
		{
				assert(false);
				return false;
		}
		
		if(m_state != DS_STOP)
		{
				assert(false);
				return false;
		}

		if(bitfield.NBits() != m_torrent_file.GetPieceCount())
		{
				assert(false);
				return false;
		}
		assert(m_bitfield.NBits() == bitfield.NBits());

		m_bitfield = bitfield;
		return true;
}

void Downloader::Close()
{
		if(!m_is_open)
		{
				assert(false);
				return;
		}

		if(m_state != DS_STOP)
		{
				assert(false);
				return;
		}
		
		m_save_path = m_global_setting.GetDefaultSavePath();
		m_torrent_file.Clear();
		m_bitfield.Reset(1);
		m_torrent_file.Clear();
		m_down_status.Clear();
		m_is_open = false;
}


bool Downloader::ResetSavePath(const std::wstring &save_path)
{
		if(!m_is_open || m_state != DS_STOP)
		{
				assert(false);
				return false;
		}
		using namespace FileSysSpace;		
		if(Path(m_save_path).GetPathType() != PATH_DIR) return false;

		m_save_path = save_path;
		return true;
}

bool Downloader::IsOpen()const
{
		return m_is_open;
}



float Downloader::GetFinishedDegree()const
{
		assert(IsOpen());
		

		Rage::BitField bitf = GetBitField();
		size_t finished = 0;
		for(size_t i = 0; i < bitf.NBits(); ++i)
		{
				if(bitf.IsSet(i))
				{
						finished++;
				}
		}
		return ((float)finished / (float)bitf.NBits());
}





DownStatus Downloader::GetDownStatus()const
{
		LockT lock(m_status_mutex);
		assert(IsOpen() && (m_state == DS_RUNNING || m_state == DS_DOWN_ABORT));
		
		return m_down_status;
}


HashingStatus Downloader::GetHashStatus()const
{
		assert(IsOpen() && (m_state == DS_HASH_CHECKING || m_state == DS_HASH_ABORT));
		LockT lock(m_status_mutex);
		return m_hash_status;
}


DownloaderState Downloader::GetDownloaderState()const
{
		return m_state;
}



BitField Downloader::GetBitField()const
{
		LockT lock(m_status_mutex);
		assert(IsOpen());
		return m_bitfield;

}
		
FileStatusList Downloader::GetFilesStatus()const
{
		assert(IsOpen());
		
		BitField tmp(m_torrent_file.GetPieceCount());
		{
				LockT lock(m_status_mutex);
				tmp = m_bitfield;
		}

		size_t nfiles = m_torrent_file.GetFileCount();

		FileStatusList files_status;

		files_status.reserve(nfiles);

		for(size_t idx = 0; idx < nfiles; ++idx)
		{
				t_uint32 beg = 0, end = 0;
				bool res = m_torrent_file.MapFileToPieces(idx, beg, end);
				assert(res);
				
				size_t own_count = 0;

				for(size_t i = beg; i < end + 1; ++i)
				{
						if(tmp.IsSet(i))
						{
								own_count++;
						}
				}
				
				FileStatus fstatus = { idx, (float)own_count / ((float)(end + 1 - beg)) };
				
				files_status.push_back(fstatus);
		}
		
		return files_status;
}


namespace {

PeerID generate_peer_id()
{
		t_byte buf[20];
		memcpy(buf, "-RG0001-", 8);
		*((t_uint64*)(buf + 8)) = TimeSpace::CycleTimer::GetCycleCount();
		return PeerID(buf);
}

class TimerFunc
{
private:
		const TaskID			m_task_id;
		TimerService			&m_timer_service;
		TimerT					m_timer;
public:
		TimerFunc(const TaskID &task_id, TimerService	&timer_service) 
				: m_task_id(task_id)
				, m_timer_service(timer_service)
				, m_timer(this, &TimerFunc::OnTimer, 1000)
		{

				if(!m_timer_service.RegisterTimer(&m_timer))
				{
						throw ExceptionSpace::RuntimeException("TimerFunc::m_timer_service.RegisterTimer(&m_timer) failed");
				}
		}

		~TimerFunc()
		{
				try{
						m_timer_service.RemoveTimer(&m_timer);

				}catch(...)
				{
						DEBUG_PRINT0("UnKnow Exception throw from ~TimerFunc()\n");
				}

		}

public:
		void OnTimer()
		{
				MsgSpace::PostMessageEx(m_task_id, new TimerEvent(), 10);
		}

};

}



void Downloader::download_thread()
{
		assert(m_state == DS_RUNNING && IsOpen());
		
		
		bool is_initialized = false; //这个标志取决于有异常发生时如何在cache中处理
		try{
				using namespace ExceptionSpace;

				m_task_id = ThreadT::GetCurrentThreadId();
				const PeerID peer_id = generate_peer_id();	
				const Sha1Hash infohash = m_torrent_file.GetInfoHash();
				TimeSpace::TimeStamp	elapsed_ts;
				
				
				Storage storage(m_save_path, m_torrent_file);
				
				if(!storage.Open())
				{
						throw FileException("Storage init failed\n");
				}

				storage.SetCacheSize(m_global_setting.GetMaxCacheSize());


				PieceManager piece_manager(m_bitfield, m_torrent_file, storage);
				piece_manager.ResetPiecesInfo();



				PeerManager peer_manager(m_serv_manager, 
						m_res_manager, 
						m_global_setting, 
						piece_manager, 
						m_task_id,
						peer_id, 
						infohash
						);


				TrackerManager tracker_manager(m_serv_manager, peer_manager);

				for(size_t i = 0; i < m_torrent_file.GetAnnounceCount(); ++i)
				{
						std::string url = m_torrent_file.GetAnnounce(i);

						if(!tracker_manager.InsertTrackerUrl(url))
						{
								DEBUG_PRINT1("url == %s : can't insert to tracker manger\n", url.c_str());
						}
				}

				if(!m_res_manager.GetDownloaderRecorder().RegisterRecord(infohash, peer_id, m_task_id))
				{
						throw RuntimeException("m_res_manager.GetDownloaderRecorder().RegisterRecord failed");
				}

				TimerFunc timer_func(m_task_id, m_serv_manager.GetTimerService());

				
				
				m_down_status.Clear();
				is_initialized = true;
				m_barrier.Acquire();


				MsgSpace::MsgContext context;
				NotifyEvent *pevent = 0;

				while(m_state == DS_RUNNING)
				{
						pevent = (NotifyEvent*)context.GetMessageEx();
						switch(pevent->GetType())
						{
						case NET_EVENT:
						{
								peer_manager.OnNetEvent((NetEvent*)pevent);
								break;
						}
						case TIMER_EVENT:
						{
								peer_manager.SecondTick();

								const Statistics &ref_stat = peer_manager.GetTaskStatistics();

								/*************************************************************/

								RequestInfo req_info;
								req_info.compact = 1;
								req_info.downloaded = piece_manager.GetFinishedSize();
								req_info.event = TS_NONE;
								req_info.info_hash = infohash;
								req_info.key = 0;
								req_info.left = piece_manager.GetLeftSize();
								req_info.listen_port = m_global_setting.GetListenPort();
								req_info.numwant = 999;
								req_info.peer_id = peer_id;
								req_info.uploaded = ref_stat.TotalPayloadUpload();
								tracker_manager.SecondTick(req_info);

								/****************************************************************/


								LockT lock(m_status_mutex);////////////////////////////
								
								m_bitfield = piece_manager.GetBitField();
								m_down_status.connected_peers = peer_manager.GetConnectedPeers();
								m_down_status.connecting_peers = peer_manager.GetConnectingPeers();
								m_down_status.total_down = ref_stat.TotalPayloadDownload();
								m_down_status.total_up = ref_stat.TotalPayloadUpload();
								m_down_status.finished_degree = piece_manager.GetFinishedDegree();

								m_down_status.down_rate = ref_stat.DownloadPayloadRate();
								m_down_status.up_rate = ref_stat.UploadPayloadRate();
								m_down_status.num_unused_peer = peer_manager.GetUnusedPeersCount();
								m_down_status.num_peer = peer_manager.GetPeerCount();
								m_down_status.num_seed = peer_manager.GetSeedCount();
								m_down_status.peers_status = peer_manager.GetPeersStatus();
								m_down_status.trackers_status = tracker_manager.GetTrackerStatus();
								m_down_status.elapsed_time = elapsed_ts.ElapsedSecond();
								
								t_uint64 down_rate = ref_stat.DownloadPayloadRate();
								if(down_rate == 0)
								{
										m_down_status.remain_time = ~0;
								}else
								{
										m_down_status.remain_time = piece_manager.GetLeftSize() / down_rate;
								}

								break;
						}
						case USER_EVENT:
						{
								DEBUG_PRINT0("USER_EVENT\n");
								UserEvent *puser_event = (UserEvent*)pevent;

								switch(puser_event->GetUserEventType())
								{
								case WakeUpEvent:
								{
										break;
								}
								case RELOAD_SETTING:
								{
												//////////////////
												storage.SetCacheSize(m_global_setting.GetMaxCacheSize());
												///////////
												break;
								}
								case ADD_PEERS:
								{
										const std::list<PeerEntry>& peers = ((AddPeersUserEvent*)pevent)->GetPeers();

										for(std::list<PeerEntry>::const_iterator cit = peers.begin(); cit != peers.end(); ++cit)
										{
												peer_manager.AddUnusedPeer(*cit);
										}

										break;
								}
								default:
								{
										assert(0);
										break;

								}
						}
								break;
						}
						default:
								assert(0);
						}
				}

		}catch(std::exception &expt)
		{
				DEBUG_PRINT1("%s\n", expt.what());

				m_state = DS_DOWN_ABORT;
				if(!is_initialized)
				{
						m_barrier.Acquire();
				}
		}

		m_res_manager.GetDownloaderRecorder().RemoveRecord(this->GetTorrentFile().GetInfoHash());
		
}



void Downloader::hash_thread()
{
		assert(m_state == DS_HASH_CHECKING && IsOpen());
		
		bool is_initialized = false; //这个标志取决于有异常发生时如何在cache中处理
		try{
				using namespace ExceptionSpace;
				
				DEBUG_PRINT0("Begin Hash Checking\n");

				Storage storage(m_save_path, m_torrent_file);
				
				if(!storage.Open())
				{
						throw FileException("Storage init failed\n");
				}

				m_hash_status.finished_degree = 0.0f;
				m_hash_status.is_done = false;
				
				is_initialized = true;
				m_barrier.Acquire();

				size_t pieces_count = m_torrent_file.GetPieceCount();

				for(size_t piece_idx = 0; m_state == DS_HASH_CHECKING && piece_idx < pieces_count; ++piece_idx)
				{
						DEBUG_PRINT1("hash idx == %d\n", piece_idx);
						if(storage.CheckPiece(piece_idx))
						{
								m_bitfield.Set(piece_idx);
						}else
						{
								m_bitfield.UnSet(piece_idx);
						}
						ThreadT::Sleep(20);
						LockT lock(m_status_mutex);
						m_hash_status.finished_degree = ((float)piece_idx + 1) / pieces_count;
				}

				LockT lock(m_status_mutex);
				m_hash_status.is_done = true;

		}catch(std::exception &expt)
		{
				DEBUG_PRINT1("%s\n", expt.what());
				m_state = DS_HASH_ABORT;
				if(!is_initialized)
				{
						m_barrier.Acquire();
				}
		}
}



const std::wstring& Downloader::GetSavePath()const
{
		assert(IsOpen());
		return m_save_path;
}




const TorrentFile& Downloader::GetTorrentFile()const
{
		assert(IsOpen());
		return m_torrent_file;
}

}