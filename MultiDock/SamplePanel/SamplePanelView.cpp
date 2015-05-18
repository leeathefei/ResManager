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
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_COMMAND(ID_SAMPLEPANEL_SHOWPANE, OnToolButton1)
	ON_COMMAND(ID_SAMPLEPANEL_HIDEPANE,OnToolButton2)
	ON_COMMAND(ID_SAMPLEPANEL_NEWVIEWER,OnToolButton3)
	ON_COMMAND(ID_SAMPLEPANEL_ACTIVATEVERTICALPANE0, OnResponseMenuItem1)
	ON_COMMAND(ID_SAMPLEPANEL_ACTIVATEHORIZONTALPANE3, OnResponseMenuItem2)
	ON_COMMAND(ID_TEST_ITEM1, &CSamplePanelView::OnTestItem1)
	ON_COMMAND(ID_TEST_ITEM2, &CSamplePanelView::OnTestItem2)
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

void CSamplePanelView::OnResponseMenuItem1()
{
	AfxMessageBox(_T("响应工程二菜单1!"));
}

void CSamplePanelView::OnResponseMenuItem2()
{
	AfxMessageBox(_T("响应工程二菜单2!"));
}

void CSamplePanelView::OnToolButton1()
{
	AfxMessageBox(_T("响应工程二toolbar button 1."));
}

void CSamplePanelView::OnToolButton2()
{
	AfxMessageBox(_T("响应工程二toolbar button 2."));
}

void CSamplePanelView::OnToolButton3()
{
	AfxMessageBox(_T("响应工程二toolbar button 3."));
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
	CWndManager::Instance()->AddCreatedWnd(this, _T("CSamplePanelView"), strName, _T("TestProj2.dll"));

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


void CSamplePanelView::OnTestItem1()
{
	AfxMessageBox(_T("工程二测试菜单一"));
}


void CSamplePanelView::OnTestItem2()
{
	AfxMessageBox(_T("工程二测试菜单二"));
	// TODO: Add your command handler code here
}
void CSamplePanelView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	AdjustLayout();
}
void CSamplePanelView::AdjustLayout()
{
	ListChildWnd listChilds;
	if (CWndManager::Instance()->GetChildWnds(this, listChilds))
	{
		for (ListChildWnd::iterator it = listChilds.begin();
			it != listChilds.end(); ++it)
		{
			stChildWnd& oneItem = *it;
			CWnd* pWnd = oneItem.pChild;
			CRect& rect = oneItem.rcChild;
			if (NULL != pWnd && NULL != pWnd->GetSafeHwnd())
			{
				pWnd->MoveWindow(&rect,TRUE);
			}
		}
	}
}
void CSamplePanelView::OnDestroy()
{
	__super::OnDestroy();
}

