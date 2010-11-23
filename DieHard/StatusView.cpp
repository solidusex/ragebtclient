// StatusView.cpp : implementation of the CStatusView class
//

#include "stdafx.h"
#include "DieHard.h"

#include "DieHardDoc.h"
#include "StatusView.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStatusView

IMPLEMENT_DYNCREATE(CStatusView, CListView)

BEGIN_MESSAGE_MAP(CStatusView, CListView)
		ON_WM_CREATE()
		ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CStatusView::OnLvnItemchanged)
		ON_NOTIFY_REFLECT(NM_RCLICK, &CStatusView::OnNMRclick)
		ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CStatusView::OnLvnGetdispinfo)
//		ON_WM_DROPFILES()
END_MESSAGE_MAP()

// CStatusView construction/destruction

CStatusView::CStatusView()
{
	// TODO: add construction code here

}

CStatusView::~CStatusView()
{
}

BOOL CStatusView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
		cs.style |= (LVS_REPORT | LVS_SINGLESEL |LVS_SHOWSELALWAYS|LVS_ALIGNLEFT);
		return CListView::PreCreateWindow(cs);
}




// CStatusView diagnostics

#ifdef _DEBUG
void CStatusView::AssertValid() const
{
	CListView::AssertValid();
}

void CStatusView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CDieHardDoc* CStatusView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDieHardDoc)));
	return (CDieHardDoc*)m_pDocument;
}
#endif //_DEBUG


// CStatusView message handlers



static const TCHAR * COLUMN_ARRAY[] = 
{
		_T("Status"),
		_T("Name"),
		_T("Size"),
		_T("%"),
		_T("Kb/s Up"),
		_T("Kb/s Down"),
		_T("Time Left"),
		_T("Seed/Peer")
};

int CStatusView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CListView::OnCreate(lpCreateStruct) == -1)
				return -1;

		// TODO:  Add your specialized creation code here

		GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP );


		for(size_t idx = 0; idx < GET_ARRAY_SIZE(COLUMN_ARRAY); ++idx)
		{
				GetListCtrl().InsertColumn(idx, COLUMN_ARRAY[idx], LVCFMT_LEFT, 80 - 5);
				
		}

		
		return 0;
}


bool CStatusView::is_existed(Rage::Downloader *pd)
{
		ASSERT(pd != NULL);
		return (m_status_map.count(pd) > 0);



}


bool CStatusView::insert_downloader(Rage::Downloader *pdownloader, UINT item_idx)
{
		ASSERT(pdownloader != NULL);
		ASSERT(!is_existed(pdownloader));

		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT|LVIF_PARAM;
		lvi.iItem = 0;//GetListCtrl().GetItemCount();
		lvi.iSubItem = 0;
		lvi.iImage = 0;
		lvi.pszText = LPSTR_TEXTCALLBACK;
		lvi.lParam = (LPARAM)pdownloader;
		
		if(GetListCtrl().InsertItem(&lvi) != 0)
		{
				return false;
		}

		m_status_map[pdownloader] = CDownloaderStatus();
		update_downloader(pdownloader);
		return true;
}


bool CStatusView::remove_downloader(Rage::Downloader *pdownloader)
{
		if(is_existed(pdownloader))
		{
				for(UINT idx = 0; idx < GetListCtrl().GetItemCount(); ++idx)
				{
						if(pdownloader == (Rage::Downloader*)GetListCtrl().GetItemData(idx))
						{
								GetListCtrl().DeleteItem(idx);
								m_status_map.erase(pdownloader);
								return true;				
						}
				}
				assert(0);
				return false;
		}else
		{
				return false;
		}

}


void CStatusView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	m_status_map.clear();

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().

	
		
	CDieHardDoc *pdoc = this->GetDocument();

	const Rage::DownloaderList &ref_list = pdoc->GetDownloaderList();

	for(Rage::DownloaderList::const_iterator cit = ref_list.begin(); cit != ref_list.end(); ++cit)
	{
			bool res = insert_downloader(*cit, GetListCtrl().GetItemCount());

			assert(res);
	}

}



void CStatusView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
		// TODO: Add your specialized code here and/or call the base class

		switch((DocUpdateHint)lHint)
		{
		case ADD_TASK:
		{
				Rage::Downloader *pdown = (Rage::Downloader*)pHint;
				assert(pdown != 0);
				insert_downloader(pdown, GetListCtrl().GetItemCount());
				break;
		}
		case REMOVE_TASK:
		{
				remove_downloader((Rage::Downloader*)pHint);
				break;
		}
		default:
		{
				for(StatusMap::iterator it = m_status_map.begin(); it != m_status_map.end(); ++it)
				{
						update_downloader(it->first);
				}

				this->Invalidate();
				this->UpdateWindow();
		}
		}
}

void CStatusView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		// TODO: Add your control notification handler code here
		*pResult = 0;

		if(pNMLV->uChanged == LVIF_STATE)
		{
				if(pNMLV->uNewState)
				{
						//MessageBox(_T("选择改变且有选中的行\r\n"));
						POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();

						int idx = GetListCtrl().GetNextSelectedItem(pos);		
						if(idx != -1)
						{
								Rage::Downloader *pd = (Rage::Downloader*)GetListCtrl().GetItemData(idx);
								assert(pd != 0 && pd->IsOpen());
								GetDocument()->SetFocusDownloader(pd);
						}
				}else
				{
						GetDocument()->SetFocusDownloader(NULL);
				}
		}else
		{
				//MessageBox(_T("行改变(CListCtrl::SetItem)\r\n"));
		}
}

void CStatusView::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
		// TODO: Add your control notification handler code here
		
		POSITION pos = this->GetListCtrl().GetFirstSelectedItemPosition();
		if(pos != NULL && this->GetListCtrl().GetNextSelectedItem(pos) != -1)
		{
				CMenu menu;
				menu.LoadMenu(IDM_MENU_CONTEXT);


				CMenu *pcontext = menu.GetSubMenu(0);
				ASSERT(pcontext != 0);

				POINT point2;
				::GetCursorPos(&point2);
				pcontext->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RIGHTBUTTON, point2.x, point2.y, ::AfxGetMainWnd());
		}









		*pResult = 0;
}


#define SET_TEXT(cstr) ::lstrcpy(pDispInfo->item.pszText, cstr)




void CStatusView::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
		NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
		// TODO: Add your control notification handler code here
		*pResult = 0;
		
		Rage::Downloader *pd = (Rage::Downloader*)pDispInfo->item.lParam;

		assert(is_existed(pd));

		const CDownloaderStatus &ref_stat = m_status_map[pd];

		switch(pDispInfo->item.iSubItem) 
		{
		case 0:
				SET_TEXT(ref_stat.status);
				break;
		case 1:
				SET_TEXT(ref_stat.name);
				break;
		case 2:
				SET_TEXT(ref_stat.total_size);
				break;
		case 3:
				SET_TEXT(ref_stat.percent);
				break;
		case 4:
				SET_TEXT(ref_stat.up_rate);
				break;
		case 5:
				SET_TEXT(ref_stat.down_rate);
				break;
		case 6:
				SET_TEXT(ref_stat.time_left);
				break;
		case 7:
				SET_TEXT(ref_stat.seed_peer_count);
				break;
		}
}


#undef SET_TEXT





std::string second_to_str(t_uint64 sec)
{
		t_uint64 hour = sec / 3600;
		
		if(hour > 99) return "99:99:99";

		sec = sec % 3600;

		t_uint64 minute = sec / 60;

		sec = sec % 60;

		std::string res = FormatSpace::NumberFormatter::Format(hour);

		res += ":";
		res += FormatSpace::NumberFormatter::Format(minute);
		res += ":";
		res += FormatSpace::NumberFormatter::Format(sec);
		
		return res;
}





#define INT_FMT(val) StrConv(FormatSpace::AnsiConverter::ToUnicode(FormatSpace::NumberFormatter::Format(val)))

#define FLOAT_FMT(val) StrConv(FormatSpace::AnsiConverter::ToUnicode(FormatSpace::NumberFormatter::Format(val,2)))




void CStatusView::update_downloader(Rage::Downloader *pd)
{
		using namespace Rage;

		assert(is_existed(pd));
		
		CDownloaderStatus &ref_stat = m_status_map[pd];

		ref_stat = CDownloaderStatus();//清空
		
		Rage::BitField bitf = pd->GetBitField();

		switch(pd->GetDownloaderState())
		{
		case DS_RUNNING:
		{
				if(!bitf.IsAllSet())
				{
						ref_stat.status = _T("Downloading");
				}else
				{
						ref_stat.status = _T("Seeding");
				}
				break;
		}
				
		case Rage::DS_DOWN_ABORT:
				ref_stat.status = _T("Download Abort");
				break;
		case DS_HASH_CHECKING:
				ref_stat.status = _T("HashChecking");
				break;
		case DS_HASH_ABORT:
				ref_stat.status = _T("Hash Abort");
				break;
		case DS_STOP:
				ref_stat.status = _T("Stop");
				break;
		}

		

		ref_stat.name = StrConv(pd->GetTorrentFile().GetTorrentName());
		
		double total_size = (double)pd->GetTorrentFile().GetTotalSize() / (1*MB);

		ref_stat.total_size = FLOAT_FMT(total_size) + _T(" Mb");

		
		
		
		switch(pd->GetDownloaderState())
		{
		case DS_RUNNING:
		case Rage::DS_DOWN_ABORT:
				ref_stat.percent = FLOAT_FMT(pd->GetFinishedDegree() * 100);
				break;
		case DS_HASH_CHECKING:
		case DS_HASH_ABORT:
		{
				Rage::HashingStatus hs = pd->GetHashStatus();
				if(hs.is_done)
				{
						pd->StopHashChecking();
				}else
				{
						ref_stat.percent = FLOAT_FMT(hs.finished_degree * 100);		
						
				}
				break;
		}
		case DS_STOP:
		{
				ref_stat.percent = FLOAT_FMT(pd->GetFinishedDegree() * 100);
		}
				break;
		}


		switch(pd->GetDownloaderState())
		{
		case DS_RUNNING:
		{
				Rage::DownStatus ds = pd->GetDownStatus();

				ref_stat.up_rate = FLOAT_FMT(ds.up_rate / (1*KB));
				ref_stat.down_rate = FLOAT_FMT(ds.down_rate / (1*KB));
				
				std::string str = second_to_str(ds.remain_time);
				
				if(!bitf.IsAllSet())
				{
						ref_stat.time_left = StrConv(FormatSpace::AnsiConverter::ToUnicode(str));
				}else
				{
						ref_stat.time_left = _T("0");
				}
				
				ref_stat.seed_peer_count = INT_FMT(ds.num_seed);
				ref_stat.seed_peer_count += _T("/");
				ref_stat.seed_peer_count += INT_FMT(ds.num_peer);
				
		}
		case Rage::DS_DOWN_ABORT:
		case DS_HASH_CHECKING:
		case DS_HASH_ABORT:
		case DS_STOP:
				break;
		default:
				break;
		}
		


}

#undef NUM_FMT


