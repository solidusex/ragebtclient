// DieHard.h : main header file for the DieHard application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDieHardApp:
// See DieHard.cpp for the implementation of this class
//

class CDieHardApp : public CWinApp
{
public:
	CDieHardApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
//	virtual int ExitInstance();
};

extern CDieHardApp theApp;