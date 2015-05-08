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

IMPLEMENT_DYNCREATE(CSamplePanelView, CScrollView)

BEGIN_MESSAGE_MAP(CSamplePanelView, CScrollView)
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

CSamplePanelView::CSamplePanelView()
{
	// TODO: add construction code here

}

CSamplePanelView::~CSamplePanelView()
{
}

// CSamplePanelView drawing

void CSamplePanelView::OnDraw(CDC* /*pDC*/)
{
	CSamplePanelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSamplePanelView printing

void CSamplePanelView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

int CSamplePanelView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CScrollView::OnCreate(lpcs) == -1)
	{
		return -1;
	}

	UINT nIndex = CWndManager::Instance()->GetNextViewIndex();
	CString strAddr;
	strAddr.Format(_T("0x%08x"), this);
	CString strNode;
	strNode.Format(_T("MainFrame\\View_%d\\Address"), nIndex);
	AppXml()->SetAttribute(strNode, strAddr);
	strNode.Format(_T("MainFrame\\View_%d\\ClassName"), nIndex);
	AppXml()->SetAttribute(strNode, _T("CSamplePanelView"));
	AppXml()->FlushData();


	return 0;
}



// CSamplePanelView diagnostics

#ifdef _DEBUG
void CSamplePanelView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSamplePanelView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSamplePanelDoc* CSamplePanelView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSamplePanelDoc)));
	return (CSamplePanelDoc*)m_pDocument;
}
#endif //_DEBUG


// CSamplePanelView message handlers
