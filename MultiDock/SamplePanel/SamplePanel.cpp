// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// SamplePanel.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SamplePanel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSamplePanelApp, CWinAppEx)
END_MESSAGE_MAP()

CSamplePanelApp::CSamplePanelApp()
{

}

// The one and only CSamplePanelApp object

CSamplePanelApp theApp;


// CSamplePanelApp initialization

BOOL CSamplePanelApp::InitInstance()
{
	CWinAppEx::InitInstance();

	return TRUE;
}

