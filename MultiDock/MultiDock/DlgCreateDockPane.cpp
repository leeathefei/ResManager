// DlgCreateDockPane.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgCreateDockPane.h"
#include "afxdialogex.h"


// CDlgCreateDockPane dialog

IMPLEMENT_DYNAMIC(CDlgCreateDockPane, CDialogEx)

CDlgCreateDockPane::CDlgCreateDockPane(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateDockPane::IDD, pParent)
	, m_strClassname(_T(""))
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
	DDX_Control(pDX, IDC_TREE_WNDOBJS_INDOCK, m_treeInDock);
}


BEGIN_MESSAGE_MAP(CDlgCreateDockPane, CDialogEx)
END_MESSAGE_MAP()


void CDlgCreateDockPane::OnObjectCreated(CWnd* pWnd, CString& strClass)
{

}