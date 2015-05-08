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

	virtual void OnObjectCreated(CWnd* pWnd, CString& strClassName);

	void UpdateClassName(CString&strDll, CString&strClass);
	void InitCreatedWnd();

// Dialog Data
	enum { IDD = IDD_CREATE_CHILD_PANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listPrentInCreateChild;
	CString m_strClassnameInChild;
	CString m_strDllname;
	afx_msg void OnBnClickedBtnCreateChildpane();
	CString m_strPrtLeft;
	CString m_strPrtTop;
	CString m_strPrtRight;
	CString m_strPrtBottom;
	CString m_strChildLeft;
	CString m_strChildTop;
	CString m_strChildRight;
	CString m_strChildBottom;
	int m_nRadioSelect;
	CButton m_btnCreateChild;
	CButton m_btnModifyChild;
	afx_msg void OnBnClickedRadiocreate();
	afx_msg void OnBnClickedRadiomodify();
};
