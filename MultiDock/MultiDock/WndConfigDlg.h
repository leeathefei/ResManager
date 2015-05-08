#pragma once
#include "afxwin.h"
#include "afxcmn.h"


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
	afx_msg void OnDllItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseClicked(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClassnameItemChanged(NMHDR* pNMHDR, LRESULT* pResult);

	void UpdateClassnames(int nSelected);
	DECLARE_MESSAGE_MAP()
public:
	int m_nDllHitIndex;
	int m_nClassnameHitindex;
	CListCtrl m_listctrlDllName;
	CListCtrl m_listctrlClassname;
	CTabCtrl m_tabctrlCreateWnd;

	CDlgCreateFloatPane* m_pCreateFloat;
	CDlgCreateDockPane*  m_pCreateDock;
	CDlgCreateChildPane* m_pCreateChild;



};
