#include "StdAfx.h"
#include "PeerView.h"
#include "Util.h"
#include "DieHardDoc.h"

IMPLEMENT_DYNCREATE(CPeerView, CReportView)

CPeerView::CPeerView(void)
{

}

CPeerView::~CPeerView(void)
{

}

BEGIN_MESSAGE_MAP(CPeerView, CReportView)
		ON_WM_CREATE()
END_MESSAGE_MAP()



const TCHAR *peer_str_arr[] = 
{
		_T("PeerEntry"),
		_T("Down Rate"),
		_T("Up Rate"),
		_T("Mode"),
		_T("%")
		
};


int CPeerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CReportView::OnCreate(lpCreateStruct) == -1)
				return -1;

		// TODO:  Add your specialized creation code here
		for(size_t i = 0; i < GET_ARRAY_SIZE(peer_str_arr); ++i)
		{
				GetListCtrl().InsertColumn(i, peer_str_arr[i], 0, 100);
		}
		return 0;
}


#define STR_TO_CSTR(str) (StrConv(FormatSpace::AnsiConverter::ToUnicode(str)))


#define INT_FMT(val) StrConv(FormatSpace::AnsiConverter::ToUnicode(FormatSpace::NumberFormatter::Format(val)))

#define FLOAT_FMT(val) StrConv(FormatSpace::AnsiConverter::ToUnicode(FormatSpace::NumberFormatter::Format(val,2)))

/*
		const TCHAR *peer_str_arr[] = 
{
		_T("PeerEntry"),
		_T("Downloaded"),
		_T("Uploaded"),
		_T("Mode")
};
*/

void CPeerView::update_peers(Rage::Downloader *pcurr)
{
		
		assert(pcurr !=NULL);
		CListCtrl &ref_list = GetListCtrl();
		ref_list.DeleteAllItems();
		Rage::DownStatus ds = pcurr->GetDownStatus();
		
		size_t idx = 0;
		for(size_t i = 0; i < ds.peers_status.size(); ++i)
		{
				NetSpace::InetAddress addr;
				addr.SetAddr(ds.peers_status[i].peer_entry.addr);

				if(addr.IP() == 0)
				{
						continue;
				}else
				{
						ref_list.InsertItem(idx, STR_TO_CSTR(addr.Str() + ":" + FormatSpace::NumberFormatter::Format(ds.peers_status[i].peer_entry.port)));

						ref_list.SetItemText(idx, 1, FLOAT_FMT(ds.peers_status[i].down_rate / (1*KB)));
						ref_list.SetItemText(idx, 2, FLOAT_FMT(ds.peers_status[i].up_rate /(1*KB)));

						CString mode_s;

						if(ds.peers_status[i].mode == Rage::LOCAL)
						{
								mode_s = _T("Local");
						}else
						{
								mode_s = _T("Remote");
						}

						ref_list.SetItemText(idx, 3, mode_s);
						ref_list.SetItemText(idx, 4, FLOAT_FMT(ds.peers_status[i].finished_degree * 100));
						++idx;
				}
		}

}

void CPeerView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
		// TODO: Add your specialized code here and/or call the base class
		
		CListCtrl &ref_list = GetListCtrl();
		
		Rage::Downloader *pcurr = (Rage::Downloader*)pHint;

		if(pcurr == 0 || pcurr->GetDownloaderState() != Rage::DS_RUNNING)
		{
				
				ref_list.DeleteAllItems();
		}else
		{
				if((UINT)lHint == CHANGED_FOCUS)
				{
						update_peers(pcurr);
						m_ts.Update();
				}
				
				if((UINT)lHint == UPDATE_TIMER && m_ts.ElapsedSecond() > 2)
				{
						update_peers(pcurr);
						m_ts.Update();
				}
		}

}
