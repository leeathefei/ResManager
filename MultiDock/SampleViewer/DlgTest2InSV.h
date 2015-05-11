#pragma once

#include "..\Common\WndManager.h"
#include "afxcmn.h"

class CDlgTest2InSV : public CDialogEx, public CBaseObj
{
	DECLARE_DYNAMIC(CDlgTest2InSV)

public:
	CDlgTest2InSV(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest2InSV();

// Dialog Data
	enum { IDD = IDD_SAMPLEVIEW_TEST_2 };

	virtual BOOL OnInitDialog();

	struct stRegister
	{
		stRegister()
		{
			LPCTSTR lpszClassName = _T("CDlgTest2InSV");
			static bool bReged;
			if (!bReged)
			{
				CWndManager::Instance()->Register(lpszClassName, CDlgTest2InSV::CreateDlgObj);
				bReged = true;
			}
		}
	};

	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName);
	static CWnd* CreateDlgObj();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listsvtest2;
};
