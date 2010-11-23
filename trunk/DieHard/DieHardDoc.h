// DieHardDoc.h : interface of the CDieHardDoc class
//


#pragma once
#include "Console.h"

enum DocUpdateHint
{
		ADD_TASK	= 0x0001,
		REMOVE_TASK = 0x0002,
		CHANGED_FOCUS = 0x0003,
		UPDATE_TIMER  = 0x0004
};



class CDieHardDoc : public CDocument
{
protected: // create from serialization only
	CDieHardDoc();
	DECLARE_DYNCREATE(CDieHardDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CDieHardDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
		afx_msg void OnGlobalSetting();
		


//////////////////////初始化/反初始化代码///////////////////////////
protected:
		BOOL init(void);

		VOID uninit(void);

		bool show_task_setting(Rage::Downloader *pdownloader);

/////////////////////////自定义数据和函数/////////////

private:
		Rage::DownloaderManager			*m_downloader_manager;
		UINT_PTR						m_timer_id;

private:
		Rage::Downloader				*m_focus_downloader;
public:
		void SetFocusDownloader(Rage::Downloader *pdownloader);

		Rage::Downloader* GetFocusDownloader();

		const Rage::DownloaderList& GetDownloaderList()const;
public:
		TimeSpace::TimeStamp			m_info_save_ts;
		TimeSpace::TimeStamp			m_ipfilter_ts;
		void SecondTick();

public:
		BOOL OpenTorrent(const CString &torrent_path);

		afx_msg void OnFileOpenTorrent();
		
		void OnTaskStart();
		void OnTaskStartUpdateUI(CCmdUI *pui);

		void OnTaskStop();
		void OnTaskStopUpdateUI(CCmdUI *pui);

		void OnTaskHash();
		void OnTaskHashUpdateUI(CCmdUI *pui);

		void OnTaskSetting();
		void OnTaskSettingUpdateUI(CCmdUI *pui);

		void OnTaskRemove();
		void OnTaskRemoveUpdateUI(CCmdUI *pui);

		void OnTaskBrowseDir();
		void OnTaskBrowseDirUpdateUI(CCmdUI *pui);

		void OnTaskSaveAsTorrent();
		void OnTaskSaveAsTorrentUpdateUI(CCmdUI *pui);

private:
		Console			m_console;

		void OnOpenConsole();
		void OnOpenConsoleUpdateUI(CCmdUI *pui);

		void ShowDirectory();
		void ShowDirectoryUpdateUI(CCmdUI *pui);

};




