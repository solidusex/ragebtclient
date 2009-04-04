#pragma once
#include "afxcview.h"

class CReportView :	public CListView
{
public:
		DECLARE_DYNCREATE(CReportView)
public:
		CReportView(void);
		virtual ~CReportView(void);
protected:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
		DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
