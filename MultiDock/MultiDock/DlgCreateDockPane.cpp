// DlgCreateDockPane.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgCreateDockPane.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "XmlDataProc.h"


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

	InitCreatedWnd();

	return TRUE;

}
BEGIN_MESSAGE_MAP(CDlgCreateDockPane, CDialogEx)
	ON_BN_CLICKED(IDC_CREATEWND_INDOCKPAGE, &CDlgCreateDockPane::OnBnClickedCreatewndIndockpage)
END_MESSAGE_MAP()

void CDlgCreateDockPane::UpdateClassName(CString&strDll, CString&strClass)
{
	m_strClassname = strClass;
	m_strDllname   = strDll;

	UpdateData(FALSE);
}

void CDlgCreateDockPane::OnBnClickedCreatewndIndockpage()
{
	UpdateData(TRUE);

	EPANE_ALIGNMENT eType;
	switch(m_nRadioDirection)
	{
	case 0:
		eType = ALIGN_LEFT;
		break;
	case 1:
		eType = ALIGN_RIGHT;
		break;
	case 2:
		eType = ALIGN_TOP;
		break;
	case 3:
		eType = ALIGN_BOTTON;
		break;

	default:
		break;
	}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (!m_strClassname.IsEmpty())
	{
		if (CXmlDataProc::Instance()->IsFrameViewClass(m_strClassname))
		{
			if (CXmlDataProc::Instance()->IsFrameViewLoaded(m_strClassname))
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
			CWndManager::Instance()->CreateDockWnd((CWnd*)pFrame, m_strClassname, eType);
		}

	}
}
void CDlgCreateDockPane::OnObjectCreated(CWnd* pWnd, CString& strClassName)
{
	InitCreatedWnd();
}

void CDlgCreateDockPane::InitCreatedWnd()
{
	m_listParentsInDock.DeleteAllItems();

	MapWnd2Classname mapAllCreatedWnd;
	if(CWndManager::Instance()->GetCreatedWnd(mapAllCreatedWnd))
	{
		int index = 0;
		for (MapWnd2Classname::iterator it = mapAllCreatedWnd.begin();
			it != mapAllCreatedWnd.end(); ++it)
		{
			stWndInfoItem& oneItem = it->second;
			m_listParentsInDock.InsertItem(index, oneItem.strClassName);
			m_listParentsInDock.SetItemText(index, 1, oneItem.strHinstance);
			m_listParentsInDock.SetItemData(index, (DWORD_PTR)oneItem.pWnd);

			index++;
		}
	}
}