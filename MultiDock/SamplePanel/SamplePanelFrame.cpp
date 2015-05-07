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

// MainFrm.cpp : implementation of the CSamplePanelFrame class
//

#include "stdafx.h"
#include "SamplePanel.h"
#include "SamplePanelFrame.h"
#include "SamplePanelManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSamplePanelFrame

IMPLEMENT_DYNCREATE(CSamplePanelFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CSamplePanelFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

CSamplePanelFrame::CSamplePanelFrame()
{
	//CSamplePanelManager::Instance()->AddFrameWnd(this);
}

CSamplePanelFrame::~CSamplePanelFrame()
{
}

void CSamplePanelFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CSamplePanelManager* pDllManager = CSamplePanelManager::Instance();

	if(bActivate&&pActivateWnd==this)
	{
		pDllManager->ShowToolBar(TRUE);
		pDllManager->ActivatePane(_T("Vertical Pane 0"));
		pDllManager->ActivatePane(_T("Horizontal Pane 3"));
	}

	if(!bActivate&&pDeactivateWnd==this)
	{
		pDllManager->ShowToolBar(FALSE);
	}

	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

void CSamplePanelFrame::OnClose()
{
	//Control the toolbar's refer_counter
	//CSamplePanelManager::Instance()->RemoveFrameWnd(this);

	
	CMDIChildWndEx::OnClose();
}


int CSamplePanelFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}