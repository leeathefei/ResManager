// SampleViewerFrame.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "SampleViewerFrame.h"
#include "SampleViewerManager.h"
#include "SampleViewerDialog.h"
#include "SampleViewerView.h"

// CSampleViewerFrame

IMPLEMENT_DYNCREATE(CSampleViewerFrame, CMDIChildWndEx)

CSampleViewerFrame::CSampleViewerFrame()
{
   CSampleViewerManager::Instance()->AddFrameWnd(this);
}

CSampleViewerFrame::~CSampleViewerFrame()
{
}

static UINT indicators[] =
{
	ID_SEPARATOR       // status line indicator
};



BEGIN_MESSAGE_MAP(CSampleViewerFrame, CMDIChildWndEx)
   ON_WM_MDIACTIVATE()
   ON_WM_CLOSE()
   ON_WM_CREATE()
END_MESSAGE_MAP()



// CSampleViewerFrame message handlers

void CSampleViewerFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
   CSampleViewerManager* pDllManager = CSampleViewerManager::Instance();

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
//tab was closed.and postNcDestroy will be called.
void CSampleViewerFrame::OnClose()
{
   //���ʱ��view����û�����٣�������ǣ�����Ƿ���Կ����Ƶ�PostNcDestroy��������ȥ
	CSampleViewerView* pView = (CSampleViewerView*)GetActiveView();
	if (NULL != pView)
	{
		pView->SendMessage(WM_CLOSE);
	}

   //CSampleViewerManager::Instance()->UnregisterModulePane( _T("Vertical Pane 1"));
   //CSampleViewerManager::Instance()->UnregisterModulePane(_T("Vertical Pane 0"));
   //CSampleViewerManager::Instance()->UnregisterModulePane(_T("Horizontal Pane 2"));
   //CSampleViewerManager::Instance()->UnregisterModulePane(_T("Horizontal Pane 3"));

   CMDIChildWndEx::OnClose();
}

int CSampleViewerFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"))
   if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
      return -1;

	if (!m_wndStatusBar.Create(this))
	{
		return -1;
	}

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	m_wndStatusBar.SetPaneText(0, _T("���ǹ���һ��״̬��"));

   return 0;
}


void CSampleViewerFrame::PostNcDestroy()
{
	CSampleViewerManager::Instance()->RemoveFrameWnd(this);
	__super::PostNcDestroy();
}