// DlgCreateFloatPane.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgCreateFloatPane.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "..\Common\WndManager.h"
#include "XmlDataProc.h"

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
			pFrame->CreateDockWnd(m_strDllname.GetString(), m_strEditClassname.GetString(), ALIGN_VERTICAL);
		}
		
	}
	
}

void CDlgCreateFloatPane::UpdateClassName(CString& strClass, CString& strDllName)
{
	m_strEditClassname = strClass;
	m_strDllname	   = strDllName;

	UpdateData(FALSE);
}