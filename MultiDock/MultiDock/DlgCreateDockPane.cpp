// DlgCreateDockPane.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgCreateDockPane.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "..\Common\XmlDataProc.h"


// CDlgCreateDockPane dialog

IMPLEMENT_DYNAMIC(CDlgCreateDockPane, CDialogEx)

CDlgCreateDockPane::CDlgCreateDockPane(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateDockPane::IDD, pParent)
	, m_strClassname(_T(""))
	, m_nRadioDirection(0)
	, m_strDockWndName(_T(""))
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
	DDX_Text(pDX, IDC_EDIT1, m_strDockWndName);
	DDX_Control(pDX, IDC_COMBO_DOCK_OWNTO, m_comboDockProj);
}

BOOL CDlgCreateDockPane::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//1.Init dll name listctrl
	DWORD dwStyle = m_listParentsInDock.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;    
	m_listParentsInDock.SetExtendedStyle(dwStyle);
	m_listParentsInDock.InsertColumn(0, _T("窗口类名"), LVCFMT_LEFT, 130);
	m_listParentsInDock.InsertColumn(1, _T("窗口类实例"), LVCFMT_LEFT, 150);
	m_listParentsInDock.InsertColumn(2, _T("所属工程"), LVCFMT_LEFT, 120);

	vector<CString> vecDlls;
	if(CXmlDataProc::Instance()->GetDllNames(vecDlls))
	{
		for (int i=0; i<vecDlls.size(); i++)
		{
			m_comboDockProj.InsertString(i, vecDlls[i]);
		}
		m_comboDockProj.SetCurSel(0);
	}

	RefreshCreatedWndTree();

	return TRUE;

}
BEGIN_MESSAGE_MAP(CDlgCreateDockPane, CDialogEx)
	ON_BN_CLICKED(IDC_CREATEWND_INDOCKPAGE, &CDlgCreateDockPane::OnBnClickedCreatewndIndockpage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PARENTWNDINDOCKPANE, OnParentSelectChanged)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PARENTWNDINDOCKPANE, OnMouseClicked)
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

	if (m_strClassname.IsEmpty())
	{
		AfxMessageBox(_T("请选择要创建对象的类名！"));
		return;
	}

	CString strDll;
	m_comboDockProj.GetWindowText(strDll);

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
	case 4:
		eType = ALIGN_LEFT_GROUP;
		break;
	case 5:
		eType = ALIGN_RIGHT_GROUP;
		break;
	case 6:
		eType = ALIGN_TOP_GROUP;
		break;
	case 7:
		eType = ALIGN_BOTTOM_GROUP;
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
			CWndManager::Instance()->CreateDockWnd((CWnd*)pFrame/*m_pSelParentWnd*/, m_strClassname, eType, m_strDockWndName,strDll);
		}

	}
}


void CDlgCreateDockPane::OnMouseClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nSelectedItem = -1;
	POSITION pos = m_listParentsInDock.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}
	else
	{
		nSelectedItem = m_listParentsInDock.GetNextSelectedItem(pos);
		if(nSelectedItem != -1)
		{
			m_nParentIndex = nSelectedItem;
			m_pSelParentWnd = (CWnd*)m_listParentsInDock.GetItemData(nSelectedItem);
		}
	}

	*pResult = 0;	
}

void CDlgCreateDockPane::OnParentSelectChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_listParentsInDock.GetFirstSelectedItemPosition();
	DWORD dwErr = GetLastError();
	if ( !pos )
		return;

	int nItem = m_listParentsInDock.GetNextSelectedItem(pos);

	if (nItem != m_nParentIndex)
	{
		m_nParentIndex = nItem;

		CString strClassname = m_listParentsInDock.GetItemText(nItem, 0);
		CWnd* pWnd = (CWnd*)m_listParentsInDock.GetItemData(nItem);
		m_pSelParentWnd = pWnd;

		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CDlgCreateDockPane::OnObjectCreated()
{
	RefreshCreatedWndTree();
}
void CDlgCreateDockPane::OnWndClosed()
{
	RefreshCreatedWndTree();
}

void CDlgCreateDockPane::RefreshCreatedWndTree()
{
	m_listParentsInDock.DeleteAllItems();

	MapCreatedWnd mapAllCreatedWnd;
	if(CWndManager::Instance()->GetCreatedWnd(mapAllCreatedWnd))
	{
		int index = 0;
		for (MapCreatedWnd::iterator it = mapAllCreatedWnd.begin();
			it != mapAllCreatedWnd.end(); ++it)
		{
			stCreateWndItem& oneItem = it->second;
			m_listParentsInDock.InsertItem(index, oneItem.strClassName);
			m_listParentsInDock.SetItemText(index, 1, oneItem.strHinstance);
			m_listParentsInDock.SetItemText(index, 2, oneItem.strDllname);
			m_listParentsInDock.SetItemData(index, (DWORD_PTR)oneItem.pWnd);

			index++;
		}
	}
}