// SamplePanelDialog2.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePanel.h"
#include "SamplePanelDialog4.h"
#include "SamplePanelManager.h"


static CSamplePanelDialog4::stRegister s_Register;
// CSamplePanelDialog4 dialog

IMPLEMENT_DYNAMIC(CSamplePanelDialog4, CDialog)

	CSamplePanelDialog4::CSamplePanelDialog4(CWnd* pParent /*=NULL*/)
	: CDialog(CSamplePanelDialog4::IDD, pParent)
	, m_dbEditvalue1(0)
	, m_dbEditValue2(0)
	, m_dbEditValue3(0)
{
	m_bPaneClosed = false;
}

CSamplePanelDialog4::~CSamplePanelDialog4()
{
}

void CSamplePanelDialog4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editEditValue1);
	DDX_Text(pDX, IDC_EDIT1, m_dbEditvalue1);
	DDV_MinMaxDouble(pDX, m_dbEditvalue1, 0.0, 180.0);
	DDX_Control(pDX, IDC_EDIT2, m_editValue2);
	DDX_Control(pDX, IDC_EDIT3, m_editValue3);
	DDX_Text(pDX, IDC_EDIT2, m_dbEditValue2);
	DDV_MinMaxDouble(pDX, m_dbEditValue2, 0.0, 180.0);
	DDX_Text(pDX, IDC_EDIT3, m_dbEditValue3);
	DDV_MinMaxDouble(pDX, m_dbEditValue3, 0.0, 1000.0);
}


BEGIN_MESSAGE_MAP(CSamplePanelDialog4, CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CSamplePanelDialog4::OnBnClickedButton1)
	ON_MESSAGE(WM_RECV_POSITION, OnRecvPostion)
END_MESSAGE_MAP()


// CSamplePanelDialog4 message handlers

void CSamplePanelDialog4::OnClose()
{
	CWndManager::Instance()->RemoveCreatedWnd(this/*, _T("CSamplePanelDialog4")*/);
	// TODO: Add your message handler code here and/or call default
	m_bPaneClosed = true;
	CDialog::OnClose();
}

void CSamplePanelDialog4::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

BOOL CSamplePanelDialog4::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DestroyWindow();
}

void CSamplePanelDialog4::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::PostNcDestroy();
}


void CSamplePanelDialog4::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}

LRESULT CSamplePanelDialog4::OnRecvPostion(WPARAM W,LPARAM L)
{
	CString strValue;
	strValue.Format(_T("Receive values Attitude= %f,Lattitue=%f,Tensity=%f,which from SampleViewer of pane1."),m_dbEditvalue1,m_dbEditValue2,m_dbEditValue3);
	MessageBox(strValue,_T("工程二面板四收到消息："));

	UpdateData(FALSE);
	return 0;
}
CWnd* CSamplePanelDialog4::CreateDlgObj()
{
	return new CSamplePanelDialog4;
}
BOOL CSamplePanelDialog4::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName,BOOL bAutoDelete)
{
	USE_CUSTOM_RESOURCE(_T("SamplePanel.dll"));
	__super::Create(IDD, pParent);
	if (eDockType == ALIGN_CHILD_WITH_TITLE)
	{
		return TRUE;
	}
	else if (eDockType == ALIGN_CHILD_WITH_NO_TITLE)
	{
		ModifyStyle(WS_CAPTION|WS_BORDER|WS_SIZEBOX,0);
		return TRUE;
	}
	else
	{
		ModifyStyle(WS_BORDER|WS_CAPTION|WS_SIZEBOX,0);
		return CSamplePanelManager::Instance()->RegisterDockPane(this, strWndName.GetString(),eDockType, bAutoDelete);
	}
}