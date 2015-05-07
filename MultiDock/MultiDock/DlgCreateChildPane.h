#pragma once


// CDlgCreateChildPane dialog

class CDlgCreateChildPane : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCreateChildPane)

public:
	CDlgCreateChildPane(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCreateChildPane();

// Dialog Data
	enum { IDD = IDD_CREATE_CHILD_PANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
