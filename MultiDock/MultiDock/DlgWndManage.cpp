// DlgWndManage.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgWndManage.h"
#include "afxdialogex.h"


// CDlgWndManage dialog

IMPLEMENT_DYNAMIC(CDlgWndManage, CDialogEx)

CDlgWndManage::CDlgWndManage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgWndManage::IDD, pParent)
{
	m_pSelParentWnd = NULL;
	m_nParentIndex = 0;
	CWndManager::Instance()->AddEventHandler(this);
}

CDlgWndManage::~CDlgWndManage()
{
}

void CDlgWndManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALL_CREATED_WNDS, m_listAllVisibleWnds);
}


BEGIN_MESSAGE_MAP(CDlgWndManage, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ALL_CREATED_WNDS, OnParentSelectChanged)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ALL_CREATED_WNDS, OnMouseClicked)
END_MESSAGE_MAP()


BOOL CDlgWndManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//1.Init dll name listctrl
	DWORD dwStyle = m_listAllVisibleWnds.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       
	dwStyle |= LVS_EX_GRIDLINES;    
	m_listAllVisibleWnds.SetExtendedStyle(dwStyle);
	m_listAllVisibleWnds.InsertColumn(0, _T("窗口类名"), LVCFMT_LEFT, 130);
	m_listAllVisibleWnds.InsertColumn(1, _T("窗口类实例"), LVCFMT_LEFT, 150);
	m_listAllVisibleWnds.InsertColumn(2, _T("所属工程"), LVCFMT_LEFT, 120);
	m_listAllVisibleWnds.InsertColumn(3, _T("当前状态"),LVCFMT_LEFT, 120);


	RefreshCreatedWndTree();

	return TRUE;

}

void CDlgWndManage::OnObjectCreated()
{
	RefreshCreatedWndTree();
}
void CDlgWndManage::OnWndClosed()
{
	RefreshCreatedWndTree();
}


void CDlgWndManage::RefreshCreatedWndTree()
{
	m_listAllVisibleWnds.DeleteAllItems();

	MapCreatedWnd mapAllCreatedWnd;
	if(CWndManager::Instance()->GetCreatedWnd(mapAllCreatedWnd))
	{
		int index = 0;
		for (MapCreatedWnd::iterator it = mapAllCreatedWnd.begin();
			it != mapAllCreatedWnd.end(); ++it)
		{
			stCreateWndItem& oneItem = it->second;

			CString strState(_T("显  示"));
			if (oneItem.pWnd != NULL && NULL != oneItem.pWnd->GetSafeHwnd())
			{
				strState = oneItem.pWnd->IsWindowVisible() ? _T("显示") : _T("隐藏");
			}
			
			
			m_listAllVisibleWnds.InsertItem(index, oneItem.strClassName);
			m_listAllVisibleWnds.SetItemText(index, 1, oneItem.strHinstance);
			m_listAllVisibleWnds.SetItemText(index, 2, oneItem.strDllname);
			m_listAllVisibleWnds.SetItemText(index, 3, strState);
			m_listAllVisibleWnds.SetItemData(index, (DWORD_PTR)oneItem.pWnd);
			
			index++;
		}
	}
}


void CDlgWndManage::OnMouseClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nSelectedItem = -1;
	POSITION pos = m_listAllVisibleWnds.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}
	else
	{
		nSelectedItem = m_listAllVisibleWnds.GetNextSelectedItem(pos);
		if(nSelectedItem != -1)
		{
			m_nParentIndex = nSelectedItem;
			m_pSelParentWnd = (CWnd*)m_listAllVisibleWnds.GetItemData(nSelectedItem);
		}
	}

	*pResult = 0;	
}

void CDlgWndManage::OnParentSelectChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_listAllVisibleWnds.GetFirstSelectedItemPosition();
	DWORD dwErr = GetLastError();
	if ( !pos )
		return;

	int nItem = m_listAllVisibleWnds.GetNextSelectedItem(pos);
	if (nItem != m_nParentIndex)
	{
		m_nParentIndex = nItem;

		CString strClassname = m_listAllVisibleWnds.GetItemText(nItem, 0);
		CWnd* pWnd = (CWnd*)m_listAllVisibleWnds.GetItemData(nItem);

		if (NULL != pWnd)
		{
			m_pSelParentWnd = pWnd;
			UpdateData(FALSE);
		}
	}

	*pResult = 0;
}