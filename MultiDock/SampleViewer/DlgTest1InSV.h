#pragma once


#include "..\Common\WndManager.h"
#include "afxcmn.h"

class CDlgTest1InSV : public CDialogEx, public CBaseObj
{
	DECLARE_DYNAMIC(CDlgTest1InSV)

public:
	CDlgTest1InSV(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest1InSV();

// Dialog Data
	enum { IDD = IDD_SAMPLEVIEW_TEST_1 };
	virtual BOOL OnInitDialog();

	struct stRegister
	{
		stRegister()
		{
			LPCTSTR lpszClassName = _T("CDlgTest1InSV");
			static bool bReged = false;
			if (!bReged)
			{
				CWndManager::Instance()->Register(lpszClassName, CDlgTest1InSV::CreateDlgObj);
				bReged = true;
			}
		}
	};

	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName,BOOL bAutoDelete = TRUE,CRect rect=CRect(0,0,0,0));
	static  CWnd* CreateDlgObj();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnClose();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listsvtest1;
};
