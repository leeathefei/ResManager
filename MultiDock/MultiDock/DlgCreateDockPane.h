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

	virtual void OnObjectCreated(CWnd* pWnd, CString& strClass);

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
