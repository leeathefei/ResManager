#pragma once
#include "Resource.h"
#include "afxwin.h"
#include "..\Common\WndManager.h"

// CSamplePanelDialog dialog

#define WM_RECV_POSITION WM_USER+2020

class CSamplePanelDialog4 : public CDialog,
							public CBaseObj
{
	DECLARE_DYNAMIC(CSamplePanelDialog4)

public:
	CSamplePanelDialog4(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamplePanelDialog4();

	struct stRegister
	{
		stRegister()
		{
			LPCTSTR lpszClassName = _T("CSamplePanelDialog4");
			static bool bReged = false;
			if (!bReged)
			{
				CWndManager::Instance()->Register(lpszClassName, CSamplePanelDialog4::CreateDlgObj);
				bReged = true;
			}
		}
	};

	static CWnd* CreateDlgObj();
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName,BOOL bAutoDelete=TRUE,CRect rect=CRect(0,0,0,0));

	// Dialog Data
	enum { IDD = IDD_SAMPLE_PANEDIALOG_4 };

	bool m_bPaneClosed;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnRecvPostion(WPARAM W,LPARAM L);
	virtual BOOL DestroyWindow();
protected:
	virtual void PostNcDestroy();
public:
	CEdit m_editEditValue1;
	double m_dbEditvalue1;
	CEdit m_editValue2;
	CEdit m_editValue3;
	double m_dbEditValue2;
	double m_dbEditValue3;
	afx_msg void OnBnClickedButton1();
};
