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

void CDlgTest1InSV::OnClose()
{
	CWndManager::Instance()->RemoveCreatedWnd(this/*, _T("CDlgTest1InSV")*/);
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnClose();
}

void CDlgTest1InSV::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SV_TEST1, m_listsvtest1);
}

BOOL CDlgTest1InSV::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//1.Init dll name listctrl
	DWORD dwStyle = m_listsvtest1.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;    
	m_listsvtest1.SetExtendedStyle(dwStyle);
	m_listsvtest1.InsertColumn(0, _T("数据一"), LVCFMT_LEFT, 180);
	m_listsvtest1.InsertColumn(1, _T("数据二"), LVCFMT_LEFT, 100);

	return TRUE;

}

void CDlgTest1InSV::OnDestroy()
{
	CDialogEx::OnDestroy();
}


BEGIN_MESSAGE_MAP(CDlgTest1InSV, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


CWnd* CDlgTest1InSV::CreateDlgObj()
{
	return new CDlgTest1InSV;
}

BOOL CDlgTest1InSV::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName, BOOL bAutoDelete,CRect rect)
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
		ModifyStyle(WS_CAPTION|WS_BORDER|WS_SIZEBOX,0);
		return CSampleViewerManager::Instance()->RegisterDockPane(this, strWndName.GetString(),eDockType, bAutoDelete,rect);
	}
}