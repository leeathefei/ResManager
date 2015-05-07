// PaneDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "SampleViewerDialog.h"
#include "SampleViewerManager.h"
#include "..\Common\Imc.h"

static CSampleViewerDialog::DerivedRegister Derived_for_registering; // 没有其它机制能够保证在全局空间调用注册函数
// CSampleViewerDialog dialog

IMPLEMENT_DYNAMIC(CSampleViewerDialog, CDialog)

CSampleViewerDialog::CSampleViewerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleViewerDialog::IDD, pParent)
	, m_dbSent2Module(0)
	, m_radioValue(0)
	, m_dbAttitude(0)
	, m_dbLattitude(0)
	, m_dbTensity(0)
{
   m_bPaneClosed = false;
   RegisterImc();
}

void CSampleViewerDialog::RegisterImc()
{
	m_ImcReceiver.Listen(IMC_SAMPLE_PANE1_VIEW1);
	m_ImcReceiver.Register(this);
}

CSampleViewerDialog::~CSampleViewerDialog()
{
}

void CSampleViewerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SENDVALUE, m_editSend2Module);
	DDX_Text(pDX, IDC_EDIT_SENDVALUE, m_dbSent2Module);
	DDX_Radio(pDX, IDC_RADIO1, m_radioValue);
	DDX_Control(pDX, IDC_COMBO1, m_comboControl);
	DDX_Text(pDX, IDC_EDIT1, m_dbAttitude);
	DDV_MinMaxDouble(pDX, m_dbAttitude, 0, 180.0);
	DDX_Text(pDX, IDC_EDIT2, m_dbLattitude);
	DDV_MinMaxDouble(pDX, m_dbLattitude, 0, 180.0);
	DDX_Text(pDX, IDC_EDIT3, m_dbTensity);
}

BOOL CSampleViewerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_comboControl.InsertString(0,_T("Item1"));
	m_comboControl.InsertString(1,_T("Item2"));
	m_comboControl.InsertString(2,_T("Item3"));
	m_comboControl.SetCurSel(0);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CSampleViewerDialog, CDialog)
   ON_WM_CLOSE()
   ON_WM_DESTROY()
    ON_MESSAGE(WM_RECVALUE_PANE1_VIEW1,OnRecvValueFromPane)
   ON_BN_CLICKED(IDC_BUTTON1, &CSampleViewerDialog::OnBnClickedButton1)
   ON_BN_CLICKED(IDC_BUTTON2, &CSampleViewerDialog::OnBnClickedButton2)
   ON_BN_CLICKED(IDC_BUTTON3, &CSampleViewerDialog::OnBnClickedButton3)
   ON_BN_CLICKED(IDC_BUTTON4, &CSampleViewerDialog::OnBnClickedButton4)
END_MESSAGE_MAP()


// CSampleViewerDialog message handlers

void CSampleViewerDialog::OnClose()
{
   // TODO: Add your message handler code here and/or call default
   m_bPaneClosed = true;
   CDialog::OnClose();
}

void CSampleViewerDialog::OnDestroy()
{
   CDialog::OnDestroy();

   // TODO: Add your message handler code here
}

BOOL CSampleViewerDialog::DestroyWindow()
{
   // TODO: Add your specialized code here and/or call the base class

   return CDialog::DestroyWindow();
}

void CSampleViewerDialog::PostNcDestroy()
{
   // TODO: Add your specialized code here and/or call the base class

   CDialog::PostNcDestroy();
}


void CSampleViewerDialog::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CImcSender sender;
	sender.SendView1ToPanel1(m_dbSent2Module);

	// TODO: Add your control notification handler code here
}

void CSampleViewerDialog::OnBnClickedButton3()
{
	UpdateData(TRUE);
	CImcSender sender;
	sender.SendView1ToPanel3(m_comboControl.GetCurSel());
}

void CSampleViewerDialog::OnBnClickedButton2()
{
	UpdateData(TRUE);
	CImcSender sender;
	sender.SendView1ToPanel2(m_radioValue);

	// TODO: Add your control notification handler code here
}


void CSampleViewerDialog::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CImcSender sender;
	sender.SendView1ToPanel4(m_dbAttitude, m_dbLattitude,m_dbTensity);
}

LRESULT CSampleViewerDialog::OnMessageReceived(CMessage* pMessage)
{
	UINT uMessageID = pMessage->message;
	switch( pMessage->message )
	{
		//Before loading log file.
	case IMC_SAMPLE_PANE1_VIEW1:
		{
			CMsgPanel1ToView1* pMsg = dynamic_cast<CMsgPanel1ToView1*> (pMessage);
			m_dbRecvValue = pMsg->dbValue;
			PostMessage(WM_RECVALUE_PANE1_VIEW1);
			return 0;
		}
		break;
	default:
		break;

	}

	return 0;
}

LRESULT CSampleViewerDialog::OnRecvValueFromPane(WPARAM W,LPARAM L)
{
	CString strValue;
	strValue.Format(_T("Receive value  = %f,which from SamplePanel of pane1."), m_dbRecvValue);
	MessageBox(strValue,_T("工程一面板一收到消息："));

	m_dbSent2Module = m_dbRecvValue;
	UpdateData(FALSE);

	return 0;
}


CWnd* CSampleViewerDialog::CreateDlgObj()
{
	return new CSampleViewerDialog;
}

BOOL CSampleViewerDialog::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType)
{
	USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));
	__super::Create(IDD, pParent);
	return CSampleViewerManager::Instance()->RegisterDockPane(this, _T("SampleViewerDialog"),eDockType, TRUE);
}