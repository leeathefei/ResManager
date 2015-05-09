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


BEGIN_MESSAGE_MAP(CDlgTest3InSV, CDialogEx)
END_MESSAGE_MAP()


// CDlgTest3InSV message handlers
CWnd* CDlgTest3InSV::CreateDlgObj()
{
	return new CDlgTest3InSV;
}

BOOL CDlgTest3InSV::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType)
{
	USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));
	__super::Create(IDD, pParent);
	if (eDockType == ALIGN_CHILD)
	{
		return TRUE;
	}
	else
	{
		return CSampleViewerManager::Instance()->RegisterDockPane(this, _T("DlgTest3InSV"),eDockType, TRUE);
	}
	
}