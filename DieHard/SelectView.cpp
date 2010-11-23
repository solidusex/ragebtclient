#include "StdAfx.h"
#include "SplitterWndEx.h"
#include "SelectView.h"
#include "FileView.h"
#include "PeerView.h"
#include "SummaryView.h"
#include "TrackerView.h"
#include "DieHardDoc.h"

IMPLEMENT_DYNCREATE(CSelectView, CListView)


CSelectView::CSelectView(void)
{
}

CSelectView::~CSelectView(void)
{
}

BOOL CSelectView::PreCreateWindow(CREATESTRUCT& cs)
{
		// TODO: Add your specialized code here and/or call the base class
		cs.style |= (LVS_ALIGNLEFT|LVS_LIST|LVS_SINGLESEL);

		return CListView::PreCreateWindow(cs);
}
BEGIN_MESSAGE_MAP(CSelectView, CListView)
		ON_WM_CREATE()
		ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CSelectView::OnLvnItemchanged)
END_MESSAGE_MAP()

int CSelectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CListView::OnCreate(lpCreateStruct) == -1)
				return -1;

		// TODO:  Add your specialized creation code here

		// TODO:  Add your specialized creation code here
		GetListCtrl().ModifyStyle(0x0001,0);
		
		GetListCtrl().InsertItem(0, _T("Summary"));
		GetListCtrl().InsertItem(1, _T("Peers"));
		GetListCtrl().InsertItem(2, _T("Files"));
		GetListCtrl().InsertItem(3, _T("Trackers"));
		return 0;
}



CRuntimeClass* rt_arr[] = 
{
		RUNTIME_CLASS(CSummaryView),
		RUNTIME_CLASS(CPeerView),
		RUNTIME_CLASS(CFileView),
		RUNTIME_CLASS(CTrackerView)
};


void CSelectView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		// TODO: Add your control notification handler code here
		*pResult = 0;

		if(pNMLV->uChanged == LVIF_STATE)
		{
				if(pNMLV->uNewState)
				{
						
						POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();

						int idx = GetListCtrl().GetNextSelectedItem(pos);		
						if(idx != -1 && idx < 4)
						{
								CSplitterWndEx *pspwnd = (CSplitterWndEx*)(this->GetParent());
								
								pspwnd->ReplaceView(0,1, rt_arr[idx], CSize(0,0));
								GetDocument()->UpdateAllViews(this, CHANGED_FOCUS, (CObject*)((CDieHardDoc*)GetDocument())->GetFocusDownloader());
						}
				}else
				{
						//MessageBox(_T("选择改变且没有选中的行\r\n"));
				}
		}else
		{
				//MessageBox(_T("行改变(CListCtrl::SetItem)\r\n"));
		}

}
