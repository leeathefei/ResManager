// DlgCreateFloatPane.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgCreateFloatPane.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "..\Common\WndManager.h"


// CDlgCreateFloatPane dialog

IMPLEMENT_DYNAMIC(CDlgCreateFloatPane, CDialogEx)

CDlgCreateFloatPane::CDlgCreateFloatPane(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateFloatPane::IDD, pParent)
	, m_strEditClassname(_T(""))
{

}

CDlgCreateFloatPane::~CDlgCreateFloatPane()
{
}

void CDlgCreateFloatPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboParentWnd);
	DDX_Control(pDX, IDC_EDIT1, m_editBeCreatedClassname);
	DDX_Text(pDX, IDC_EDIT1, m_strEditClassname);
}


BEGIN_MESSAGE_MAP(CDlgCreateFloatPane, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CREATE_FLOATPANE, &CDlgCreateFloatPane::OnBnClickedBtnCreateFloatpane)
END_MESSAGE_MAP()


// CDlgCreateFloatPane message handlers


void CDlgCreateFloatPane::OnBnClickedBtnCreateFloatpane()
{
	//test interface.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->CreateDockWnd(_T("SampleViewer"), _T("CDlgTest4InSV"), ALIGN_NON);
	pFrame->CreateDockWnd(_T("SampleViewer"), _T("CDlgTest1InSV"), ALIGN_HORIZONTAL);

	pFrame->CreateDockWnd(_T("SamplePanel"), _T("CSamplePanelDialog"), ALIGN_HORIZONTAL);
	pFrame->CreateDockWnd(_T("SamplePanel"), _T("CSamplePanelDialog4"), ALIGN_VERTICAL);
}

void CDlgCreateFloatPane::UpdateClassName(CString& strClass)
{
	m_strEditClassname = strClass;

	UpdateData(FALSE);
}