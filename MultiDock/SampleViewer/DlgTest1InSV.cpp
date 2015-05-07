// DlgTest1InSV.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "DlgTest1InSV.h"
#include "afxdialogex.h"
#include "SampleViewerManager.h"

static CDlgTest1InSV::stRegister s_Register;

IMPLEMENT_DYNAMIC(CDlgTest1InSV, CDialogEx)

CDlgTest1InSV::CDlgTest1InSV(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTest1InSV::IDD, pParent)
{

}

CDlgTest1InSV::~CDlgTest1InSV()
{
}

void CDlgTest1InSV::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTest1InSV, CDialogEx)
END_MESSAGE_MAP()


CWnd* CDlgTest1InSV::CreateDlgObj()
{
	return new CDlgTest1InSV;
}

BOOL CDlgTest1InSV::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType)
{
	USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));
	__super::Create(IDD, pParent);
	return CSampleViewerManager::Instance()->RegisterDockPane(this, _T("DlgTest1InSV"),eDockType, TRUE);
}