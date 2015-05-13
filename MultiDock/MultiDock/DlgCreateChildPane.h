#pragma once

#include "..\Common\WndManager.h"
#include "afxcmn.h"
#include "afxwin.h"

class CDlgCreateChildPane : public CDialogEx, public IObjCreatedEvent
{
	DECLARE_DYNAMIC(CDlgCreateChildPane)

public:
	CDlgCreateChildPane(CWnd* pParent = NULL);  
	virtual ~CDlgCreateChildPane();

	virtual void OnObjectCreated();
	virtual void OnWndClosed();

	void UpdateClassName(CString&strDll, CString&strClass);
	void RefreshCreatedWndTree();

// Dialog Data
	enum { IDD = IDD_CREATE_CHILD_PANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedRadiocreate();
	afx_msg void OnBnClickedRadiomodify();
	afx_msg void OnBnClickedBtnCreateChildpane();
	afx_msg void OnBnClickedBtnUpdateChildsize();
	afx_msg void OnBnClickedRadioWithCaption();
	afx_msg void OnBnClickedRadioWithNoCaption();
	afx_msg void OnParentSelectChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseClicked(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	CWnd* m_pSelParentWnd;
	int m_nParentIndex;
	CListCtrl m_listPrentInCreateChild;
	CString m_strClassnameInChild;
	CString m_strDllname;
	
	UINT m_uPrtLeft;
	UINT m_uPrtRight;
	UINT m_uPrtTop;
	UINT m_uPrtBottom;
	UINT m_uChildLeft;
	UINT m_uChildRight;
	UINT m_uChildTop;
	UINT m_uChildBottom;

	int m_nRadioSelect;
	CButton m_btnCreateChild;
	CButton m_btnModifyChild;

	

	CEdit m_editChildWndName;
	CString m_strChildWndName;
	int m_nRadioWithNoCaption;

};
