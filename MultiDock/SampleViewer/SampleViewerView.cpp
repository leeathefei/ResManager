// SampleViewerView.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "SampleViewerView.h"
#include "SampleViewerManager.h"
#include "..\Common\XmlConfig.h"
#include "..\Common\WndManager.h"


IMPLEMENT_DYNCREATE(CSampleViewerView, CScrollView)

CSampleViewerView::CSampleViewerView()
{
}

CSampleViewerView::~CSampleViewerView()
{
}


BEGIN_MESSAGE_MAP(CSampleViewerView, CScrollView)
   ON_COMMAND(ID_SAMPLEVIEWER_ACTIVATEVERTICALPANE0, &CSampleViewerView::OnSampleviewerActivateVerticalPane0)
   ON_COMMAND(ID_SAMPLEVIEWER_ACTIVATEHORIZONTALPANE3, &CSampleViewerView::OnSampleviewerActivateHorizontalPane3)
   ON_COMMAND(ID_SAMPLEVIEWER_SHOWPANE, OnToolButton1)
   ON_COMMAND(ID_SAMPLEVIEWER_HIDEPANE,OnToolButton2)
   ON_COMMAND(ID_SAMPLEVIEWER_NEWVIEWER,OnToolButton3)
   ON_WM_RBUTTONUP()
   ON_WM_SIZE()
   ON_WM_DESTROY()
   ON_WM_CREATE()
END_MESSAGE_MAP()


// CSampleViewerView drawing
int CSampleViewerView::OnCreate(LPCREATESTRUCT lpcs)
{
	/*UINT nIndex = CWndManager::Instance()->GetNextViewIndex();
	CString strAddr;
	strAddr.Format(_T("0x%08x"), this);
	CString strNode;
	strNode.Format(_T("MainFrame\\View_%d\\Address"), nIndex);
	AppXml()->SetAttribute(strNode, strAddr);
	strNode.Format(_T("MainFrame\\View_%d\\ClassName"), nIndex);
	AppXml()->SetAttribute(strNode, _T("CSampleViewerView"));
	AppXml()->FlushData();*/

	if (CView::OnCreate(lpcs) == -1)
	{
		return -1;
	}

	CWndManager::Instance()->AddCreatedWnd(this, _T("CSampleViewerView"));

	return 0;
}

void CSampleViewerView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();


	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

   CRect rcClient;
   GetClientRect(&rcClient);
   m_ListCtrl.Create(LVS_REPORT, rcClient, this, 99);
   m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
   m_ListCtrl.ShowWindow(SW_SHOW);
   m_ListCtrl.InsertColumn(0, _T("Index"));
   m_ListCtrl.InsertColumn(1, _T("InstanceID"));
   m_ListCtrl.InsertColumn(2, _T("Protocol"));

}

void CSampleViewerView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}


// CSampleViewerView diagnostics

#ifdef _DEBUG
void CSampleViewerView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CSampleViewerView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


void CSampleViewerView::OnSampleviewerActivateVerticalPane0()
{
   CSampleViewerManager* pManager = CSampleViewerManager::Instance();
   pManager->ActivatePane(_T("����һ�Ĵ�ֱ�Զ�ɾ�����1"));
}

void CSampleViewerView::OnSampleviewerActivateHorizontalPane3()
{
   CSampleViewerManager* pManager = CSampleViewerManager::Instance();
   pManager->ActivatePane(_T("����һ�Ĵ�ֱ�ɹر����2"));
}

void CSampleViewerView::OnRButtonUp(UINT nFlags, CPoint point)
{
   // TODO: Add your message handler code here and/or call default

   AfxMessageBox(_T("CSampleViewerView"));
   CScrollView::OnRButtonUp(nFlags, point);
}

void CSampleViewerView::OnSize(UINT nType, int cx, int cy)
{
   CScrollView::OnSize(nType, cx, cy);

   if(m_ListCtrl.GetSafeHwnd())
   {
      CRect rc;
      GetClientRect(&rc);
      m_ListCtrl.MoveWindow(&rc, TRUE);
      m_ListCtrl.SetColumnWidth(0, 100);
      m_ListCtrl.SetColumnWidth(1, 100);
      m_ListCtrl.SetColumnWidth(2, 100);
   }
}

void CSampleViewerView::OnDestroy()
{
   CScrollView::OnDestroy();
}

void CSampleViewerView::OnToolButton1()
{
	AfxMessageBox(_T("Do something to response toolbar button 1."));
}

void CSampleViewerView::OnToolButton2()
{
	AfxMessageBox(_T("Do something to response toolbar button 2."));
}

void CSampleViewerView::OnToolButton3()
{
	AfxMessageBox(_T("Do something to response toolbar button 3."));
}