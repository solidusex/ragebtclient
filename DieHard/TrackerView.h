#pragma once
#include "afxcview.h"
#include "ReportView.h"

class CTrackerView : public CReportView
{
public:
		DECLARE_DYNCREATE(CTrackerView)
public:
		CTrackerView(void);
		virtual ~CTrackerView(void);
		DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
		virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

		typedef std::map<CString, CString>		TrackerMap;
		TrackerMap								m_tracker_map;
};
