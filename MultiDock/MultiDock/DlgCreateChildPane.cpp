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
	, m_nRadioSelect(0)
	,m_nParentIndex(0)
	, m_uPrtLeft(0)
	, m_uPrtRight(0)
	, m_uPrtTop(0)
	, m_uPrtBottom(0)
	, m_uChildLeft(0)
	, m_uChildRight(0)
	, m_uChildTop(0)
	, m_uChildBottom(0)
	, m_pSelParentWnd(NULL)
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
	m_listPrentInCreateChild.InsertColumn(0, _T("窗口类名"), LVCFMT_LEFT, 180);
	m_listPrentInCreateChild.InsertColumn(1, _T("窗口类实例"), LVCFMT_LEFT, 100);

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
	DDX_Text(pDX, IDC_EDIT_PRT_LEFT, m_uPrtLeft);
	DDX_Text(pDX, IDC_EDIT_PRT_TOP, m_uPrtTop);
	DDX_Text(pDX, IDC_EDIT_PRT_RIGHT, m_uPrtRight);
	DDX_Text(pDX, IDC_EDIT_PRT_BOTTOM, m_uPrtBottom);
	DDX_Text(pDX, IDC_EDIT_CHILD_LEFT, m_uChildLeft);
	DDX_Text(pDX, IDC_EDIT_CHILD_TOP, m_uChildTop);
	DDX_Text(pDX, IDC_EDIT_CHILD_RIGHT, m_uChildRight);
	DDX_Text(pDX, IDC_EDIT_CHILD_BOTTOM, m_uChildBottom);
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
	ON_NOTIFY(NM_CLICK, IDC_LIST_CREATECHILD_PARENTWND, OnMouseClicked)
	ON_BN_CLICKED(IDC_BTN_UPDATE_CHILDSIZE, &CDlgCreateChildPane::OnBnClickedBtnUpdateChildsize)
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

void CDlgCreateChildPane::OnMouseClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nSelectedItem = -1;
	POSITION pos = m_listPrentInCreateChild.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}
	else
	{
		nSelectedItem = m_listPrentInCreateChild.GetNextSelectedItem(pos);
		if(nSelectedItem != -1)
		{
			m_nParentIndex = nSelectedItem;
			m_pSelParentWnd = (CWnd*)m_listPrentInCreateChild.GetItemData(nSelectedItem);
		}
	}

	*pResult = 0;	
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
			m_pSelParentWnd = pWnd;

			CRect rcClient;
			pWnd->GetClientRect(&rcClient);
			m_uPrtLeft  = rcClient.left;
			m_uPrtRight = rcClient.right;
			m_uPrtTop	= rcClient.top;
			m_uPrtBottom = rcClient.bottom;

			UpdateData(FALSE);


		}
	}

	*pResult = 0;
}

void CDlgCreateChildPane::OnBnClickedBtnCreateChildpane()
{
	UpdateData(TRUE);

	CRect rcChild;
	rcChild.left = m_uChildLeft;
	rcChild.right = m_uChildRight;
	rcChild.top  = m_uChildTop;
	rcChild.bottom = m_uChildBottom;

	//1.create child window.
	if (NULL != m_pSelParentWnd && NULL != m_pSelParentWnd->GetSafeHwnd())
	{
		CWndManager::Instance()->CreateChildWnd(m_pSelParentWnd, m_strClassnameInChild, rcChild);
		m_pSelParentWnd->SendMessage(WM_SIZE);
	}
	else
	{
		AfxMessageBox(_T("请先选择父窗口！"));
	}
}


void CDlgCreateChildPane::OnBnClickedBtnUpdateChildsize()
{
	UpdateData(TRUE);

	CRect rcNew;
	rcNew.left = m_uChildLeft;
	rcNew.right = m_uChildRight;
	rcNew.top  = m_uChildTop;
	rcNew.bottom = m_uChildBottom;

	if (NULL != m_pSelParentWnd && NULL != m_pSelParentWnd->GetSafeHwnd())
	{
		CWnd* pParent = CWndManager::Instance()->UpdateChildWndSize(m_pSelParentWnd, rcNew);
		if(pParent != NULL)
		{
			pParent->SendMessage(WM_SIZE);
		}
		else
		{
			AfxMessageBox(_T("请选择子窗口，父窗口不可以修改大小！"));
		}
	}
}
