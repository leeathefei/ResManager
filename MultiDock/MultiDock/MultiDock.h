
// MultiDock.h : main header file for the MultiDock application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMultiDockApp:
// See MultiDock.cpp for the implementation of this class
//

class CMultiDockApp : public CWinAppEx
{
public:
	CMultiDockApp();


// Overrides
public:
	virtual BOOL InitInstance();
	//virtual int ExitInstance();
	//virtual int  Run();
	//virtual BOOL PumpMessage();     // low level message pump

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	CView* OpenView(const CString &strViewName, BOOL bSingle=FALSE);
	int    GetNumOfView(const CString& strViewName);
};

extern CMultiDockApp theApp;
BOOL AFXAPI MTDOCK_AfxInternalPumpMessage();
