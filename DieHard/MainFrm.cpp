// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DieHard.h"

#include "MainFrm.h"
#include "Util.h"
#include "SelectView.h"
#include "SummaryView.h"
#include "DieHardDoc.h"
#include "StatusView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this/*, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC*/) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	LoadTrueColorToolBar();


	


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
		
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	/*m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/
	this->DragAcceptFiles(TRUE);
	


	return 0;
}




BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;

	
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// º”»Î’Ê≤ ToolBar




static const TCHAR* tool_bar_text[] = 
{
		_T("Open"),
		_T("Start"),
		_T("Pause"),
		_T("Stop"),
		_T("Directory"),
		_T("Task Setting"),
		_T("Remove"),
		_T("Favorite"),
		_T("Move Up"),
		_T("Move Down"),
		_T("Setting"),
		_T("UNKNOW1"),
		_T("Exit"),
		_T("Create New"),
		_T("Transfer"),
		_T("Web"),
		_T("Back"),
		_T("Forward"),
		_T("Refresh"),
		_T("Stop"),
		_T("Preview"),
		_T("UNKNOW2")
};

static const UINT tool_bar_id[] = 
{
		IDT_OPEN_TORRENT,					//0
		IDT_START,							//1
		IDT_PAUSE,							//2
		IDT_STOP,							//3
		IDT_OPEN_DIR,						//4
		IDT_TASK_SETTING,					//5
		IDT_REMOVE,							//6
		IDT_FAVORITE,						//7
		IDT_MOVE_UP,						//8
		IDT_MOVE_DOWN,						//9
		IDT_SETTING,						//10
		IDT_UNKNOW1,						//11
		IDT_EXIT,							//12
		IDT_CREATE_NEW,						//13
		IDT_TRANSFER_VIEW,					//14
		IDT_WEB_VIEW,						//15
		IDT_WEB_BACK,						//16
		IDT_WEB_FORWARD,  					//17
		IDT_WEB_REFRESH,					//18
		IDT_WEB_STOP,						//19
		IDT_PREVIEW,						//20
		IDT_UNKNOW2							//21
};



BOOL CMainFrame::LoadTrueColorToolBar(void)
{
		m_wndToolBar.SetBorders(1,1,1,1);
		m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT);

		BOOL res = m_wndToolBar.LoadTrueColorToolBar(24, IDB_TOOLBAR_HOT, IDB_TOOLBAR_HOT, IDB_TOOLBAR_COLD);

		if(res != TRUE)
		{
				ASSERT(FALSE);
				return FALSE;
		}
	
		for(size_t idx = 0; idx < GET_ARRAY_SIZE(tool_bar_text); ++idx)
		{
				m_wndToolBar.SetButtonText(idx, tool_bar_text[idx]);
		}

		m_wndToolBar.SetSizes(CSize(45,45),CSize(24,24));


		for(size_t i = 14; i < GET_ARRAY_SIZE(tool_bar_id) + 1; ++i)
		{
				m_wndToolBar.GetToolBarCtrl().HideButton(tool_bar_id[i], TRUE);
		}
		
		m_wndToolBar.GetToolBarCtrl().HideButton(tool_bar_id[2], TRUE);
		m_wndToolBar.GetToolBarCtrl().HideButton(tool_bar_id[7], TRUE);
		m_wndToolBar.GetToolBarCtrl().HideButton(tool_bar_id[8], TRUE);
		m_wndToolBar.GetToolBarCtrl().HideButton(tool_bar_id[9], TRUE);
		m_wndToolBar.GetToolBarCtrl().HideButton(tool_bar_id[11], TRUE);


		return TRUE;
}



void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
		// TODO: Add your message handler code here and/or call default

		int nFileNum = DragQueryFile(hDropInfo,0xFFFFFFFF,0,0);


		for(size_t i = 0; i < nFileNum; ++i)
		{
				TCHAR *pbuf = new TCHAR[MAX_PATH];
				int nCharactersLen = ::DragQueryFile(hDropInfo,i,NULL,100);
				::DragQueryFile(hDropInfo,i,pbuf,nCharactersLen+1);
				
				CDieHardDoc *pdoc = (CDieHardDoc*)GetActiveDocument();
				pdoc->OpenTorrent(CString(pbuf));


				delete pbuf;
		}
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
		// TODO: Add your specialized code here and/or call the base class

		CRect rect;
		GetClientRect(&rect);

		m_wndSplitter.CreateStatic(this, 2, 1);
		m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CStatusView), CSize(rect.Width(), rect.Height()/2), pContext);

		
		m_wndSplitter2.CreateStatic(&m_wndSplitter, 1, 2, WS_CHILD|WS_VISIBLE, m_wndSplitter.IdFromRowCol(1,0));

		m_wndSplitter2.CreateView(0,0, RUNTIME_CLASS(CSelectView), CSize(rect.Width()/5, rect.Height()/2), pContext);
		
		m_wndSplitter2.CreateView(0,1, RUNTIME_CLASS(CSummaryView), CSize(rect.Width()/2, rect.Height()/2), pContext);

		m_wndSplitter2.SetActivePane(0,0);

		((CSelectView*)m_wndSplitter2.GetPane(0,0))->GetListCtrl().SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);

		return TRUE;

		//return CFrameWnd::OnCreateClient(lpcs, pContext);

}










#define MYWM_NOTIFYICON		(WM_USER+2)


BOOL CMainFrame::TrayMessage(DWORD dwMessage)
{
		CString				sTip(_T("DieHard"));
		NOTIFYICONDATA		tnd;

		tnd.cbSize		= sizeof(NOTIFYICONDATA);
		tnd.hWnd		= m_hWnd;
		tnd.uID			= IDR_MAINFRAME;

		tnd.uFlags		= NIF_MESSAGE|NIF_ICON;

		tnd.uCallbackMessage	= MYWM_NOTIFYICON;


		VERIFY( tnd.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE (IDR_MAINFRAME)) );

		tnd.uFlags		= NIF_MESSAGE|NIF_ICON|NIF_TIP;

		lstrcpyn(tnd.szTip, (LPCTSTR)sTip, sizeof(tnd.szTip)/sizeof(tnd.szTip[0]) );

		return Shell_NotifyIcon(dwMessage, &tnd);

}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
			// TODO: Add your message handler code here
		if(nType == SIZE_MINIMIZED)
		{
				TrayMessage(NIM_ADD);
				ShowWindow(SW_HIDE);
		}
		else{
				CFrameWnd::OnSize(nType, cx, cy);
		}	
}

void CMainFrame::OnClose()
{
		// TODO: Add your message handler code here and/or call default

		CFrameWnd::OnClose();
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
		// TODO: Add your specialized code here and/or call the base class
		if(message == MYWM_NOTIFYICON)
		{
				switch (lParam)
				{
				case WM_LBUTTONDBLCLK:
				{
					switch (wParam)	
					{
					case IDR_MAINFRAME:
					{
							ShowWindow(SW_NORMAL);
							SetForegroundWindow();
							SetFocus();
							TrayMessage(NIM_DELETE);
							return TRUE;
							break;
					}
					}
				}
				break;
				}
		}
		return CFrameWnd::WindowProc(message, wParam, lParam);
}
