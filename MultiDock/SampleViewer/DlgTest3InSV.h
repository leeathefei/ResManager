#pragma once

#include "..\Common\WndManager.h"

class CDlgTest3InSV : public CDialogEx,public CBaseObj
{
	DECLARE_DYNAMIC(CDlgTest3InSV)

public:
	CDlgTest3InSV(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest3InSV();

// Dialog Data
	enum { IDD = IDD_SAMPLEVIEW_TEST_3 };

	virtual BOOL OnInitDialog();

	struct stRegister
	{
		stRegister()
		{
			LPCTSTR lpszClassName = _T("CDlgTest3InSV");
			static bool bReged = false;
			if (!bReged)
			{
				CWndManager::Instance()->Register(lpszClassName, CDlgTest3InSV::CreateDlgObj);
				bReged = true;
			}
		}
	};

	static CWnd* CreateDlgObj();
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName,BOOL bAutoDelete=TRUE,CRect rect=CRect(0,0,0,0));

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};
