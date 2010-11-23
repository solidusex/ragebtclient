#include "StdAfx.h"
#include "ReportView.h"


IMPLEMENT_DYNCREATE(CReportView, CListView)


CReportView::CReportView(void)
{
}

CReportView::~CReportView(void)
{
}

BOOL CReportView::PreCreateWindow(CREATESTRUCT& cs)
{
		// TODO: Add your specialized code here and/or call the base class
		cs.style |= (LVS_REPORT | LVS_SINGLESEL |LVS_SHOWSELALWAYS|LVS_ALIGNLEFT);
		return CListView::PreCreateWindow(cs);
}
BEGIN_MESSAGE_MAP(CReportView, CListView)
		ON_WM_CREATE()
END_MESSAGE_MAP()

int CReportView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CListView::OnCreate(lpCreateStruct) == -1)
				return -1;
		// TODO:  Add your specialized creation code here
		GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

		return 0;
}
