#pragma once
#include "afxcview.h"
#include "ReportView.h"

class CPeerView : public CReportView
{
public:
		DECLARE_DYNCREATE(CPeerView)
public:
		CPeerView(void);
		virtual ~CPeerView(void);
		DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
		virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
private:
		TimeSpace::TimeStamp	m_ts;
private:
		void update_peers(Rage::Downloader *pcurr);
		
};


