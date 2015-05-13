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
	DDX_Control(pDX, IDC_LIST_SV_TEST41, m_listsvtest11);
	DDX_Control(pDX, IDC_LIST_SV_TEST42, m_listsvtest12);
}

BOOL CDlgTest4InSV::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//1.Init dll name listctrl
	DWORD dwStyle = m_listsvtest11.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;    
	m_listsvtest11.SetExtendedStyle(dwStyle);
	m_listsvtest11.InsertColumn(0, _T("数据一"), LVCFMT_LEFT, 180);
	m_listsvtest11.InsertColumn(1, _T("数据二"), LVCFMT_LEFT, 100);

	dwStyle = m_listsvtest12.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;    
	m_listsvtest12.SetExtendedStyle(dwStyle);
	m_listsvtest12.InsertColumn(0, _T("数据一"), LVCFMT_LEFT, 180);
	m_listsvtest12.InsertColumn(1, _T("数据二"), LVCFMT_LEFT, 100);
	m_listsvtest12.InsertColumn(0, _T("数据三"), LVCFMT_LEFT, 180);
	m_listsvtest12.InsertColumn(1, _T("数据四"), LVCFMT_LEFT, 100);
	return TRUE;

}


void CDlgTest4InSV::OnClose()
{
	CWndManager::Instance()->RemoveCreatedWnd(this/*, _T("CDlgTest4InSV")*/);
	CDialogEx::OnClose();
}

BEGIN_MESSAGE_MAP(CDlgTest4InSV, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

CWnd* CDlgTest4InSV::CreateDlgObj()
{
	return new CDlgTest4InSV;
}

BOOL CDlgTest4InSV::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName)
{
	USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));
	__super::Create(IDD, pParent);

	if (eDockType == ALIGN_CHILD_WITH_TITLE)
	{
		return TRUE;
	}
	else
	{
		ModifyStyle(WS_BORDER|WS_CAPTION|WS_SIZEBOX,0);
		return CSampleViewerManager::Instance()->RegisterDockPane(this, strWndName.GetString(),eDockType, TRUE);
	}
}
// CDlgTest4InSV message handlers
