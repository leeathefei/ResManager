#pragma once
#include "Resource.h"
#include "..\Common\Imc.h"
#include "..\Common\Imc\ImcListener.h"
#include "afxwin.h"

// CSamplePanelDialog dialog

#define WM_RECVCOMBOSEL_FROM_PANE1 WM_USER+2019

class CSamplePanelDialog2 : public CDialog,
							public ImcListener
{
	DECLARE_DYNAMIC(CSamplePanelDialog2)

public:
	CSamplePanelDialog2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamplePanelDialog2();
	virtual BOOL OnInitDialog();

	// Dialog Data
	enum { IDD = IDD_SAMPLE_PANEDIALOG_2 };

	bool m_bPaneClosed;

	CImcReceiver m_receiver;
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT OnMessageReceived(CMessage* pMessage);

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
