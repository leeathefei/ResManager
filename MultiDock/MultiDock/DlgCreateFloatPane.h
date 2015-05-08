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

	virtual void OnObjectCreated(CWnd* pWnd, CString& strClass);

	void UpdateClassName(CString& strClass, CString& strDllName);

// Dialog Data
	enum { IDD = IDD_CREATE_FLOAT_PANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCreateFloatpane();
	CEdit m_editBeCreatedClassname;
	CString m_strEditClassname;
	CString m_strDllname;
	CTreeCtrl m_treePrarents;
};
