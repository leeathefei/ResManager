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

	virtual void OnObjectCreated(CWnd* pWnd, CString& strClassName);


	void UpdateClassName(CString&strDll, CString&strClass);
	void InitCreatedWnd();

// Dialog Data
	enum { IDD = IDD_CREATE_DOCK_PANE };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_dockDirection;
	CString m_strClassname;
	CString m_strDllname;
	int m_nRadioDirection;
	afx_msg void OnBnClickedCreatewndIndockpage();
	CListCtrl m_listParentsInDock;
};
