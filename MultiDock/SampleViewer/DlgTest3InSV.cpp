// DlgTest3InSV.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "DlgTest3InSV.h"
#include "afxdialogex.h"
#include "SampleViewerManager.h"


static CDlgTest3InSV::stRegister s_Register;
// CDlgTest3InSV dialog

IMPLEMENT_DYNAMIC(CDlgTest3InSV, CDialogEx)

CDlgTest3InSV::CDlgTest3InSV(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTest3InSV::IDD, pParent)
{

}

CDlgTest3InSV::~CDlgTest3InSV()
{
}

void CDlgTest3InSV::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CDlgTest3InSV::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	return TRUE;
}


void CDlgTest3InSV::OnClose()
{
	CWndManager::Instance()->RemoveCreatedWnd(this/*, _T("CDlgTest3InSV")*/);
	CDialogEx::OnClose();
}

BEGIN_MESSAGE_MAP(CDlgTest3InSV, CDialogEx)
END_MESSAGE_MAP()


// CDlgTest3InSV message handlers
CWnd* CDlgTest3InSV::CreateDlgObj()
{
	return new CDlgTest3InSV;
}

BOOL CDlgTest3InSV::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName,BOOL bAutoDelete,CRect rect)
{
	USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));
	__super::Create(IDD, pParent);
	if (eDockType == ALIGN_CHILD_WITH_TITLE)
	{
		return TRUE;
	}
	else if (eDockType == ALIGN_CHILD_WITH_NO_TITLE)
	{
		ModifyStyle(WS_CAPTION|WS_BORDER|WS_SIZEBOX,0);
		return TRUE;
	}
	else
	{
		ModifyStyle(WS_BORDER|WS_CAPTION|WS_SIZEBOX,0);
		return CSampleViewerManager::Instance()->RegisterDockPane(this, strWndName.GetString(),eDockType, bAutoDelete,rect);
	}
	
}