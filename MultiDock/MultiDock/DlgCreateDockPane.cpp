// DlgCreateDockPane.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgCreateDockPane.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgCreateDockPane dialog

IMPLEMENT_DYNAMIC(CDlgCreateDockPane, CDialogEx)

CDlgCreateDockPane::CDlgCreateDockPane(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateDockPane::IDD, pParent)
	, m_strClassname(_T(""))
	, m_nRadioDirection(0)
{
	CWndManager::Instance()->AddEventHandler(this);
}

CDlgCreateDockPane::~CDlgCreateDockPane()
{
}

void CDlgCreateDockPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_dockDirection);
	DDX_Text(pDX, IDC_EDIT_CLASSNAME_INDOCK, m_strClassname);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadioDirection);
	DDX_Control(pDX, IDC_LIST_PARENTWNDINDOCKPANE, m_listParentsInDock);
}

BOOL CDlgCreateDockPane::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//1.Init dll name listctrl
	DWORD dwStyle = m_listParentsInDock.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;    
	m_listParentsInDock.SetExtendedStyle(dwStyle);
	m_listParentsInDock.InsertColumn(0, _T("ClassName"), LVCFMT_LEFT, 180);
	m_listParentsInDock.InsertColumn(1, _T("hInstance"), LVCFMT_LEFT, 100);

	return TRUE;

}
BEGIN_MESSAGE_MAP(CDlgCreateDockPane, CDialogEx)
	ON_BN_CLICKED(IDC_CREATEWND_INDOCKPAGE, &CDlgCreateDockPane::OnBnClickedCreatewndIndockpage)
END_MESSAGE_MAP()


void CDlgCreateDockPane::OnObjectCreated(CWnd* pWnd, CString& strClass)
{

}

void CDlgCreateDockPane::UpdateClassName(CString&strDll, CString&strClass)
{
	m_strClassname = strClass;
	m_strDllname   = strDll;

	UpdateData(FALSE);
}

void CDlgCreateDockPane::OnBnClickedCreatewndIndockpage()
{
	UpdateData(TRUE);

	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//if (!m_strEditClassname.IsEmpty())
	//{
	//	if (CXmlDataProc::Instance()->IsFrameViewClass(m_strEditClassname))
	//	{
	//		if (CXmlDataProc::Instance()->IsFrameViewLoaded(m_strEditClassname))
	//		{
	//			//目前只支持单文档？！
	//		}
	//		else//load first 
	//		{
	//			pFrame->LoadDllByName(m_strDllname);
	//		}
	//	}
	//	else
	//	{
	//		pFrame->CreateFloatWnd(m_strEditClassname);
	//	}

	//}
}
