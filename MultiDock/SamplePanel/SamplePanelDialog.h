#pragma once
#include "Resource.h"
#include "afxwin.h"
#include "..\Common\WndManager.h"

// CSamplePanelDialog dialog
#define WM_RECVALUE_FROM_VIEWER WM_USER+2015

class __declspec(dllexport) CSamplePanelDialog : public CDialog,
							public CBaseObj
{
	DECLARE_DYNAMIC(CSamplePanelDialog)

public:
	CSamplePanelDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamplePanelDialog();

	struct stRegister
	{
		stRegister()
		{
			LPCTSTR lpszClassName = _T("CSamplePanelDialog");
			static bool bReged = false;
			if (!bReged)
			{
				CWndManager::Instance()->Register(lpszClassName, CSamplePanelDialog::CreateDlgObj);
				bReged = true;
			}
		}
	};

	static CWnd* CreateDlgObj();
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName,BOOL bAutoDelete=TRUE,CRect rect=CRect(0,0,0,0));

// Dialog Data
	enum { IDD = IDD_SAMPLE_PANEDIALOG_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnClose();
   afx_msg void OnDestroy();
   afx_msg void OnSize(UINT nType, int cx, int cy);
   virtual BOOL DestroyWindow();
protected:
   virtual void PostNcDestroy();
public:
	CEdit m_edit1Value;
	double m_dEdit1Value;
	bool m_bPaneClosed;
	double m_dbRecvValue;
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnRecvValueFromView(WPARAM W,LPARAM L);
};
