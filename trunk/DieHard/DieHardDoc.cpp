// DieHardDoc.cpp : implementation of the CDieHardDoc class
//

#include "stdafx.h"
#include "Console.h"

#include "DieHard.h"

#include "DieHardDoc.h"

#include "SettingDialog.h"
#include "MainFrm.h"
#include "TaskSetting.h"
#include "Util.h"
#include "FolderDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Rage;

#pragma warning(disable : 4100)


VOID CALLBACK DocTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
		((CDieHardDoc*)(((CMainFrame*)AfxGetMainWnd())->GetActiveDocument()))->SecondTick();
}


void CDieHardDoc::OnOpenConsole()
{
		if(!m_console.IsOpen())
		{
				m_console.Open();
		}else
		{
				m_console.Close();
		}
}

void CDieHardDoc::OnOpenConsoleUpdateUI(CCmdUI *pui)
{
		pui->SetCheck(m_console.IsOpen());

}

// CDieHardDoc

IMPLEMENT_DYNCREATE(CDieHardDoc, CDocument)

BEGIN_MESSAGE_MAP(CDieHardDoc, CDocument)
		
		ON_COMMAND(IDM_FILE_SETTING, &CDieHardDoc::OnGlobalSetting)
		ON_COMMAND(IDT_SETTING, &CDieHardDoc::OnGlobalSetting)

		ON_COMMAND(IDM_FILE_OPENTORRENT, &CDieHardDoc::OnFileOpenTorrent)
		ON_COMMAND(IDT_OPEN_TORRENT, &CDieHardDoc::OnFileOpenTorrent)

		ON_COMMAND(IDT_TASK_SETTING, &CDieHardDoc::OnTaskSetting)
		ON_UPDATE_COMMAND_UI(IDT_TASK_SETTING, &CDieHardDoc::OnTaskSettingUpdateUI)

		ON_COMMAND(IDM_TASK_SETTING,&CDieHardDoc::OnTaskSetting)
		ON_UPDATE_COMMAND_UI(IDM_TASK_SETTING, &CDieHardDoc::OnTaskSettingUpdateUI)

		ON_COMMAND(IDT_REMOVE, &CDieHardDoc::OnTaskRemove)
		ON_UPDATE_COMMAND_UI(IDT_REMOVE, &CDieHardDoc::OnTaskRemoveUpdateUI)
		ON_COMMAND(IDM_TASK_REMOVE, &CDieHardDoc::OnTaskRemove)
		ON_UPDATE_COMMAND_UI(IDM_TASK_REMOVE, &CDieHardDoc::OnTaskRemoveUpdateUI)
		
		ON_COMMAND(IDM_TASK_START, &CDieHardDoc::OnTaskStart)
		ON_UPDATE_COMMAND_UI(IDM_TASK_START, &CDieHardDoc::OnTaskStartUpdateUI)
		ON_COMMAND(IDT_START, &CDieHardDoc::OnTaskStart)
		ON_UPDATE_COMMAND_UI(IDT_START, &CDieHardDoc::OnTaskStartUpdateUI)

		ON_COMMAND(IDM_TASK_STOP, &CDieHardDoc::OnTaskStop)
		ON_UPDATE_COMMAND_UI(IDM_TASK_STOP, &CDieHardDoc::OnTaskStopUpdateUI)
		ON_COMMAND(IDT_STOP, &CDieHardDoc::OnTaskStop)
		ON_UPDATE_COMMAND_UI(IDT_STOP, &CDieHardDoc::OnTaskStopUpdateUI)
		

		ON_COMMAND(IDM_TASK_HASH_CHECKING, &CDieHardDoc::OnTaskHash)
		ON_UPDATE_COMMAND_UI(IDM_TASK_HASH_CHECKING, &CDieHardDoc::OnTaskHashUpdateUI)

		ON_COMMAND(IDM_SAVE_AS_TASK_TORRENT, &CDieHardDoc::OnTaskSaveAsTorrent)
		ON_UPDATE_COMMAND_UI(IDM_SAVE_AS_TASK_TORRENT, &CDieHardDoc::OnTaskSaveAsTorrentUpdateUI)

		ON_COMMAND(IDM_VIEW_CONSOLE, &CDieHardDoc::OnOpenConsole)
		ON_UPDATE_COMMAND_UI(IDM_VIEW_CONSOLE, &CDieHardDoc::OnOpenConsoleUpdateUI)


		ON_COMMAND(IDM_BROWSE_TASK_DIR, &CDieHardDoc::ShowDirectory)
		ON_UPDATE_COMMAND_UI(IDM_BROWSE_TASK_DIR, &CDieHardDoc::ShowDirectoryUpdateUI)
		ON_COMMAND(IDT_OPEN_DIR, &CDieHardDoc::ShowDirectory)
		ON_UPDATE_COMMAND_UI(IDT_OPEN_DIR, &CDieHardDoc::ShowDirectoryUpdateUI)



END_MESSAGE_MAP()


void CDieHardDoc::ShowDirectory()
{
		if(m_focus_downloader == NULL) return;

		CString dir = StrConv(m_focus_downloader->GetSavePath() + m_focus_downloader->GetTorrentFile().GetTopDir());

		::ShellExecute(AfxGetMainWnd()->m_hWnd, _T("open"), NULL, NULL, dir, SW_SHOW);
		
}


void CDieHardDoc::ShowDirectoryUpdateUI(CCmdUI *pui)
{
		pui->Enable(m_focus_downloader != NULL);
}

void CDieHardDoc::OnTaskSaveAsTorrent()
{
		assert(m_downloader_manager != NULL);
		
		if(m_focus_downloader == NULL) return;
		


		CFolderDialog dlg;

		if(dlg.DoModal() == IDOK)
		{
				std::wstring path = StrConv(dlg.GetPathName());
				path += L"\\";

				if(!m_downloader_manager->CopyTorrentFileTo(path, m_focus_downloader->GetTorrentFile()))
				{
						AfxMessageBox(_T("Copy Filad"));
				}
		}

		



}

void CDieHardDoc::OnTaskSaveAsTorrentUpdateUI(CCmdUI *pui)
{
		pui->Enable(m_focus_downloader != NULL);
}


/*
#define IDM_TASK_START                  32808
#define IDM_TASK_STOP                   32809
#define IDM_TASK_HASH_CHECKING          32810
#define IDM_BROWSE_TASK_DIR             32812
#define IDM_SAVE_AS_TASK_TORRENT        32813
#define IDM_TASK_SETTING                32814
#define IDM_TASK_REMOVE                 32815
#define IDM_REMOVE                      32816*/

void CDieHardDoc::OnTaskHash()
{
		assert(m_downloader_manager != NULL);
		
		using namespace Rage;
		assert(m_downloader_manager != NULL);
		
		if(m_focus_downloader == NULL || m_focus_downloader->GetDownloaderState() != Rage::DS_STOP)return;
		
		bool res = m_focus_downloader->StartHashChecing();

		assert(res);
		
		this->UpdateAllViews(NULL);
}

void CDieHardDoc::OnTaskHashUpdateUI(CCmdUI *pui)
{
		pui->Enable(m_focus_downloader != NULL && m_focus_downloader->GetDownloaderState() == DS_STOP);

}



void CDieHardDoc::OnTaskStart()
{
		using namespace Rage;
		assert(m_downloader_manager != NULL);
		
		if(m_focus_downloader == NULL || m_focus_downloader->GetDownloaderState() != Rage::DS_STOP)return;

		bool res = m_focus_downloader->StartDownload();

		assert(res);

		SetFocusDownloader(m_focus_downloader);
}

void CDieHardDoc::OnTaskStartUpdateUI(CCmdUI *pui)
{
		using namespace Rage;
		pui->Enable(m_focus_downloader != NULL && m_focus_downloader->GetDownloaderState() == DS_STOP);
}

void CDieHardDoc::OnTaskStop()
{
		assert(m_downloader_manager != NULL);
		
		if(m_focus_downloader == NULL && m_focus_downloader->GetDownloaderState() == Rage::DS_STOP)return;

		using namespace Rage;
		switch(m_focus_downloader->GetDownloaderState())
		{
		case DS_RUNNING:
		case DS_DOWN_ABORT:
				m_focus_downloader->StopDownload();
				break;
		case DS_HASH_CHECKING:
		case DS_HASH_ABORT:
				m_focus_downloader->StopHashChecking();
				break;
		}
		
		SetFocusDownloader(m_focus_downloader);
}

void CDieHardDoc::OnTaskStopUpdateUI(CCmdUI *pui)
{
		pui->Enable(m_focus_downloader != NULL && m_focus_downloader->GetDownloaderState() != Rage::DS_STOP);
}



// CDieHardDoc construction/destruction

CDieHardDoc::CDieHardDoc() 
		: m_downloader_manager(0)
		, m_timer_id(0)
		, m_focus_downloader(0)
{
	// TODO: add one-time construction code here
		if(init() == FALSE)
		{
				AfxMessageBox(_T("DownloaderManager Init Failed"));
				exit(-1);
		}
}

CDieHardDoc::~CDieHardDoc()
{
		try{
				uninit();
		}catch(...)
		{

		}
}

BOOL CDieHardDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CDieHardDoc serialization

void CDieHardDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CDieHardDoc diagnostics

#ifdef _DEBUG
void CDieHardDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDieHardDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDieHardDoc commands

void CDieHardDoc::OnGlobalSetting()
{
		// TODO: Add your command handler code here

		ASSERT(m_downloader_manager != NULL);
		Rage::GlobalSetting &cfg = m_downloader_manager->GetGlobalSetting();

		CSettingDialog dialog;

		dialog.m_cache_size = cfg.GetMaxCacheSize() / (1*MB);


		dialog.m_listen_port = m_downloader_manager->GetCurrListenPort();
		dialog.m_max_conn_per_task = cfg.GetMaxConnectionPerTask();
		dialog.m_max_upload_conn_per_task = cfg.GetMaxUploadFromLocalPeerCount();
		dialog.m_request_queue_size = cfg.GetMaxRequestPendingCount();
		dialog.m_def_save_path = StrConv(cfg.GetDefaultSavePath());



		
		dialog.DoModal();
		
		if(cfg.GetMaxCacheSize() != dialog.m_cache_size * (1*MB))
		{
				cfg.SetMaxCacheSize(dialog.m_cache_size * (1*MB));
				this->m_downloader_manager->ReloadSetting();
		}

		

		cfg.SetListenPort(dialog.m_listen_port);

		cfg.SetMaxConnectionPerTask(dialog.m_max_conn_per_task);
		cfg.SetMaxUploadFromLocalPeerCount(dialog.m_max_upload_conn_per_task);
		cfg.SetMaxRequestPendingCount(dialog.m_request_queue_size);
		
		cfg.SetDefaultSavePath(StrConv(dialog.m_def_save_path));
		
		m_downloader_manager->ResetListenPort();
		

		

		
}


void CDieHardDoc::OnTaskRemove()
{
		assert(m_downloader_manager != NULL);
		
		if(m_focus_downloader == NULL)return;

		assert(m_focus_downloader->IsOpen());

		
		Rage::Downloader *pd = m_focus_downloader;

		UpdateAllViews(NULL, (LPARAM)REMOVE_TASK, (CObject*)m_focus_downloader);
		
		bool res = m_downloader_manager->RemoveTask(pd->GetTorrentFile().GetInfoHash());
		assert(res);

}

void CDieHardDoc::OnTaskRemoveUpdateUI(CCmdUI *pui)
{
		pui->Enable(m_focus_downloader != NULL);

}

/*

struct CFileInfo
{
		CString		file_name;
		CString		file_size;
		CString		finished_degree;
};
*/

bool CDieHardDoc::show_task_setting(Rage::Downloader *pdownloader)
{

		ASSERT(pdownloader->IsOpen());

		

		CTaskSetting dlg;

		dlg.m_is_stoped_task = (pdownloader->GetDownloaderState() == Rage::DS_STOP ? TRUE : FALSE);

		const Rage::TorrentFile &tf = pdownloader->GetTorrentFile();


		
		

		dlg.m_save_location = StrConv(pdownloader->GetSavePath());
		dlg.m_torrent_name = StrConv(tf.GetTorrentName());

		
		
		std::string str = FormatSpace::NumberFormatter::Format((double)tf.GetTotalSize() / (double)(1*MB),2);
		

		dlg.m_torrent_total_size = StrConv(FormatSpace::AnsiConverter::ToUnicode(str + " Mb"));
		
		Rage::FileStatusList fsl = pdownloader->GetFilesStatus();
		for(Rage::FileStatusList::const_iterator it = fsl.begin(); it != fsl.end(); ++it)
		{
				CFileInfo fi;
				fi.file_name = StrConv(tf.GetFileInfo(it->fidx).path);
				str = FormatSpace::NumberFormatter::Format((double)tf.GetFileInfo(it->fidx).length / (double)(1*MB), 2);
				
				fi.file_size = StrConv(FormatSpace::AnsiConverter::ToUnicode(str));
				str = FormatSpace::NumberFormatter::Format(it->finished_degree * 100,2);
				fi.finished_degree = StrConv(FormatSpace::AnsiConverter::ToUnicode(str));
				dlg.m_files.Add(fi);

		}


		const Rage::TorrentFile &ref_tf = pdownloader->GetTorrentFile();
		
		bool res = (dlg.DoModal() == IDOK);

		if(res && dlg.m_is_stoped_task == TRUE)
		{
				bool res  = pdownloader->ResetSavePath(StrConv(dlg.m_save_location));
				m_downloader_manager->ResumeDataUpdate();
				m_downloader_manager->m_resume->Save();
				assert(res);
		}
		return res;
}




void CDieHardDoc::OnTaskSetting()
{
		if(m_focus_downloader != NULL)
		{
				show_task_setting(m_focus_downloader);
		}
}

void CDieHardDoc::OnTaskSettingUpdateUI(CCmdUI *pcmd_ui)
{
		pcmd_ui->Enable(m_focus_downloader != NULL);

}


BOOL CDieHardDoc::init(void)
{
		

		if(m_downloader_manager != NULL)
		{
				ASSERT(FALSE);
				return FALSE;
		}

		
		TCHAR buf[MAX_PATH];

		if(::GetModuleFileName(NULL, buf, MAX_PATH) == 0) return FALSE;
		
		FileSysSpace::Path		path(buf);
		std::wstring wstr;
		path.GetParent(wstr);

		std::auto_ptr<Rage::DownloaderManager> pdm(new Rage::DownloaderManager());
		
		if(!pdm->Init(wstr + L"resume.dat", wstr + L"config.dat", wstr + L"torrent\\"))
		{
				return FALSE;
		}

		Rage::GlobalSetting &ref_cfg = pdm->GetGlobalSetting();

		if(ref_cfg.GetDefaultSavePath() == L"")
		{
				path.Reset(wstr + L"download\\");
				path.Make();
				ref_cfg.SetDefaultSavePath(wstr + L"download\\");
				ref_cfg.Save();
		}

		m_downloader_manager = pdm.release();
		
		m_timer_id = ::SetTimer(NULL, 0, 1000, DocTimerProc);

		return (m_timer_id != 0);
}


VOID CDieHardDoc::uninit(void)
{
		assert(m_downloader_manager != NULL);

		assert(m_timer_id != 0);
		
		::KillTimer(NULL, m_timer_id);
		
		m_downloader_manager->UnInit();

		delete m_downloader_manager;
		m_downloader_manager = 0;
}


const Rage::DownloaderList& CDieHardDoc::GetDownloaderList()const
{
		assert(m_downloader_manager != NULL);
		return m_downloader_manager->GetDownloaderList();

}



static const TCHAR filter[]
= _T("Torrent Files (*.torrent)|*.torrent|All Files (*.*)|*.*||");



void CDieHardDoc::OnFileOpenTorrent()
{
		// TODO: Add your command handler code here
		
		assert(m_downloader_manager != NULL);

		

		CFileDialog dlg(TRUE,NULL,NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST, filter);
		
		
		if(dlg.DoModal() == IDOK)
		{
				OpenTorrent(dlg.GetPathName());
		}
}


BOOL CDieHardDoc::OpenTorrent(const CString &torrent_path)
{

		Rage::Downloader *pdownloader = m_downloader_manager->AddTask(StrConv(torrent_path));

		if(pdownloader == 0)
		{
				AfxMessageBox(_T("Can't Open Torrent"));
				return FALSE;
		}else
		{
				if(!show_task_setting(pdownloader))
				{
						bool res = m_downloader_manager->RemoveTask(pdownloader->GetTorrentFile().GetInfoHash());
						ASSERT(res);
				}else
				{
						this->UpdateAllViews(NULL, (LPARAM)ADD_TASK, (CObject*)pdownloader);
				}
				return TRUE;
		}
}

void CDieHardDoc::SetFocusDownloader(Rage::Downloader *pdownloader)
{
		m_focus_downloader = pdownloader;
		this->UpdateAllViews(NULL, (LPARAM)CHANGED_FOCUS, (CObject*)pdownloader);

}

Rage::Downloader* CDieHardDoc::GetFocusDownloader()
{
		return m_focus_downloader;

}

#define SAVE_TIME_OUT	(60 * 10)
#define CLEAR_IPFILTER_TIMEOUT (60 * 60)

VOID CDieHardDoc::SecondTick()
{
		assert(m_downloader_manager != NULL);

		UpdateAllViews(NULL, (LPARAM)UPDATE_TIMER, (CObject*)m_focus_downloader);
		SetModifiedFlag(FALSE);

		if(m_info_save_ts.ElapsedSecond() > SAVE_TIME_OUT)
		{
				m_downloader_manager->SaveResumeInfo();
				m_info_save_ts.Update();
		}

		if(m_ipfilter_ts.ElapsedSecond() > CLEAR_IPFILTER_TIMEOUT)
		{
				m_downloader_manager->ClearIPFilter();
				m_ipfilter_ts.Update();

		}
				

}
