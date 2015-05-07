// DlgCreateChildPane.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgCreateChildPane.h"
#include "afxdialogex.h"


// CDlgCreateChildPane dialog

IMPLEMENT_DYNAMIC(CDlgCreateChildPane, CDialogEx)

CDlgCreateChildPane::CDlgCreateChildPane(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateChildPane::IDD, pParent)
{

}

CDlgCreateChildPane::~CDlgCreateChildPane()
{
}

void CDlgCreateChildPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCreateChildPane, CDialogEx)
END_MESSAGE_MAP()


// CDlgCreateChildPane message handlers
