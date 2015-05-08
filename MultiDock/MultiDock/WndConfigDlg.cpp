// WndConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "WndConfigDlg.h"
#include "afxdialogex.h"
#include "DlgCreateFloatPane.h"
#include "DlgCreateDockPane.h"
#include "DlgCreateChildPane.h"
#include "XmlDataProc.h"

IMPLEMENT_DYNAMIC(CWndConfigDlg, CDialogEx)

CWndConfigDlg::CWndConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWndConfigDlg::IDD, pParent)
{
	m_pCreateChild = NULL;
	m_pCreateDock  = NULL;
	m_pCreateFloat = NULL;
	m_nDllHitIndex = -1;
	m_nClassnameHitindex = -1;
}

CWndConfigDlg::~CWndConfigDlg()
{
	if (NULL != m_pCreateFloat)
	{
		m_pCreateFloat->DestroyWindow();
		delete m_pCreateFloat;
	}
	if (NULL != m_pCreateDock)
	{
		m_pCreateDock->DestroyWindow();
		delete m_pCreateDock;
	}
	if (NULL != m_pCreateChild)
	{
		m_pCreateChild->DestroyWindow();
		delete m_pCreateChild;
	}
}

void CWndConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listctrlDllName);
	DDX_Control(pDX, IDC_LIST1, m_listctrlClassname);
	DDX_Control(pDX, IDC_TAB1, m_tabctrlCreateWnd);
}


BEGIN_MESSAGE_MAP(CWndConfigDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CWndConfigDlg::OnTcnSelchangeTabFilter)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, OnDllItemChanged)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, OnMouseClicked)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnClassnameItemChanged)
END_MESSAGE_MAP()


BOOL CWndConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//1.Init dll name listctrl
	DWORD dwStyle = m_listctrlDllName.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;    
	m_listctrlDllName.SetExtendedStyle(dwStyle);
	m_listctrlDllName.InsertColumn(0, _T("DllName"), LVCFMT_LEFT, 165);

	//2.Init classes names.
	dwStyle = m_listctrlClassname.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;        
	dwStyle |= LVS_EX_GRIDLINES;     
	m_listctrlClassname.SetExtendedStyle(dwStyle);
	m_listctrlClassname.InsertColumn(0, _T("ClassNames"), LVCFMT_LEFT, 176);


	//3.Init tab control
	TCITEM tie;
	tie.mask = TCIF_TEXT;
	tie.pszText = _T("浮动窗口");
	TabCtrl_InsertItem(m_tabctrlCreateWnd.GetSafeHwnd(), 0, &tie);

	tie.pszText = _T("Dock窗口");
	TabCtrl_InsertItem(m_tabctrlCreateWnd.GetSafeHwnd(), 1, &tie);

	tie.pszText = _T("子窗口");
	TabCtrl_InsertItem(m_tabctrlCreateWnd.GetSafeHwnd(), 2, &tie);

	m_pCreateFloat = new CDlgCreateFloatPane;
	if (NULL != m_pCreateFloat)
	{
		m_pCreateFloat->Create(CDlgCreateFloatPane::IDD, &m_tabctrlCreateWnd);
	}

	m_pCreateDock = new CDlgCreateDockPane;
	if (NULL != m_pCreateDock)
	{
		m_pCreateDock->Create(CDlgCreateDockPane::IDD, &m_tabctrlCreateWnd);
	}

	m_pCreateChild = new CDlgCreateChildPane;
	if (NULL != m_pCreateChild)
	{
		m_pCreateChild->Create(CDlgCreateChildPane::IDD, &m_tabctrlCreateWnd);
	}

	CRect rc;
	m_tabctrlCreateWnd.GetClientRect(&rc);
	rc.top += 23;
	rc.bottom -=4;
	rc.left += 2;
	rc.right -= 3;

	m_pCreateFloat->MoveWindow(&rc);
	m_pCreateDock->MoveWindow(&rc);
	m_pCreateChild->MoveWindow(&rc);

	m_pCreateFloat->ShowWindow(SW_SHOW);
	m_pCreateDock->ShowWindow(SW_HIDE);
	m_pCreateChild->ShowWindow(SW_HIDE);


	//4.Init values for controls
	InitControls();

	return TRUE;
}

BOOL CWndConfigDlg::InitControls()
{
	vector<CString> vecDlls;
	if(CXmlDataProc::Instance()->GetDllNames(vecDlls))
	{
		for (int i=0; i<vecDlls.size();i++)
		{
			CString& strDll = vecDlls[i];

			//Insert to dll ctrl
			m_listctrlDllName.InsertItem(i, strDll);

			vector<CString> vecClasses;
			if (CXmlDataProc::Instance()->GetClassNames(strDll, vecClasses))
			{
				for (int j=0; j<vecClasses.size(); j++)
				{
					m_listctrlClassname.InsertItem(j, vecClasses[j]);
				}
			}
		}
	}


	m_listctrlDllName.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	return TRUE;
}

void CWndConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (   NULL != m_tabctrlCreateWnd.GetSafeHwnd()
		&& NULL != m_pCreateFloat->GetSafeHwnd()
		&& NULL != m_pCreateDock->GetSafeHwnd()
		&& NULL != m_pCreateChild->GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(&rc);
		m_tabctrlCreateWnd.MoveWindow(&rc);

		CRect rcDlg = rc;
		rcDlg.left   += 2;
		rcDlg.right  -= 2;
		rcDlg.top	 += 20;
		rcDlg.bottom -= 4;
		m_pCreateFloat->MoveWindow(&rcDlg);
		m_pCreateDock->MoveWindow(&rcDlg);
		m_pCreateChild->MoveWindow(&rcDlg);

	}
}

void CWndConfigDlg::OnTcnSelchangeTabFilter(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_tabctrlCreateWnd.GetCurSel()==0)
	{
		m_pCreateFloat->ShowWindow(SW_SHOW);
		m_pCreateDock->ShowWindow(SW_HIDE);
		m_pCreateChild->ShowWindow(SW_HIDE);
	}
	else if(m_tabctrlCreateWnd.GetCurSel() == 1)
	{
		m_pCreateFloat->ShowWindow(SW_HIDE);
		m_pCreateDock->ShowWindow(SW_SHOW);
		m_pCreateChild->ShowWindow(SW_HIDE);
	} 
	else if(m_tabctrlCreateWnd.GetCurSel() == 2)
	{
		m_pCreateFloat->ShowWindow(SW_HIDE);
		m_pCreateDock->ShowWindow(SW_HIDE);
		m_pCreateChild->ShowWindow(SW_SHOW);
	}
	

	*pResult = 0;
}


void CWndConfigDlg::OnDllItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_listctrlDllName.GetFirstSelectedItemPosition();
	DWORD dwErr = GetLastError();
	if ( !pos )
		return;

	int nItem = m_listctrlDllName.GetNextSelectedItem(pos);

	if (nItem != m_nDllHitIndex)
	{
		UpdateClassnames(nItem);
	}

	*pResult = 0;
}

void CWndConfigDlg::OnMouseClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nSelectedItem = -1;
	POSITION pos = m_listctrlDllName.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}
	else
	{
		nSelectedItem = m_listctrlDllName.GetNextSelectedItem(pos);
		if(nSelectedItem != -1)
		{
			UpdateClassnames(nSelectedItem);
		}
	}

	*pResult = 0;	
}

void CWndConfigDlg::UpdateClassnames(int nSelected)
{
	if (nSelected == m_nDllHitIndex)
	{
		return;
	}

	m_nDllHitIndex = nSelected;
	m_listctrlClassname.DeleteAllItems();

	CString strDllName;
	strDllName = m_listctrlDllName.GetItemText(nSelected,0);

	vector<CString> vecClass;
	if (CXmlDataProc::Instance()->GetClassNames(strDllName, vecClass))
	{
		for(int i=0; i<vecClass.size(); i++)
		{
			m_listctrlClassname.InsertItem(i, vecClass[i]);
		}
	}
}


void CWndConfigDlg::OnClassnameItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_listctrlClassname.GetFirstSelectedItemPosition();
	DWORD dwErr = GetLastError();
	if ( !pos )
		return;

	int nItem = m_listctrlClassname.GetNextSelectedItem(pos);

	if (nItem != m_nClassnameHitindex)
	{
		m_nClassnameHitindex = nItem;

		CString strClassname = m_listctrlClassname.GetItemText(nItem, 0);
		if (NULL != m_pCreateFloat)
		{
			m_pCreateFloat->UpdateClassName(strClassname);
		}
	}

	*pResult = 0;
}
