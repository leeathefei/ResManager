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

// SamplePanelView.cpp : implementation of the CSamplePanelView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SamplePanel.h"
#endif

#include "SamplePanelDoc.h"
#include "SamplePanelView.h"
#include "..\Common\WndManager.h"
#include "..\Common\XmlConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSamplePanelView

IMPLEMENT_DYNCREATE(CSamplePanelView, CView)

BEGIN_MESSAGE_MAP(CSamplePanelView, CView)
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_COMMAND(ID_SAMPLEPANEL_SHOWPANE, OnToolButton1)
	ON_COMMAND(ID_SAMPLEPANEL_HIDEPANE,OnToolButton2)
	ON_COMMAND(ID_SAMPLEPANEL_NEWVIEWER,OnToolButton3)
END_MESSAGE_MAP()

CSamplePanelView::CSamplePanelView()
{

}

CSamplePanelView::~CSamplePanelView()
{
}

void CSamplePanelView::OnClose()
{
	CWndManager::Instance()->RemoveCreatedWnd(this);
}

void CSamplePanelView::OnToolButton1()
{
	AfxMessageBox(_T("Do something to response toolbar button 1."));
}

void CSamplePanelView::OnToolButton2()
{
	AfxMessageBox(_T("Do something to response toolbar button 2."));
}

void CSamplePanelView::OnToolButton3()
{
	AfxMessageBox(_T("Do something to response toolbar button 3."));
}
// CSamplePanelView drawing

void CSamplePanelView::OnDraw(CDC* /*pDC*/)
{
	CSamplePanelDoc* pDoc = GetDocument();
// 	ASSERT_VALID(pDoc);
// 	if (!pDoc)
// 		return;
}

void CSamplePanelView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

int CSamplePanelView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
	{
		return -1;
	}

	CString strName(_T("工程二View窗口"));
	CWndManager::Instance()->AddCreatedWnd(this, _T("CSamplePanelView"), strName);

	return 0;
}



// CSamplePanelView diagnostics

#ifdef _DEBUG
void CSamplePanelView::AssertValid() const
{
	CView::AssertValid();
}

void CSamplePanelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSamplePanelDoc* CSamplePanelView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSamplePanelDoc)));
	return (CSamplePanelDoc*)m_pDocument;
}
#endif //_DEBUG


// CSamplePanelView message handlers
