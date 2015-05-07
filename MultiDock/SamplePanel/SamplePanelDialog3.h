#pragma once
#include "Resource.h"
#include "..\Common\Imc.h"
#include "..\Common\Imc\ImcListener.h"
// CSamplePanelDialog dialog


#define WM_RECVALUE_VIEWER1_PANE2 WM_USER+2016

class CSamplePanelDialog3 : public CDialog,public ImcListener
{
	DECLARE_DYNAMIC(CSamplePanelDialog3)

public:
	CSamplePanelDialog3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamplePanelDialog3();

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
