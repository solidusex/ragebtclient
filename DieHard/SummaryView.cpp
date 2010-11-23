#include "StdAfx.h"
#include "SummaryView.h"
#include "Util.h"
#include "DieHardDoc.h"

IMPLEMENT_DYNCREATE(CSummaryView, CReportView)


CSummaryView::CSummaryView(void)
{
}

CSummaryView::~CSummaryView(void)
{
}
BEGIN_MESSAGE_MAP(CSummaryView, CReportView)
		ON_WM_CREATE()
END_MESSAGE_MAP()


const TCHAR *summary_str_arr[] = 
{
		_T("Item"),
		_T("Value")
};


int CSummaryView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CReportView::OnCreate(lpCreateStruct) == -1)
				return -1;

		// TODO:  Add your specialized creation code here

		for(size_t i = 0; i < GET_ARRAY_SIZE(summary_str_arr); ++i)
		{
				GetListCtrl().InsertColumn(i, summary_str_arr[i], 0, 400);
		}
		
		GetListCtrl().InsertItem(0, _T("Torrent Name"));
		GetListCtrl().InsertItem(1, _T("Save Location"));
		GetListCtrl().InsertItem(2, _T("InfoHash"));
		GetListCtrl().InsertItem(3, _T("Piece Length"));
		GetListCtrl().InsertItem(4, _T("Progress"));
		GetListCtrl().InsertItem(5, _T("Remaining Size"));
		GetListCtrl().InsertItem(6, _T("Total Size"));
		return 0;
}


#define INT_FMT(val) StrConv(FormatSpace::AnsiConverter::ToUnicode(FormatSpace::NumberFormatter::Format(val)))

#define FLOAT_FMT(val) StrConv(FormatSpace::AnsiConverter::ToUnicode(FormatSpace::NumberFormatter::Format(val,2)))


void CSummaryView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
		// TODO: Add your specialized code here and/or call the base class
		
		CDieHardDoc *pdoc = (CDieHardDoc*)this->GetDocument();

		ASSERT(pdoc != NULL);
		
		Rage::Downloader *pcurr = pdoc->GetFocusDownloader();
		
		CListCtrl &ref_list = GetListCtrl();
		
		if(pcurr == 0 )
		{
				ref_list.SetItemText(0, 1, _T(""));
				ref_list.SetItemText(1, 1, _T(""));
				ref_list.SetItemText(2, 1, _T(""));
				ref_list.SetItemText(3, 1, _T(""));
				ref_list.SetItemText(4, 1, _T(""));
				ref_list.SetItemText(5, 1, _T(""));
				ref_list.SetItemText(6, 1, _T(""));
		}else
		{
				
				const Rage::TorrentFile &rtf = pcurr->GetTorrentFile();

				assert(pcurr->IsOpen());
				
				ref_list.SetItemText(0, 1, StrConv(rtf.GetTorrentName()));
				
				ref_list.SetItemText(1, 1, StrConv(pcurr->GetSavePath()));

				ref_list.SetItemText(2, 1, StrConv(FormatSpace::AnsiConverter::ToUnicode(rtf.GetInfoHash().ToHexString())));

				ref_list.SetItemText(3, 1, INT_FMT(rtf.GetPieceLength() / KB) + _T(" Kb"));
				
				ref_list.SetItemText(4, 1, FLOAT_FMT(pcurr->GetFinishedDegree() * 100));
				
				ref_list.SetItemText(5, 1, FLOAT_FMT((1.0f - (double)pcurr->GetFinishedDegree()) * (double)rtf.GetTotalSize() / MB) + _T(" Mb"));

				ref_list.SetItemText(6, 1, FLOAT_FMT((double)rtf.GetTotalSize() / MB) + _T(" Mb"));

		}

}

