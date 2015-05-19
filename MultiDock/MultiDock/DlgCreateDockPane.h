#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "..\Common\WndManager.h"

class CDlgCreateDockPane : public CDialogEx, public IObjCreatedEvent
{
	DECLARE_DYNAMIC(CDlgCreateDockPane)

public:
	CDlgCreateDockPane(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCreateDockPane();

	virtual void OnObjectCreated();
	virtual void OnWndClosed();


	void UpdateClassName(CString&strDll, CString&strClass);
	void RefreshCreatedWndTree();

// Dialog Data
	enum { IDD = IDD_CREATE_DOCK_PANE };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnBnClickedCreatewndIndockpage();
	afx_msg void OnParentSelectChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseClicked(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	CButton m_dockDirection;
	CString m_strClassname;
	CString m_strDllname;
	int m_nRadioDirection;
	
	CListCtrl m_listParentsInDock;
	CString m_strDockWndName;
	CComboBox m_comboDockProj;

	CWnd* m_pSelParentWnd;
	int m_nParentIndex;
	int m_nDockAutoDel;
	afx_msg void OnBnClickedRadioDockBeautodel();
	afx_msg void OnBnClickedRadioDockUnautodel();
};
