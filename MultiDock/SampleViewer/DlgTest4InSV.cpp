// DlgTest4InSV.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "DlgTest4InSV.h"
#include "afxdialogex.h"
#include "SampleViewerManager.h"


static CDlgTest4InSV::stRegister s_register;

IMPLEMENT_DYNAMIC(CDlgTest4InSV, CDialogEx)

CDlgTest4InSV::CDlgTest4InSV(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTest4InSV::IDD, pParent)
{

}

CDlgTest4InSV::~CDlgTest4InSV()
{
}

void CDlgTest4InSV::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTest4InSV, CDialogEx)
END_MESSAGE_MAP()

CWnd* CDlgTest4InSV::CreateDlgObj()
{
	return new CDlgTest4InSV;
}

BOOL CDlgTest4InSV::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType)
{
	USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));
	__super::Create(IDD, pParent);

	return CSampleViewerManager::Instance()->RegisterDockPane(this, _T("DlgTest4InSV"),eDockType, TRUE);
}
// CDlgTest4InSV message handlers
