// DlgTest1InSV.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "DlgTest1InSV.h"
#include "afxdialogex.h"

static CDlgTest1InSV::stRegister s_Register;

IMPLEMENT_DYNAMIC(CDlgTest1InSV, CDialogEx)

CDlgTest1InSV::CDlgTest1InSV(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTest1InSV::IDD, pParent)
{

}

CDlgTest1InSV::~CDlgTest1InSV()
{
}

void CDlgTest1InSV::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTest1InSV, CDialogEx)
END_MESSAGE_MAP()


CWnd* CDlgTest1InSV::CreateDlgObj()
{
	return new CDlgTest1InSV;
}

BOOL CDlgTest1InSV::CreateWnd(CWnd* pParent)
{
	return __super::Create(IDD, pParent);
}