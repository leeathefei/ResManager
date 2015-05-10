#pragma once
#include "afxwin.h"
#include "resource.h"
#include "..\Common\WndManager.h"

// CDlgTest1 dialog


class CDlgTest1 : public CDialogEx, public CBaseObj
{
	DECLARE_DYNAMIC(CDlgTest1)

public:
	CDlgTest1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest1();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

	struct DerivedRegister // 辅助类，用于注册 
	{
		DerivedRegister()
		{ // 注册子类，虽然map能保证唯一，但仍只注册一次
			LPCTSTR className = _T("CDlgTest1");
			static bool bRegistered = false;
			if(!bRegistered)
			{
				CWndManager::Instance()->Register(className, CDlgTest1::CreateDlgObj); // 注册子类信息 
				bRegistered = true;
			}
		}    
	};

	static CWnd* CreateDlgObj( );
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName);

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CComboBox m_comboBox1;
	CComboBox m_comboBox2;
	CComboBox m_comboBox3;
};
