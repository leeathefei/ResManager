// DlgCreateChildPane.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgCreateChildPane.h"
#include "afxdialogex.h"


// CDlgCreateChildPane dialog

IMPLEMENT_DYNAMIC(CDlgCreateChildPane, CDialogEx)

CDlgCreateChildPane::CDlgCreateChildPane(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateChildPane::IDD, pParent)
	, m_strClassnameInChild(_T(""))
	, m_strPrtLeft(_T(""))
	, m_strPrtTop(_T(""))
	, m_strPrtRight(_T(""))
	, m_strPrtBottom(_T(""))
	, m_strChildLeft(_T(""))
	, m_strChildTop(_T(""))
	, m_strChildRight(_T(""))
	, m_strChildBottom(_T(""))
	, m_nRadioSelect(0)
	,m_nParentIndex(0)
{
	CWndManager::Instance()->AddEventHandler(this);
}

CDlgCreateChildPane::~CDlgCreateChildPane()
{
}

BOOL CDlgCreateChildPane::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//1.Init dll name listctrl
	DWORD dwStyle = m_listPrentInCreateChild.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;    
	m_listPrentInCreateChild.SetExtendedStyle(dwStyle);
	m_listPrentInCreateChild.InsertColumn(0, _T("ClassName"), LVCFMT_LEFT, 180);
	m_listPrentInCreateChild.InsertColumn(1, _T("hInstance"), LVCFMT_LEFT, 100);

	InitCreatedWnd();

	if (m_nRadioSelect == 0)
	{
		m_btnCreateChild.EnableWindow(TRUE);
		m_btnModifyChild.EnableWindow(FALSE);
	}
	else if (m_nRadioSelect == 1)
	{
		m_btnCreateChild.EnableWindow(FALSE);
		m_btnModifyChild.EnableWindow(TRUE);
	}

	return TRUE;

}

void CDlgCreateChildPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CREATECHILD_PARENTWND, m_listPrentInCreateChild);
	DDX_Text(pDX, IDC_EDIT_CREATECHILD_CLASSNAME, m_strClassnameInChild);
	DDX_Text(pDX, IDC_EDIT_PRT_LEFT, m_strPrtLeft);
	DDX_Text(pDX, IDC_EDIT_PRT_TOP, m_strPrtTop);
	DDX_Text(pDX, IDC_EDIT_PRT_RIGHT, m_strPrtRight);
	DDX_Text(pDX, IDC_EDIT_PRT_BOTTOM, m_strPrtBottom);
	DDX_Text(pDX, IDC_EDIT_CHILD_LEFT, m_strChildLeft);
	DDX_Text(pDX, IDC_EDIT_CHILD_TOP, m_strChildTop);
	DDX_Text(pDX, IDC_EDIT_CHILD_RIGHT, m_strChildRight);
	DDX_Text(pDX, IDC_EDIT_CHILD_BOTTOM, m_strChildBottom);
	DDX_Radio(pDX, IDC_RADIO3, m_nRadioSelect);
	DDX_Control(pDX, IDC_BTN_CREATE_CHILDPANE, m_btnCreateChild);
	DDX_Control(pDX, IDC_BTN_UPDATE_CHILDSIZE, m_btnModifyChild);
}


void CDlgCreateChildPane::UpdateClassName(CString&strDll, CString&strClass)
{
	m_strClassnameInChild = strClass;
	m_strDllname   = strDll;

	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CDlgCreateChildPane, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CREATE_CHILDPANE, &CDlgCreateChildPane::OnBnClickedBtnCreateChildpane)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgCreateChildPane::OnBnClickedRadiocreate)
	ON_BN_CLICKED(IDC_RADIO5, &CDlgCreateChildPane::OnBnClickedRadiomodify)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CREATECHILD_PARENTWND, OnParentSelectChanged)
END_MESSAGE_MAP()

void CDlgCreateChildPane::OnObjectCreated(CWnd* pWnd, CString& strClassName)
{
	InitCreatedWnd();
}

void CDlgCreateChildPane::InitCreatedWnd()
{
	m_listPrentInCreateChild.DeleteAllItems();

	MapWnd2Classname mapAllCreatedWnd;
	if(CWndManager::Instance()->GetCreatedWnd(mapAllCreatedWnd))
	{
		int index = 0;
		for (MapWnd2Classname::iterator it = mapAllCreatedWnd.begin();
			it != mapAllCreatedWnd.end(); ++it)
		{
			stWndInfoItem& oneItem = it->second;
			m_listPrentInCreateChild.InsertItem(index, oneItem.strClassName);
			m_listPrentInCreateChild.SetItemText(index, 1, oneItem.strHinstance);
			m_listPrentInCreateChild.SetItemData(index, (DWORD_PTR)oneItem.pWnd);

			index++;
		}
	}
}

void CDlgCreateChildPane::OnBnClickedBtnCreateChildpane()
{
	// TODO: Add your control notification handler code here
}


void CDlgCreateChildPane::OnBnClickedRadiocreate()
{
	m_nRadioSelect = 0;
	UpdateData(FALSE);

	m_btnCreateChild.EnableWindow(TRUE);
	m_btnModifyChild.EnableWindow(FALSE);
}


void CDlgCreateChildPane::OnBnClickedRadiomodify()
{
	m_nRadioSelect = 1;
	UpdateData(FALSE);

	m_btnCreateChild.EnableWindow(FALSE);
	m_btnModifyChild.EnableWindow(TRUE);

	// TODO: Add your control notification handler code here
}

void CDlgCreateChildPane::OnParentSelectChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_listPrentInCreateChild.GetFirstSelectedItemPosition();
	DWORD dwErr = GetLastError();
	if ( !pos )
		return;

	int nItem = m_listPrentInCreateChild.GetNextSelectedItem(pos);

	if (nItem != m_nParentIndex)
	{
		m_nParentIndex = nItem;

		CString strClassname = m_listPrentInCreateChild.GetItemText(nItem, 0);
		CWnd* pWnd = (CWnd*)m_listPrentInCreateChild.GetItemData(nItem);
		
		if (NULL != pWnd)
		{
			CRect rcClient;
			pWnd->GetClientRect(&rcClient);

			CString strValue;
			strValue.Format(_T("%d"),rcClient.left);
			m_strPrtLeft  = strValue;
			strValue.Empty();
			strValue.Format(_T("%d"),rcClient.right);
			m_strPrtRight = strValue;
			strValue.Empty();
			strValue.Format(_T("%d"),rcClient.top);
			m_strPrtTop   = strValue;
			strValue.Empty();
			strValue.Format(_T("%d"),rcClient.bottom);
			m_strPrtBottom = strValue;

			UpdateData(FALSE);
		}
		
		
		
	}

	*pResult = 0;
}
