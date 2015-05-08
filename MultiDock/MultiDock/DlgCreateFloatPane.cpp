// DlgCreateFloatPane.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgCreateFloatPane.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "XmlDataProc.h"

// CDlgCreateFloatPane dialog

IMPLEMENT_DYNAMIC(CDlgCreateFloatPane, CDialogEx)

CDlgCreateFloatPane::CDlgCreateFloatPane(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateFloatPane::IDD, pParent)
	, m_strEditClassname(_T(""))
{
	CWndManager::Instance()->AddEventHandler(this);
}

CDlgCreateFloatPane::~CDlgCreateFloatPane()
{
}

void CDlgCreateFloatPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editBeCreatedClassname);
	DDX_Text(pDX, IDC_EDIT1, m_strEditClassname);
	DDX_Control(pDX, IDC_LIST_PARENT_INFLOATPANE, m_listParentInFloat);
}

BOOL CDlgCreateFloatPane::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//1.Init dll name listctrl
	DWORD dwStyle = m_listParentInFloat.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;    
	m_listParentInFloat.SetExtendedStyle(dwStyle);
	m_listParentInFloat.InsertColumn(0, _T("ClassName"), LVCFMT_LEFT, 180);
	m_listParentInFloat.InsertColumn(1, _T("hInstance"), LVCFMT_LEFT, 100);

	return TRUE;
	
}


BEGIN_MESSAGE_MAP(CDlgCreateFloatPane, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CREATE_FLOATPANE, &CDlgCreateFloatPane::OnBnClickedBtnCreateFloatpane)
END_MESSAGE_MAP()


// CDlgCreateFloatPane message handlers


void CDlgCreateFloatPane::OnBnClickedBtnCreateFloatpane()
{
	UpdateData(TRUE);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (!m_strEditClassname.IsEmpty())
	{
		if (CXmlDataProc::Instance()->IsFrameViewClass(m_strEditClassname))
		{
			if (CXmlDataProc::Instance()->IsFrameViewLoaded(m_strEditClassname))
			{
				//目前只支持单文档？！
			}
			else//load first 
			{
				pFrame->LoadDllByName(m_strDllname);
			}
		}
		else
		{
			pFrame->CreateFloatWnd(m_strEditClassname);
		}
		
	}
	
}

void CDlgCreateFloatPane::UpdateClassName(CString& strClass, CString& strDllName)
{
	m_strEditClassname = strClass;
	m_strDllname	   = strDllName;

	UpdateData(FALSE);
}

void CDlgCreateFloatPane::OnObjectCreated(CWnd* pWnd, CString& strClass)
{

}