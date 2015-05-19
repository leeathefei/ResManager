#pragma once
#include "Resource.h"
#include "afxwin.h"
#include "..\Common\WndManager.h"

// CSampleViewerDialog dialog
#define WM_RECVALUE_PANE1_VIEW1 WM_USER+2017

class CSampleViewerDialog : public CDialog,public CBaseObj
{
	DECLARE_DYNAMIC(CSampleViewerDialog)

public:
	CSampleViewerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSampleViewerDialog();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_PANEDIALOG };

	struct DerivedRegister // �����࣬����ע�� 
	{
		DerivedRegister()
		{ // ע�����࣬��Ȼmap�ܱ�֤Ψһ������ֻע��һ��
			LPCTSTR className = _T("CSampleViewerDialog");
			static bool bRegistered = false;
			if(!bRegistered)
			{
				CWndManager::Instance()->Register(className, CSampleViewerDialog::CreateDlgObj); // ע��������Ϣ 
				bRegistered = true;
			}
		}    
	};

	static CWnd* CreateDlgObj( );
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName,BOOL bAutoDelete=TRUE);

   bool m_bPaneClosed;
 
   double m_dbRecvValue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnClose();
   afx_msg void OnDestroy();
   afx_msg LRESULT OnRecvValueFromPane(WPARAM W,LPARAM L);
   virtual BOOL DestroyWindow();
protected:
   virtual void PostNcDestroy();
public:
	CEdit m_editSend2Module;
	double m_dbSent2Module;
	afx_msg void OnBnClickedButton1();
	int m_radioValue;
	afx_msg void OnBnClickedButton2();
	CComboBox m_comboControl;
	afx_msg void OnBnClickedButton3();
	double m_dbAttitude;
	double m_dbLattitude;
	afx_msg void OnBnClickedButton4();
	double m_dbTensity;
};
