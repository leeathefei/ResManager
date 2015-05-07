#pragma once


class CDlgTest3InSV : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTest3InSV)

public:
	CDlgTest3InSV(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest3InSV();

// Dialog Data
	enum { IDD = IDD_SAMPLEVIEW_TEST_3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
