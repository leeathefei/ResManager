#pragma once
#include "Resource.h"
#include "afxwin.h"
#include "..\Common\Imc.h"
#include "..\Common\Imc\ImcListener.h"
#include "..\Common\WndManager.h"

// CSamplePanelDialog dialog
#define WM_RECVALUE_FROM_VIEWER WM_USER+2015

class __declspec(dllexport) CSamplePanelDialog : public CDialog,
							public ImcListener,
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
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType);

// Dialog Data
	enum { IDD = IDD_SAMPLE_PANEDIALOG_1 };

    void RegisterImc();
    virtual LRESULT OnMessageReceived(CMessage* pMessage);
	CImcReceiver m_ImcReceiver;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnClose();
   afx_msg void OnDestroy();
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
