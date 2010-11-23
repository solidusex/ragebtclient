#pragma once
#include "afxcview.h"

class CSelectView : public CListView
{
public:
		DECLARE_DYNCREATE(CSelectView)
public:
		CSelectView(void);

		virtual ~CSelectView(void);
protected:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
		DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};
