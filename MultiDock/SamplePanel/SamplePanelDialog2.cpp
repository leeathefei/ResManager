// SamplePanelDialog2.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePanel.h"
#include "SamplePanelDialog2.h"


// CSamplePanelDialog2 dialog

IMPLEMENT_DYNAMIC(CSamplePanelDialog2, CDialog)

	CSamplePanelDialog2::CSamplePanelDialog2(CWnd* pParent /*=NULL*/)
	: CDialog(CSamplePanelDialog2::IDD, pParent)
{
	m_bPaneClosed = false;
	m_uRecvSelection = 0;
	m_receiver.Listen(IMC_SAMPLE_VIEW1_PANE3);
	m_receiver.Register(this);
}



CSamplePanelDialog2::~CSamplePanelDialog2()
{
}

void CSamplePanelDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboControl);
}

BOOL CSamplePanelDialog2::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_comboControl.InsertString(0,_T("Item1"));
	m_comboControl.InsertString(1,_T("Item2"));
	m_comboControl.InsertString(2,_T("Item3"));
	m_comboControl.SetCurSel(0);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CSamplePanelDialog2, CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_RECVCOMBOSEL_FROM_PANE1, OnRecvSelection)
	
END_MESSAGE_MAP()


// CSamplePanelDialog2 message handlers

void CSamplePanelDialog2::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_bPaneClosed = true;
	CDialog::OnClose();
}

void CSamplePanelDialog2::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

BOOL CSamplePanelDialog2::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DestroyWindow();
}

void CSamplePanelDialog2::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::PostNcDestroy();
}

LRESULT CSamplePanelDialog2::OnMessageReceived(CMessage* pMessage)
{
	UINT nMessageID = pMessage->message;
	if (nMessageID == IMC_SAMPLE_VIEW1_PANE3)
	{
		CMsgView1ToPane3* pMsg = dynamic_cast<CMsgView1ToPane3*>(pMessage);
		m_uRecvSelection = pMsg->m_comboSelection;
		PostMessage(WM_RECVCOMBOSEL_FROM_PANE1);
	}
	return 0;
}

LRESULT CSamplePanelDialog2::OnRecvSelection(WPARAM w, LPARAM l)
{
	CString str;
	switch(m_uRecvSelection)
	{
	case 0:
		str = _T("Item1");
		break;
	case 1:
		str = _T("Item2");
		break;
	case 2:
		str = _T("Item3");
		break;
	default:
		break;
	}
	CString strInfo;
	strInfo.Format(_T("Receive selection = %s from sampleviewer module."), str);
	MessageBox(strInfo, _T("工程二面板二收到消息："));

	m_comboControl.SetCurSel(m_uRecvSelection);
	UpdateData(FALSE);

	return 0;

}