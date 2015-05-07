// DlgTest3InSV.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "DlgTest3InSV.h"
#include "afxdialogex.h"

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

BOOL CDlgTest3InSV::CreateWnd(CWnd* pParent)
{
	return __super::Create(IDD, pParent);
}