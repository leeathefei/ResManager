// SamplePanelDialog2.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePanel.h"
#include "SamplePanelDialog3.h"
#include "SamplePanelManager.h"

static CSamplePanelDialog3::stRegister s_Register;

// CSamplePanelDialog3 dialog

IMPLEMENT_DYNAMIC(CSamplePanelDialog3, CDialog)

	CSamplePanelDialog3::CSamplePanelDialog3(CWnd* pParent /*=NULL*/)
	: CDialog(CSamplePanelDialog3::IDD, pParent)
	, m_nRadio(FALSE)
{
	m_bPaneClosed = false;
	RegisterImc();
	m_nRadioValue = -1;
}

void CSamplePanelDialog3::RegisterImc()
{
	m_ImcReceiver.Listen(IMC_SAMPLE_VIEW1_PANE2);
	m_ImcReceiver.Register(this);
}

CSamplePanelDialog3::~CSamplePanelDialog3()
{
}

void CSamplePanelDialog3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
}


BEGIN_MESSAGE_MAP(CSamplePanelDialog3, CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_RECVALUE_VIEWER1_PANE2,OnRecvValueFromView)
	//ON_BN_CLICKED(IDC_BUTTON1, &CSamplePanelDialog3::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSamplePanelDialog3 message handlers

void CSamplePanelDialog3::OnClose()
{
	CWndManager::Instance()->RemoveCreatedWnd(this/*, _T("CSamplePanelDialog3")*/);
	// TODO: Add your message handler code here and/or call default
	m_bPaneClosed = true;
	CDialog::OnClose();
}

void CSamplePanelDialog3::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

BOOL CSamplePanelDialog3::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DestroyWindow();
}

void CSamplePanelDialog3::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::PostNcDestroy();
}

LRESULT CSamplePanelDialog3::OnMessageReceived(CMessage* pMessage)
{
	UINT uMessageID = pMessage->message;
	switch( pMessage->message )
	{
	case IMC_SAMPLE_VIEW1_PANE2:
		{
			CMsgView1ToPanel2* pMsg = dynamic_cast<CMsgView1ToPanel2*>(pMessage);
			m_nRadioValue = pMsg->radioValue;
			PostMessage(WM_RECVALUE_VIEWER1_PANE2);
			return 0;
		}
	default:
		break;

	}

	return 0;
}

LRESULT CSamplePanelDialog3::OnRecvValueFromView(WPARAM W,LPARAM L)
{
	CString strValue;
	strValue.Format(_T("Receive value  = %d,which from SampleViewer of pane1."),m_nRadioValue);
	MessageBox(strValue,_T("工程二面板三收到消息："));

	m_nRadio = m_nRadioValue;
	UpdateData(FALSE);

	return 0;
}

void CSamplePanelDialog3::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}

CWnd* CSamplePanelDialog3::CreateDlgObj()
{
	return new CSamplePanelDialog3;
}
BOOL CSamplePanelDialog3::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName)
{
	USE_CUSTOM_RESOURCE(_T("SamplePanel.dll"));
	__super::Create(IDD, pParent);
	if (eDockType == ALIGN_CHILD)
	{
		return TRUE;
	}
	else
	{
		return CSamplePanelManager::Instance()->RegisterDockPane(this, strWndName.GetString(),eDockType, TRUE);
	}
}