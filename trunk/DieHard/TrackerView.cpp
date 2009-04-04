#include "StdAfx.h"
#include "TrackerView.h"
#include "Util.h"
#include "DieHardDoc.h"

IMPLEMENT_DYNCREATE(CTrackerView, CReportView)

CTrackerView::CTrackerView(void)
{
}

CTrackerView::~CTrackerView(void)
{
}
BEGIN_MESSAGE_MAP(CTrackerView, CReportView)
		ON_WM_CREATE()
END_MESSAGE_MAP()

const TCHAR *tracker_str_arr[] = 
{
		_T("Tracker Url"), _T("Last Status")
		
};

int CTrackerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CReportView::OnCreate(lpCreateStruct) == -1)
				return -1;

		// TODO:  Add your specialized creation code here

		for(size_t i = 0; i < GET_ARRAY_SIZE(tracker_str_arr); ++i)
		{
				GetListCtrl().InsertColumn(i, tracker_str_arr[i], 0, 400);
		}



		return 0;
}


#define STR_TO_CSTR(str) (StrConv(FormatSpace::AnsiConverter::ToUnicode(str)))

void CTrackerView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
		// TODO: Add your specialized code here and/or call the base class

		Rage::Downloader *pcurr = (Rage::Downloader *)pHint;

		CListCtrl &ref_list = GetListCtrl();

		if(pcurr == 0)
		{
				ref_list.DeleteAllItems();
				m_tracker_map.clear();
				
		}else
		{
				const Rage::TorrentFile &tf = pcurr->GetTorrentFile();
				
				if((UINT)lHint == CHANGED_FOCUS)
				{
						m_tracker_map.clear();
						ref_list.DeleteAllItems();

						for(size_t i = 0; i < tf.GetAnnounceCount(); ++i)
						{
								m_tracker_map[STR_TO_CSTR(tf.GetAnnounce(i))] = _T("Stop");
						}
						
						size_t idx = 0;
						for(TrackerMap::const_iterator it = m_tracker_map.begin(); it != m_tracker_map.end(); ++it, ++idx)
						{
								ref_list.InsertItem(idx, it->first);
						}
				}

				if(pcurr->GetDownloaderState() == Rage::DS_RUNNING)
				{
						Rage::DownStatus ds = pcurr->GetDownStatus();
						
						for(size_t i = 0; i < ds.trackers_status.size(); ++i)
						{
								CString url = STR_TO_CSTR(ds.trackers_status[i].first);

								TrackerMap::iterator it = m_tracker_map.find(url);
								if(it != m_tracker_map.end())
								{
										it->second = STR_TO_CSTR(ds.trackers_status[i].second);
								}
						}
				}

				size_t idx = 0;
				for(TrackerMap::const_iterator it = m_tracker_map.begin(); it != m_tracker_map.end(); ++it, ++idx)
				{
						ref_list.SetItemText(idx, 1, it->second);
				}
		}



}
