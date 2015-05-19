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
#include "SamplePanelView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static UINT indicators[] =
{
	ID_SEPARATOR       // status line indicator
};

// CSamplePanelFrame

IMPLEMENT_DYNCREATE(CSamplePanelFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CSamplePanelFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

CSamplePanelFrame::CSamplePanelFrame()
{
	CSamplePanelManager::Instance()->AddFrameWnd(this);
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
	}

	if(!bActivate&&pDeactivateWnd==this)
	{
		pDllManager->ShowToolBar(FALSE);
	}

	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

//if tab was closed, and postNcDestroy will be called.
void CSamplePanelFrame::OnClose()
{	
	//这个是否可以考虑移到PostNcDestroy函数里面去
	CSamplePanelView* pView = (CSamplePanelView*)GetActiveView();
	if (NULL != pView)
	{
		pView->SendMessage(WM_CLOSE);
	}

	CMDIChildWndEx::OnClose();
}

void CSamplePanelFrame::PostNcDestroy()
{
	//Control the toolbar's refer_counter
	CSamplePanelManager::Instance()->RemoveFrameWnd(this);
	__super::PostNcDestroy();
}


int CSamplePanelFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	USE_CUSTOM_RESOURCE(_T("SamplePanel.dll"));
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		return -1;
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	m_wndStatusBar.SetPaneText(0, _T("这是工程二的状态栏"));

	return 0;
}