
// IRSBatch.h : main header file for the IRSBatch application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIRSBatchApp:
// See IRSBatch.cpp for the implementation of this class
//

class CIRSBatchApp : public CWinApp
{
public:
	CIRSBatchApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();

  afx_msg void HandleFileNew();
  afx_msg void HandleFileOpen();
  afx_msg void HandleFileSave();
	DECLARE_MESSAGE_MAP()
};

extern CIRSBatchApp theApp;
