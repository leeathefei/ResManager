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
	, m_strFloatWndName(_T(""))
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
	DDX_Text(pDX, IDC_EDIT_FLAOT_WND_NAME, m_strFloatWndName);
}

BOOL CDlgCreateFloatPane::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//1.Init dll name listctrl
	DWORD dwStyle = m_listParentInFloat.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;    
	m_listParentInFloat.SetExtendedStyle(dwStyle);
	m_listParentInFloat.InsertColumn(0, _T("窗口类名"), LVCFMT_LEFT, 130);
	m_listParentInFloat.InsertColumn(1, _T("窗口类实例"), LVCFMT_LEFT, 180);

	RefreshCreatedWndTree();

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
			CWndManager::Instance()->CreateFloatWnd((CWnd*)pFrame, m_strEditClassname, m_strFloatWndName);
		}
	}
	
}

void CDlgCreateFloatPane::UpdateClassName(CString& strClass, CString& strDllName)
{
	m_strEditClassname = strClass;
	m_strDllname	   = strDllName;

	UpdateData(FALSE);
}
void CDlgCreateFloatPane::OnObjectCreated()
{
	RefreshCreatedWndTree();
}
void CDlgCreateFloatPane::OnWndClosed()
{
	RefreshCreatedWndTree();
}

void CDlgCreateFloatPane::RefreshCreatedWndTree()
{
	m_listParentInFloat.DeleteAllItems();

	MapCreatedWnd mapAllCreatedWnd;
	if(CWndManager::Instance()->GetCreatedWnd(mapAllCreatedWnd))
	{
		int index = 0;
		for (MapCreatedWnd::iterator it = mapAllCreatedWnd.begin();
			it != mapAllCreatedWnd.end(); ++it)
		{
			stCreateWndItem& oneItem = it->second;
			m_listParentInFloat.InsertItem(index, oneItem.strClassName);
			m_listParentInFloat.SetItemText(index, 1, oneItem.strHinstance);
			m_listParentInFloat.SetItemData(index, (DWORD_PTR)oneItem.pWnd);

			index++;
		}
	}

}