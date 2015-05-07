// Utility.h : main header file for the Utility DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CUtilityApp
// See Utility.cpp for the implementation of this class
//

class CUtilityApp : public CWinApp
{
public:
	CUtilityApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int  ExitInstance();

	DECLARE_MESSAGE_MAP()
};
