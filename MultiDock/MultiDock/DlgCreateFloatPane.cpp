// DlgCreateFloatPane.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgCreateFloatPane.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "..\Common\XmlDataProc.h"

// CDlgCreateFloatPane dialog

IMPLEMENT_DYNAMIC(CDlgCreateFloatPane, CDialogEx)

CDlgCreateFloatPane::CDlgCreateFloatPane(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateFloatPane::IDD, pParent)
	, m_strEditClassname(_T(""))
	, m_strFloatWndName(_T(""))
	, m_pSelParentWnd(NULL)
	, m_nParentIndex(0)
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
	DDX_Control(pDX, IDC_COMBO_FLOAT_OWNTO, m_comboFloatProj);
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
	m_listParentInFloat.InsertColumn(1, _T("窗口类实例"), LVCFMT_LEFT, 150);
	m_listParentInFloat.InsertColumn(2, _T("所属工程"), LVCFMT_LEFT, 120);

	vector<CString> vecDlls;
	if(CXmlDataProc::Instance()->GetDllNames(vecDlls))
	{
		for (int i=0; i<vecDlls.size(); i++)
		{
			m_comboFloatProj.InsertString(i, vecDlls[i]);
		}
		m_comboFloatProj.SetCurSel(0);
	}
	

	RefreshCreatedWndTree();

	return TRUE;
	
}


BEGIN_MESSAGE_MAP(CDlgCreateFloatPane, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CREATE_FLOATPANE, &CDlgCreateFloatPane::OnBnClickedBtnCreateFloatpane)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PARENT_INFLOATPANE, OnParentSelectChanged)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PARENT_INFLOATPANE, OnMouseClicked)
END_MESSAGE_MAP()


// CDlgCreateFloatPane message handlers


void CDlgCreateFloatPane::OnBnClickedBtnCreateFloatpane()
{
	UpdateData(TRUE);

	if (m_strEditClassname.IsEmpty())
	{
		AfxMessageBox(_T("请选择要创建对象的类名！"));
		return;
	}

	CString strdll;
	m_comboFloatProj.GetWindowText(strdll);

	CString strProj = m_listParentInFloat.GetItemText(m_nParentIndex, 2);
	if (strProj.Compare(strdll) != 0)
	{
		AfxMessageBox(_T("你要创建的子窗口与父窗口不在同一模块，请重新选择！"));
		return;
	}

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
			CWndManager::Instance()->CreateFloatWnd((CWnd*)pFrame, m_strEditClassname, m_strFloatWndName, strdll);
		}
	}
	
}

void CDlgCreateFloatPane::OnMouseClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nSelectedItem = -1;
	POSITION pos = m_listParentInFloat.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}
	else
	{
		nSelectedItem = m_listParentInFloat.GetNextSelectedItem(pos);
		if(nSelectedItem != -1)
		{
			m_nParentIndex = nSelectedItem;
			m_pSelParentWnd = (CWnd*)m_listParentInFloat.GetItemData(nSelectedItem);
		}
	}

	*pResult = 0;	
}

void CDlgCreateFloatPane::OnParentSelectChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_listParentInFloat.GetFirstSelectedItemPosition();
	DWORD dwErr = GetLastError();
	if ( !pos )
		return;

	int nItem = m_listParentInFloat.GetNextSelectedItem(pos);

	if (nItem != m_nParentIndex)
	{
		m_nParentIndex = nItem;

		CString strClassname = m_listParentInFloat.GetItemText(nItem, 0);
		CWnd* pWnd = (CWnd*)m_listParentInFloat.GetItemData(nItem);
		m_pSelParentWnd = pWnd;
	
		UpdateData(FALSE);

	}

	*pResult = 0;
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
			m_listParentInFloat.SetItemText(index, 2, oneItem.strDllname);
			m_listParentInFloat.SetItemData(index, (DWORD_PTR)oneItem.pWnd);

			index++;
		}
	}

}