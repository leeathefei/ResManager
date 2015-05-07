#pragma once

#include "..\Common\WndManager.h"

class __declspec(dllexport) CDlgTest4InSV : public CDialogEx,public CBaseObj
{
	DECLARE_DYNAMIC(CDlgTest4InSV)

public:
	CDlgTest4InSV(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest4InSV();


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
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType);

// Dialog Data
	enum { IDD = IDD_SAMPLEVIEW_TEST_4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
