#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgCreateDockPane dialog

class CDlgCreateDockPane : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCreateDockPane)

public:
	CDlgCreateDockPane(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCreateDockPane();

// Dialog Data
	enum { IDD = IDD_CREATE_DOCK_PANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_dockDirection;
	CString m_strClassname;
	CTreeCtrl m_treeInDock;
};
