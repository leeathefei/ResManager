#pragma once


// CDlgTest1InSV dialog

class CDlgTest1InSV : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTest1InSV)

public:
	CDlgTest1InSV(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest1InSV();

// Dialog Data
	enum { IDD = IDD_SAMPLEVIEW_TEST_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
