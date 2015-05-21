#pragma once

#include "..\Common\WndManager.h"
#include "afxcmn.h"
// CDlgWndManage dialog

class CDlgWndManage : public CDialogEx,public IObjCreatedEvent
{
	DECLARE_DYNAMIC(CDlgWndManage)

public:
	CDlgWndManage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgWndManage();

	virtual BOOL OnInitDialog();

	virtual void OnObjectCreated();
	virtual void OnWndClosed();

	void RefreshCreatedWndTree();

// Dialog Data
	enum { IDD = IDD_WND_MANAGEMENT };

protected:
	afx_msg void OnParentSelectChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseClicked(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBnClickedBtnShowhidewnd();
	afx_msg void OnBnClickedBtnHideSelwnd();
	afx_msg void OnBnClickedBtnRemovewnd();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listAllVisibleWnds;
	CWnd* m_pSelParentWnd;
	int m_nParentIndex;

};
