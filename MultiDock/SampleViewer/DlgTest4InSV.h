#pragma once


// CDlgTest4InSV dialog

class CDlgTest4InSV : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTest4InSV)

public:
	CDlgTest4InSV(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest4InSV();

// Dialog Data
	enum { IDD = IDD_SAMPLEVIEW_TEST_4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
