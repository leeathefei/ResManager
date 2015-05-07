#pragma once
#include "Resource.h"
#include "afxwin.h"
#include "..\Common\Imc.h"
#include "..\Common\Imc\ImcListener.h"

// CSamplePanelDialog dialog

#define WM_RECV_POSITION WM_USER+2020

class CSamplePanelDialog4 : public CDialog,
							public ImcListener
{
	DECLARE_DYNAMIC(CSamplePanelDialog4)

public:
	CSamplePanelDialog4(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamplePanelDialog4();

	// Dialog Data
	enum { IDD = IDD_SAMPLE_PANEDIALOG_4 };

	bool m_bPaneClosed;
	virtual LRESULT OnMessageReceived(CMessage* pMessage);
	CImcReceiver m_receiver;

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
