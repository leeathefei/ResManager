#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <map>

using namespace std;

class CDlgCreateFloatPane;
class CDlgCreateDockPane;
class CDlgCreateChildPane;

class CWndConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWndConfigDlg)

public:
	CWndConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWndConfigDlg();
	virtual BOOL OnInitDialog();

	BOOL InitControls();
// Dialog Data
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTabFilter(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listctrlDllName;
	CListCtrl m_listctrlClassname;
	CTabCtrl m_tabctrlCreateWnd;

	CDlgCreateFloatPane* m_pCreateFloat;
	CDlgCreateDockPane*  m_pCreateDock;
	CDlgCreateChildPane* m_pCreateChild;

protected:
	BOOL IsDllAdded(CString& strDllName);
	BOOL IsClassNameAdded(CString& strClassname);
	void ProcessFloatType(UINT uDllIndex, UINT uGroupIndex);
	void ProcessDockType(UINT uDllIndex, UINT uGroupIndex);
	void ProcessChildType(UINT uDllIndex, UINT uGroupIndex);

	map<CString, UINT> m_mapDllName2Index;
	map<CString, UINT> m_mapClassName2Index;

};
