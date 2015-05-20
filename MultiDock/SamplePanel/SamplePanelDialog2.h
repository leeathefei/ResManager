#pragma once
#include "Resource.h"
#include "..\Common\WndManager.h"
#include "afxwin.h"

// CSamplePanelDialog dialog

#define WM_RECVCOMBOSEL_FROM_PANE1 WM_USER+2019

class CSamplePanelDialog2 : public CDialog,
							public CBaseObj
{
	DECLARE_DYNAMIC(CSamplePanelDialog2)

public:
	CSamplePanelDialog2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamplePanelDialog2();
	virtual BOOL OnInitDialog();


	struct stRegister
	{
		stRegister()
		{
			LPCTSTR lpszClassName = _T("CSamplePanelDialog2");
			static bool bReged = false;
			if (!bReged)
			{
				CWndManager::Instance()->Register(lpszClassName, CSamplePanelDialog2::CreateDlgObj);
				bReged = true;
			}
		}
	};

	static CWnd* CreateDlgObj();
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName,BOOL bAutoDelete=TRUE,CRect rect=CRect(0,0,0,0));

	// Dialog Data
	enum { IDD = IDD_SAMPLE_PANEDIALOG_2 };

	bool m_bPaneClosed;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnRecvSelection(WPARAM w, LPARAM l);
	
	virtual BOOL DestroyWindow();
protected:
	virtual void PostNcDestroy();
public:
	CComboBox m_comboControl;
	UINT m_uRecvSelection;
};
