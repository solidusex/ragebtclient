#pragma once
#include "afxcview.h"
#include "ReportView.h"

class CFileView :	public CReportView
{
public:
		DECLARE_DYNCREATE(CFileView)
public:
		CFileView(void);
		virtual ~CFileView(void);
		DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
		virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
		virtual void OnInitialUpdate();

private:
		TimeSpace::TimeStamp	m_ts;
};
