#pragma once
#include "Resource.h"
#include "..\Common\Imc.h"
#include "..\Common\Imc\ImcListener.h"
#include "..\Common\WndManager.h"
// CSamplePanelDialog dialog


#define WM_RECVALUE_VIEWER1_PANE2 WM_USER+2016

class CSamplePanelDialog3 : public CDialog,public ImcListener, public CBaseObj
{
	DECLARE_DYNAMIC(CSamplePanelDialog3)

public:
	CSamplePanelDialog3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamplePanelDialog3();

	struct stRegister
	{
		stRegister()
		{
			LPCTSTR lpszClassName = _T("CSamplePanelDialog3");
			static bool bReged = false;
			if (!bReged)
			{
				CWndManager::Instance()->Register(lpszClassName, CSamplePanelDialog3::CreateDlgObj);
				bReged = true;
			}
		}
	};

	static CWnd* CreateDlgObj();
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName);

	// Dialog Data
	enum { IDD = IDD_SAMPLE_PANEDIALOG_3 };

	bool m_bPaneClosed;
	void RegisterImc();
	virtual LRESULT OnMessageReceived(CMessage* pMessage);
	CImcReceiver m_ImcReceiver;
	int m_nRadioValue;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnRecvValueFromView(WPARAM W,LPARAM L);
	virtual BOOL DestroyWindow();
protected:
	virtual void PostNcDestroy();
public:
	BOOL m_nRadio;
	afx_msg void OnBnClickedButton1();
};
