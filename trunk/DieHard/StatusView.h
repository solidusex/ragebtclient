// StatusView.h : interface of the CStatusView class
//


#pragma once



class CStatusView : public CListView
{
protected: // create from serialization only
	CStatusView();
	DECLARE_DYNCREATE(CStatusView)

// Attributes
public:
	CDieHardDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CStatusView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
//	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

public:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
		bool insert_downloader(Rage::Downloader *pdownloader, UINT item_idx);

		bool remove_downloader(Rage::Downloader *pdownloader);

		void update_downloader(Rage::Downloader *pdownloader);

		bool is_existed(Rage::Downloader *pd);

		virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
		afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);

private:

		struct CDownloaderStatus
		{
				CString			status; 
				CString			name;
				CString			total_size;
				CString			percent;
				CString			up_rate;
				CString			down_rate;
				CString			time_left;
				CString			seed_peer_count;
		};

		typedef std::map<Rage::Downloader*, CDownloaderStatus> StatusMap;

		StatusMap		m_status_map;
public:
		afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
//		afx_msg void OnDropFiles(HDROP hDropInfo);
};



#ifndef _DEBUG  // debug version in StatusView.cpp
inline CDieHardDoc* CStatusView::GetDocument() const
   { return reinterpret_cast<CDieHardDoc*>(m_pDocument); }
#endif

