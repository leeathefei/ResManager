// WndConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "WndConfigDlg.h"
#include "afxdialogex.h"
#include "DlgCreateFloatPane.h"
#include "DlgCreateDockPane.h"
#include "DlgCreateChildPane.h"
#include <string>

#include "..\Common\FileHelper.h"
#include "..\Common\XmlConfig.h"

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
	m_listctrlDllName.InsertColumn(0, _T("DllName"), LVCFMT_LEFT, 100);

	//2.Init classes names.
	dwStyle = m_listctrlClassname.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;        
	dwStyle |= LVS_EX_GRIDLINES;     
	m_listctrlClassname.SetExtendedStyle(dwStyle);
	m_listctrlClassname.InsertColumn(0, _T("ClassNames"), LVCFMT_LEFT, 100);


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
	m_listctrlDllName.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	return TRUE;
}

BOOL CWndConfigDlg::InitControls()
{

	UINT nDllCount = AppXml()->GetAttributeInt(_T("DllCount"), 0);
	if (nDllCount > 0)
	{
		//read dll names.
		for (int i=0; i<nDllCount; i++)
		{
			CString strNode;
			strNode.Format(_T("Dll_%d\\DllName"), i);
			std::wstring strDllValue = AppXml()->GetAttributeText(strNode.GetString(), _T(""));
			if (!IsDllAdded(CString(strDllValue.c_str())))
			{
				UINT nCount = m_mapDllName2Index.size();
				m_listctrlDllName.InsertItem(nCount, strDllValue.c_str());
				m_mapDllName2Index.insert(make_pair(strDllValue.c_str(), nCount));
			}
		
			//read class names.
			strNode.Format(_T("Dll_%d\\Panels\\GroupCount"), i);
			UINT nGroupCount = AppXml()->GetAttributeInt(strNode.GetString(), 0);
			if (nGroupCount>0)
			{
				for (int j=0; j<nGroupCount; j++)
				{
					CString strTypeNode;
					strTypeNode.Format(_T("Dll_%d\\Panels\\Group_%d\\PanelType"), i,j);
					std::wstring strType = AppXml()->GetAttributeText(strTypeNode, _T(""));
					CString strValue(strType.c_str());
					if (strValue.CompareNoCase(_T("FLOAT")) == 0)
					{
						ProcessFloatType(i,j,CString(strDllValue.c_str()));
					}
					else if (strValue.CompareNoCase(_T("DOCK")) == 0)
					{
						ProcessDockType(i,j,CString(strDllValue.c_str()));
					}
					else if (strValue.CompareNoCase(_T("CHILD")) == 0)
					{
						ProcessChildType(i,j,CString(strDllValue.c_str()));
					}

				}
			}
		}

	}

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

BOOL CWndConfigDlg::IsClassNameAdded(CString & strDll, CString& strClassname)
{
	MapDll2ClassNames::iterator itFind = m_mapAllDllClassNames.find(strDll);
	if (itFind != m_mapAllDllClassNames.end())
	{
		MapName2Index& mapClasses = itFind->second;
		MapName2Index::iterator it = mapClasses.find(strClassname);
		if (it != mapClasses.end())
		{
			return TRUE;
		}
	}
	
	return FALSE;
}
BOOL CWndConfigDlg::IsDllAdded(CString& strDllName)
{
	map<CString, UINT>::iterator itFind = m_mapDllName2Index.find(strDllName);
	return itFind != m_mapDllName2Index.end();
}

void CWndConfigDlg::ProcessFloatType(UINT uDllIndex, UINT uGroupIndex, CString& strDllName)
{
	CString strFloatNode;
	strFloatNode.Format(_T("Dll_%d\\Panels\\Group_%d\\PanelCount"), uDllIndex, uGroupIndex);
	int nFloatPaneCount = AppXml()->GetAttributeInt(strFloatNode.GetString(), 0);
	for (int i=0; i<nFloatPaneCount; i++)
	{
		//pane's parent's name
		CString strParentNode;
		strParentNode.Format(_T("Dll_%d\\Panels\\Group_%d\\Pane_%d\\ParentWnd\\ClassName"), uDllIndex, uGroupIndex, i);
		std::wstring strParentName = AppXml()->GetAttributeText(strParentNode.GetString(), _T(""));
		if (!IsClassNameAdded(strDllName, CString(strParentName.c_str())))
		{
			int nCount = m_listctrlClassname.GetItemCount();
			m_listctrlClassname.InsertItem(nCount, strParentName.c_str());
			AddClassName(strDllName, CString(strParentName.c_str()), nCount);
			//ma.insert(make_pair(strParentName.c_str(), nCount));
		}


		//pane's class name.
		CString strClassNode;
		strClassNode.Format(_T("Dll_%d\\Panels\\Group_%d\\Pane_%d\\ClassName"), uDllIndex, uGroupIndex, i);
		std::wstring strName = AppXml()->GetAttributeText(strClassNode.GetString(), _T(""));
		if (!IsClassNameAdded(strDllName, CString(strName.c_str())))
		{
			int nExistCount = m_listctrlClassname.GetItemCount();
			m_listctrlClassname.InsertItem(nExistCount, strName.c_str());
			AddClassName(strDllName, CString(strName.c_str()), nExistCount);
			//m_mapClassName2Index.insert(make_pair(strName.c_str(), nExistCount));	
		}
	}
}

void CWndConfigDlg::ProcessDockType(UINT uDllIndex, UINT uGroupIndex, CString& strDllName)
{
	ProcessFloatType(uDllIndex, uGroupIndex, strDllName);
}

//may be iteration for many times.
void CWndConfigDlg::ProcessChildType(UINT uDllIndex, UINT uGroupIndex, CString& strDllName)
{
	ProcessFloatType(uDllIndex, uGroupIndex, strDllName);

	//process position info.

}
void CWndConfigDlg::AddClassName(CString& strDll, CString& strClassName, UINT nIndex)
{
	MapDll2ClassNames::iterator itFind = m_mapAllDllClassNames.find(strDll);
	if (itFind != m_mapAllDllClassNames.end())
	{
		MapName2Index& mapClasses = itFind->second;
		mapClasses.insert(make_pair(strClassName, nIndex));

		return;
	}

	//dll have not added yet.
	MapName2Index mapClasses;
	mapClasses.insert(make_pair(strClassName, nIndex));
	m_mapAllDllClassNames.insert(make_pair(strDll, mapClasses));
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
	MapDll2ClassNames::iterator itFind = m_mapAllDllClassNames.find(strDllName);
	if (itFind != m_mapAllDllClassNames.end())
	{
		int index=0;
		MapName2Index& mapClasses = itFind->second;
		for (MapName2Index::iterator it = mapClasses.begin();
			it != mapClasses.end(); ++it)
		{
			m_listctrlClassname.InsertItem(index, it->first);
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
