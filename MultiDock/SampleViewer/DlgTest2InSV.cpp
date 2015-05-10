// DlgTest2InSV.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "DlgTest2InSV.h"
#include "afxdialogex.h"
#include "SampleViewerManager.h"

static CDlgTest2InSV::stRegister s_Register;

IMPLEMENT_DYNAMIC(CDlgTest2InSV, CDialogEx)

CDlgTest2InSV::CDlgTest2InSV(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTest2InSV::IDD, pParent)
{

}

CDlgTest2InSV::~CDlgTest2InSV()
{
}

void CDlgTest2InSV::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SV_TEST2, m_listsvtest2);
}

BOOL CDlgTest2InSV::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//1.Init dll name listctrl
	DWORD dwStyle = m_listsvtest2.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;    
	m_listsvtest2.SetExtendedStyle(dwStyle);
	m_listsvtest2.InsertColumn(0, _T("数据一"), LVCFMT_LEFT, 180);
	m_listsvtest2.InsertColumn(1, _T("数据二"), LVCFMT_LEFT, 100);
	m_listsvtest2.InsertColumn(2, _T("数据三"), LVCFMT_LEFT, 100);
	
	return TRUE;

}



BEGIN_MESSAGE_MAP(CDlgTest2InSV, CDialogEx)
END_MESSAGE_MAP()


// CDlgTest2InSV message handlers

CWnd* CDlgTest2InSV::CreateDlgObj()
{
	return new CDlgTest2InSV;
}
BOOL CDlgTest2InSV::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType)
{
	USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));
	__super::Create(IDD, pParent);
	if (eDockType == ALIGN_CHILD)
	{
		return TRUE;
	}
	else
	{
		return CSampleViewerManager::Instance()->RegisterDockPane(this, _T("DlgTest2InSV"),eDockType, TRUE);
	}
}