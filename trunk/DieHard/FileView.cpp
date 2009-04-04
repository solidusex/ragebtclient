#include "StdAfx.h"
#include "FileView.h"
#include "Util.h"
#include "DieHardDoc.h"

IMPLEMENT_DYNCREATE(CFileView, CReportView)

CFileView::CFileView(void)
{
}

CFileView::~CFileView(void)
{
}
BEGIN_MESSAGE_MAP(CFileView, CReportView)
		ON_WM_CREATE()
END_MESSAGE_MAP()

const TCHAR *file_str_arr[] = 
{
		_T("File Name"),
		_T("File Size"),
		_T("%")
};

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CReportView::OnCreate(lpCreateStruct) == -1)
				return -1;

		// TODO:  Add your specialized creation code here
		
		for(size_t i = 0; i < GET_ARRAY_SIZE(file_str_arr); ++i)
		{
				GetListCtrl().InsertColumn(i, file_str_arr[i], 0, 400);
		}
		return 0;
}

#if(0)
void CFileView::update_status()
{
		size_t idx = 0;
		CListCtrl &ref_list = GetListCtrl();

		CDieHardDoc *pdoc = (CDieHardDoc*)this->GetDocument();
		Rage::Downloader *pcurr = pdoc->GetFocusDownloader();
		assert(pcurr != 0);

		const Rage::TorrentFile &tf = ->GetTorrentFile();
		
		
}
#endif



void CFileView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
		// TODO: Add your specialized code here and/or call the base class
		
		DocUpdateHint hint = (DocUpdateHint)lHint;
		Rage::Downloader *pcurr = (Rage::Downloader*)pHint;
		CListCtrl &ref_list = GetListCtrl();

		if(pcurr == 0)
		{
				ref_list.DeleteAllItems();
				return;
		}

		switch(hint)
		{
		case CHANGED_FOCUS:
		{
				size_t idx = 0;
				Rage::FileStatusList file_status = pcurr->GetFilesStatus();
				const Rage::TorrentFile  &tf = pcurr->GetTorrentFile();

				for(Rage::FileStatusList::const_iterator it = file_status.begin(); it != file_status.end(); ++it, ++idx)
				{
						ref_list.InsertItem(idx, StrConv(tf.GetFileInfo(it->fidx).path));

						std::string str = FormatSpace::NumberFormatter::Format((double)tf.GetFileInfo(it->fidx).length / (double)(1*MB), 2);

						ref_list.SetItemText(idx, 1, StrConv(FormatSpace::AnsiConverter::ToUnicode(str)));

						str = FormatSpace::NumberFormatter::Format(it->finished_degree,2);

						ref_list.SetItemText(idx, 2, StrConv(FormatSpace::AnsiConverter::ToUnicode(str)));
				}

				m_ts.Update();
				break;
		}
		case UPDATE_TIMER:
		{
				if(m_ts.ElapsedSecond() > 10)
				{
						Rage::FileStatusList file_status = pcurr->GetFilesStatus();
						const Rage::TorrentFile  &tf = pcurr->GetTorrentFile();

						Rage::FileStatusList::const_iterator it = file_status.begin();

						for(size_t idx = 0; idx < ref_list.GetItemCount(); ++idx)
						{
								std::string str = FormatSpace::NumberFormatter::Format((double)tf.GetFileInfo(it->fidx).length / (double)(1*MB), 2);

								ref_list.SetItemText(idx, 1, StrConv(FormatSpace::AnsiConverter::ToUnicode(str)));

								str = FormatSpace::NumberFormatter::Format(it->finished_degree,2);

								ref_list.SetItemText(idx, 2, StrConv(FormatSpace::AnsiConverter::ToUnicode(str)));
								++it;
						}
						m_ts.Update();
				}
				break;
		}
		}

		

}

void CFileView::OnInitialUpdate()
{
		CReportView::OnInitialUpdate();

		// TODO: Add your specialized code here and/or call the base class
}
