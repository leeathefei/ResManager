#pragma once


// CDlgTest2InSV dialog

class CDlgTest2InSV : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTest2InSV)

public:
	CDlgTest2InSV(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest2InSV();

// Dialog Data
	enum { IDD = IDD_SAMPLEVIEW_TEST_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
