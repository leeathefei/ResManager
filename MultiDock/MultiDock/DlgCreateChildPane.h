#pragma once

#include "..\Common\WndManager.h"

class CDlgCreateChildPane : public CDialogEx, public IObjCreatedEvent
{
	DECLARE_DYNAMIC(CDlgCreateChildPane)

public:
	CDlgCreateChildPane(CWnd* pParent = NULL);  
	virtual ~CDlgCreateChildPane();

	virtual void OnObjectCreated(CWnd* pWnd, CString& strClass);
// Dialog Data
	enum { IDD = IDD_CREATE_CHILD_PANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
