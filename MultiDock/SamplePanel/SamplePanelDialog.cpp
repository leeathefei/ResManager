// PaneDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePanel.h"
#include "SamplePanelDialog.h"
#include "SamplePanelManager.h"

static CSamplePanelDialog::stRegister s_Register;
// CSamplePanelDialog dialog

IMPLEMENT_DYNAMIC(CSamplePanelDialog, CDialog)

CSamplePanelDialog::CSamplePanelDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSamplePanelDialog::IDD, pParent)
	, m_dEdit1Value(0)
{
	m_dbRecvValue = 0;
   m_bPaneClosed = false;
   RegisterImc();
}
void CSamplePanelDialog::RegisterImc()
{
	m_ImcReceiver.Listen(IMC_SAMPLE_VIEW1_PANE1);
	m_ImcReceiver.Register(this);
}
CSamplePanelDialog::~CSamplePanelDialog()
{
}

void CSamplePanelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1Value);
	DDX_Text(pDX, IDC_EDIT1, m_dEdit1Value);
}


BEGIN_MESSAGE_MAP(CSamplePanelDialog, CDialog)
   ON_WM_CLOSE()
   ON_WM_DESTROY()
   ON_WM_SIZE()
   ON_MESSAGE(WM_RECVALUE_FROM_VIEWER,OnRecvValueFromView)
   ON_BN_CLICKED(IDC_BUTTON1, &CSamplePanelDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSamplePanelDialog message handlers

void CSamplePanelDialog::OnClose()
{
   // TODO: Add your message handler code here and/or call default
   m_bPaneClosed = true;
   CDialog::OnClose();
}

void CSamplePanelDialog::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	CRect rcFrame;
	GetWindowRect(rcFrame);
	ScreenToClient(rcFrame);

	CRect rcClient;
	GetClientRect(rcClient);

}

void CSamplePanelDialog::OnDestroy()
{
   CDialog::OnDestroy();

   // TODO: Add your message handler code here
}

BOOL CSamplePanelDialog::DestroyWindow()
{
   // TODO: Add your specialized code here and/or call the base class

   return CDialog::DestroyWindow();
}

void CSamplePanelDialog::PostNcDestroy()
{
   // TODO: Add your specialized code here and/or call the base class

   CDialog::PostNcDestroy();
}


void CSamplePanelDialog::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CImcSender sender;
	sender.SendPanel1ToView1(m_dEdit1Value);

	// TODO: Add your control notification handler code here
}
 LRESULT CSamplePanelDialog::OnMessageReceived(CMessage* pMessage)
 {
	 UINT uMessageID = pMessage->message;
	 switch( pMessage->message )
	 {
		 //Before loading log file.
	 case IMC_SAMPLE_VIEW1_PANE1:
		 {
			 CMsgView1ToPanel1* pMsg = dynamic_cast<CMsgView1ToPanel1*> (pMessage);
			 m_dbRecvValue = pMsg->dbValue;
			 PostMessage(WM_RECVALUE_FROM_VIEWER);
			 return 0;
		 }
		 break;
	 default:
		 break;

	 }

	 return 0;
 }

 LRESULT CSamplePanelDialog::OnRecvValueFromView(WPARAM W,LPARAM L)
 {
	 CString strValue;
	 strValue.Format(_T("Receive value  = %f,which from SampleViewer of pane1."), m_dbRecvValue);
	 MessageBox(strValue,_T("工程二面板一收到消息："));
	 
	 m_dEdit1Value = m_dbRecvValue;
	 UpdateData(FALSE);

	 return 0;
 }

 CWnd* CSamplePanelDialog::CreateDlgObj()
 {
	 return new CSamplePanelDialog;
 }
 BOOL CSamplePanelDialog::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType)
 {
	USE_CUSTOM_RESOURCE(_T("SamplePanel.dll"));
	 __super::Create(IDD, pParent);

	 if (eDockType == ALIGN_CHILD)
	 {
		 return TRUE;
	 }
	 else
	 {
		return CSamplePanelManager::Instance()->RegisterDockPane(this, _T("SamplePanelDialog"),eDockType, TRUE);
	 }
}