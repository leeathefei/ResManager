// DlgTest1.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgTest1.h"
#include "afxdialogex.h"
#include "MultiDockView.h"



static CDlgTest1::DerivedRegister Derived_for_registering; // 没有其它机制能够保证在全局空间调用注册函数，
// CDlgTest1 dialog

IMPLEMENT_DYNAMIC(CDlgTest1, CDialogEx)

CDlgTest1::CDlgTest1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTest1::IDD, pParent)
{

}

CDlgTest1::~CDlgTest1()
{
}

void CDlgTest1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox1);
	DDX_Control(pDX, IDC_COMBO2, m_comboBox2);
	DDX_Control(pDX, IDC_COMBO3, m_comboBox3);
}


BEGIN_MESSAGE_MAP(CDlgTest1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTest1::OnBnClickedButton1)
END_MESSAGE_MAP()


BOOL CDlgTest1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_comboBox1.InsertString(0, _T("This is value of ComboBox1"));
	m_comboBox2.InsertString(0, _T("This is value of ComboBox2"));
	m_comboBox3.InsertString(0, _T("This is value of ComboBox3"));
	m_comboBox1.SetCurSel(0);
	m_comboBox2.SetCurSel(0);
	m_comboBox3.SetCurSel(0);

	return TRUE;
}
// CDlgTest1 message handlers


void CDlgTest1::OnBnClickedButton1()
{

}
CWnd* CDlgTest1::CreateDlgObj()
{
	return new CDlgTest1;
}

BOOL CDlgTest1::CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName)
{
	return __super::Create(IDD, pParent);
}