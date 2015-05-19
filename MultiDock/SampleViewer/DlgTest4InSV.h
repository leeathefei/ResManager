#pragma once

#include "..\Common\WndManager.h"
#include "afxcmn.h"

class /*__declspec(dllexport)*/ CDlgTest4InSV : public CDialogEx,public CBaseObj
{
	DECLARE_DYNAMIC(CDlgTest4InSV)

public:
	CDlgTest4InSV(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest4InSV();

	virtual BOOL OnInitDialog();


	struct stRegister 
	{
		stRegister()
		{
			LPCTSTR lpszClassName = _T("CDlgTest4InSV");
			static bool bReged = false;
			if (!bReged)
			{
				CWndManager::Instance()->Register(lpszClassName, CDlgTest4InSV::CreateDlgObj);
				bReged = true;
			}
		}
	};

	static CWnd* CreateDlgObj();
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName,BOOL bAutoDelete=TRUE);

// Dialog Data
	enum { IDD = IDD_SAMPLEVIEW_TEST_4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listsvtest11;
	CListCtrl m_listsvtest12;
};
