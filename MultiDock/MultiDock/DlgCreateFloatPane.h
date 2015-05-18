#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "..\Common\WndManager.h"


class CMainFrame;
// CDlgCreateFloatPane dialog

class CDlgCreateFloatPane : public CDialogEx, public IObjCreatedEvent
{
	DECLARE_DYNAMIC(CDlgCreateFloatPane)

public:
	CDlgCreateFloatPane(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCreateFloatPane();

	void UpdateClassName(CString& strClass, CString& strDllName);
	void RefreshCreatedWndTree();

// Dialog Data
	enum { IDD = IDD_CREATE_FLOAT_PANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void OnObjectCreated();
	virtual void OnWndClosed();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCreateFloatpane();
	CEdit m_editBeCreatedClassname;
	CString m_strEditClassname;
	CString m_strDllname;
	CListCtrl m_listParentInFloat;
	CString m_strFloatWndName;
	CComboBox m_comboFloatProj;
};
