// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "TrueColorToolBar.h"
#include "SplitterWndEx.h"
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	//CToolBar    m_wndToolBar;
	CTrueColorToolBar m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
		// º”»Î’Ê≤ ToolBar
		BOOL LoadTrueColorToolBar(void);
		afx_msg void OnDropFiles(HDROP hDropInfo);

public:
		CSplitterWndEx		m_wndSplitter;
		CSplitterWndEx		m_wndSplitter2;

		BOOL InitSplitterWnd();
protected:
		virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
		BOOL TrayMessage(DWORD dwMessage);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnClose();
protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};




