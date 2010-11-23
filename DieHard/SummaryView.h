#pragma once
#include "afxcview.h"
#include "ReportView.h"

class CSummaryView : public CReportView
{
public:
		DECLARE_DYNCREATE(CSummaryView)
public:
		CSummaryView(void);
		virtual ~CSummaryView(void);
		DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
		virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

public:


};
