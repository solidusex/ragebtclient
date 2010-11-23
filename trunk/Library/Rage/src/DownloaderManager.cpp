
/*
 * The Rage Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  It is provided "as is" without express
 * or implied warranty.
 */

#include "DownloaderManager.h"



namespace Rage{







DownloaderManager::DownloaderManager() 
		: m_is_initialized(false)
		, m_serv_manager(0)
		, m_res_manager(0)
		, m_global_setting(0)
		, m_acceptor(0)
		, m_resume(0)
		, m_torrent_save_path(L"")
{

}


DownloaderManager::~DownloaderManager()
{
		try{
				if(m_is_initialized)
				{
						UnInit();
				}
		}catch(...)
		{
				DEBUG_PRINT0("UnKnow Exception : DownloaderManager::~DownloaderManager()\n");
		}
}

GlobalSetting& DownloaderManager::GetGlobalSetting()
{
		assert(m_is_initialized);

		return *m_global_setting;

}

void DownloaderManager::SaveResumeInfo()
{
		assert(m_is_initialized);
		ResumeDataUpdate();
		m_resume->Save();
}



bool DownloaderManager::Init(const std::wstring &resume_save_path, const std::wstring config_save_path, const std::wstring &torrent_save_path)
{
		if(m_is_initialized)
		{
				assert(0);
				return false;
		}


		try{
				using namespace FileSysSpace;

				Path path(torrent_save_path);
				if(path.GetPathType() != PATH_DIR)
				{
						return false;
				}
				
				if(!path.IsExisted())
				{
						path.Make();
				}

				m_torrent_save_path = torrent_save_path;

				path.Reset(resume_save_path);
				if(path.GetPathType() != PATH_FILE)
				{
						return false;
				}

				path.Reset(config_save_path);
				if(path.GetPathType() != PATH_FILE)
				{
						return false;
				}


				std::auto_ptr<GlobalSetting> p_global_set(new GlobalSetting(config_save_path));
				
				


				std::auto_ptr<ResumeInfo> p_resume(new ResumeInfo(resume_save_path));
				
				std::auto_ptr<ServiceManager>	p_serv_manager(new ServiceManager());
				
				if(!p_serv_manager->Start())
				{
						return false;
				}

				std::auto_ptr<ResourceManager> p_res_manager(new ResourceManager());
				
				std::auto_ptr<PeerAcceptor> p_acceptor(new PeerAcceptor(*p_serv_manager, *p_res_manager));

				if(!p_acceptor->Start(p_global_set->GetListenPort()))
				{
						DEBUG_PRINT0("PeerAcceptor Start failed\n");
						return false;
				}

				m_resume = p_resume.release();
				m_serv_manager =  p_serv_manager.release();
				m_res_manager = p_res_manager.release();
				m_global_setting = p_global_set.release();
				m_acceptor = p_acceptor.release();
		
				std::vector<Sha1Hash> bad_data;
				
				for(ResumeInfo::CIterator cit = m_resume->Begin(); cit != m_resume->End(); ++cit)
				{
						
						std::auto_ptr<Downloader> pdownloader(new Downloader(*m_global_setting, *m_serv_manager, *m_res_manager));
						
						if(!pdownloader->Open(m_torrent_save_path + cit->second.torrent_name, cit->second.save_path))
						{
								DEBUG_PRINT0("downloader open failed : DownloaderManager::Init\n");
								bad_data.push_back(cit->first);
								continue;
						}

						//printf("cit-> %d\n", cit->second.own_pieces.NBits());
						//printf("downloader-> %d\n", pdownloader->GetTorrentFile().GetPieceCount());


						BitField bits = pdownloader->GetBitField();

						if(cit->second.own_pieces.size() != bits.NBytes())
						{
								DEBUG_PRINT0("downloader open failed : DownloaderManager::Init\n");
								bad_data.push_back(cit->first);
								continue;
						}
						bits.Reset(&cit->second.own_pieces[0], bits.NBits());

						pdownloader->SetOwnPieces(bits);
						m_downloader_list.push_back(pdownloader.release());
				}
				
				for(size_t i = 0; i < bad_data.size(); ++i)
				{
						
						m_resume->Remove(bad_data[i]);
				}

				m_is_initialized = true;

		}catch(...)
		{
				DEBUG_PRINT0("DownloaderManager::Init failed\n");
				return false;
		}
		return true;
}


bool DownloaderManager::ResetListenPort()
{
		assert(m_is_initialized);
		assert(m_acceptor != NULL);
		assert(m_global_setting != NULL);

		t_uint16 cur_port = m_global_setting->GetListenPort();

		if(cur_port == m_acceptor->GetListenPort()) return true;


		std::auto_ptr<PeerAcceptor> p_acceptor(new PeerAcceptor(*m_serv_manager, *m_res_manager));

		if(!p_acceptor->Start(cur_port))
		{
				DEBUG_PRINT0("PeerAcceptor Start failed\n");
				return false;
		}else
		{
				m_acceptor->Stop();
				delete m_acceptor;
				m_acceptor = p_acceptor.release();
				return true;
		}
}

t_uint16 DownloaderManager::GetCurrListenPort()const
{
		assert(m_is_initialized);
		assert(m_acceptor != NULL);

		return m_acceptor->GetListenPort();

}

std::wstring DownloaderManager::torrent_to_save_name(const TorrentFile &tf)
{
		assert(m_is_initialized);

		return tf.GetTorrentName() + L".torrent";

		
}


void DownloaderManager::ResumeDataUpdate()
{
		assert(m_is_initialized);
		for(DownloaderList::iterator  it = m_downloader_list.begin(); it != m_downloader_list.end(); ++it)
		{
				Downloader *pd = (*it);
				assert(pd->IsOpen());
				
				ResumeTaskInfo rti;
				rti.infohash = pd->GetTorrentFile().GetInfoHash();
				rti.own_pieces.resize(pd->GetBitField().NBytes());
				memcpy(&rti.own_pieces[0], pd->GetBitField().GetContent(), pd->GetBitField().NBytes());
				rti.save_path = pd->GetSavePath();
				rti.torrent_name =torrent_to_save_name(pd->GetTorrentFile());
				m_resume->Update(rti);
		}
}



void DownloaderManager::UnInit()
{
		if(!m_is_initialized)
		{
				assert(0);
				return;
		}
		
		m_global_setting->Save();
		SaveResumeInfo();

		
		
				
		for(DownloaderList::iterator  it = m_downloader_list.begin(); it != m_downloader_list.end(); ++it)
		{
				Downloader *pd = (*it);
				assert(pd->IsOpen());
				
				switch(pd->GetDownloaderState())
				{
				case DS_RUNNING:
				case DS_DOWN_ABORT:
						pd->StopDownload();
						break;
				case DS_HASH_CHECKING:
				case DS_HASH_ABORT:
						pd->StopHashChecking();
						break;
				}
				pd->Close();

				delete pd;
		}

		m_acceptor->Stop();
		delete m_acceptor;
		m_acceptor = 0;
		m_serv_manager->Stop();
		delete m_serv_manager;
		m_serv_manager = 0;
		delete m_res_manager;
		m_res_manager = 0;
		delete m_global_setting;
		m_global_setting = 0;
		delete m_resume;
		m_resume = 0;
		m_downloader_list.clear();

		m_is_initialized = false;
}

const DownloaderList& DownloaderManager::GetDownloaderList()
{
		assert(m_is_initialized);
		return m_downloader_list;
}

Downloader* DownloaderManager::AddTask(const std::wstring &torrent_path)
{
		if(!m_is_initialized)
		{
				assert(0);
				return false;
		}

		if(m_global_setting->GetDefaultSavePath() == L"") return NULL;
		
		std::auto_ptr<Downloader> pd(new Downloader(*m_global_setting, *m_serv_manager, *m_res_manager));
		
		if(!pd->Open(torrent_path, m_global_setting->GetDefaultSavePath()))
		{
				DEBUG_PRINT0("pd->Open failed\n");
				return NULL;
		}


		using namespace FileSysSpace;
		
		Path(m_torrent_save_path).Make();
		
		const TorrentFile &tf = pd->GetTorrentFile();

		if(m_resume->IsExisted(tf.GetInfoHash())) return NULL;

		
		std::wstring torrent_save_path = m_torrent_save_path + torrent_to_save_name(tf);
		

		
		Path path(torrent_save_path);

		if(path.IsExisted() || !File::FileCopyTo(torrent_path, torrent_save_path))
		{
				return NULL;
		}
		
		std::vector<t_byte> tmp_bits(pd->GetBitField().NBytes());
		memcpy(&tmp_bits[0], pd->GetBitField().GetContent(), tmp_bits.size());
		ResumeTaskInfo rti = { tf.GetInfoHash(), tmp_bits, pd->GetSavePath(), torrent_to_save_name(tf)};
		
		m_resume->Add(rti);
		m_downloader_list.push_back(pd.get());
		return pd.release();
}



bool DownloaderManager::RemoveTask(const Sha1Hash &infohash)
{
		if(!m_is_initialized)
		{
				assert(0);
				return false;
		}
		
		Downloader *pd = NULL;

		for(DownloaderList::iterator  it = m_downloader_list.begin(); it != m_downloader_list.end(); ++it)
		{
				assert((*it)->IsOpen());
				if((*it)->GetTorrentFile().GetInfoHash() == infohash)
				{
						pd = (*it);
						m_downloader_list.erase(it);
						break;
				}
		}

		if(pd == NULL) return false;


		switch(pd->GetDownloaderState())
		{
		case DS_RUNNING:
		case DS_DOWN_ABORT:
				pd->StopDownload();
				break;
		case DS_HASH_CHECKING:
		case DS_HASH_ABORT:
				pd->StopHashChecking();
				break;
		}
		

		const TorrentFile &tf = pd->GetTorrentFile();

		m_resume->Remove(tf.GetInfoHash());

		

		using namespace FileSysSpace;
		

		Path path(m_torrent_save_path + torrent_to_save_name(tf));

		if(path.IsExisted())
		{
				path.Remove();
		}

		delete pd;

		m_resume->Save();

		return true;

}

bool DownloaderManager::CopyTorrentFileTo(const std::wstring &tp, const TorrentFile &tf)
{
		using namespace FileSysSpace;

		Path path(tp);
		if(path.GetPathType() != PATH_DIR) return false;

		return File::FileCopyTo(m_torrent_save_path + torrent_to_save_name(tf), tp + torrent_to_save_name(tf));
}


void DownloaderManager::ClearIPFilter()
{
		assert(m_is_initialized);

		assert(m_res_manager != NULL);

		m_res_manager->GetIPFilter().Clear();



}


bool DownloaderManager::IsExisted(const Sha1Hash &infohash)
{
		for(DownloaderList::iterator  it = m_downloader_list.begin(); it != m_downloader_list.end(); ++it)
		{
				Downloader *pd = (*it);
				
				assert(pd->IsOpen());

				const TorrentFile &tf = pd->GetTorrentFile();

				if(tf.GetInfoHash() == infohash)
				{
						return true;
				}
		}
		return false;



}


void DownloaderManager::ReloadSetting()
{
		for(DownloaderList::iterator  it = m_downloader_list.begin(); it != m_downloader_list.end(); ++it)
		{
				Downloader *pd = (*it);
				
				assert(pd->IsOpen());

				if(pd->GetDownloaderState() == DS_RUNNING)
				{
						pd->ReLoadSetting();
				}
		}
}








}