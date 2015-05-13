#include "StdAfx.h"
#include "SamplePanelManager.h"
#include "SamplePanelDoc.h"
#include "SamplePanelFrame.h"
#include "SamplePanelView.h"
#include "SamplePanelDialog.h"
#include "SamplePanelDialog2.h"
#include "SamplePanelDialog3.h"
#include "SamplePanelDialog4.h"
#include "Resource.h"

CSamplePanelManager* CSamplePanelManager::ms_pInstance=NULL;

CSamplePanelManager* CSamplePanelManager::Instance()
{
	if(!ms_pInstance)
	{
		ms_pInstance = new CSamplePanelManager;
	}

	return ms_pInstance;
}

void CSamplePanelManager::DestroyInstance()
{
	delete ms_pInstance;
	ms_pInstance = NULL;
}

CSamplePanelManager::CSamplePanelManager(void)
{
	m_pModuleManager = new CModuleManager;

	m_iToolbarRefCnt = 0;
	m_pToolbar = NULL;
// 	for(int i=0; i<4; ++i)
// 	{
// 		m_pModuleDlg[i] = NULL;
// 	}
	m_pModuleDlg1= NULL;
	m_pModuleDlg2= NULL;
	m_pModuleDlg3= NULL;
	m_pModuleDlg4= NULL;

	m_dwAlign[0] = ALIGN_LEFT /*ALIGN_VERTICAL*/;
	m_dwAlign[1] = ALIGN_TOP;
	m_dwAlign[2] = ALIGN_RIGHT;
	m_dwAlign[3] = ALIGN_BOTTON;


	m_strModuleName[0] = _T("工程二的自动删除面板1");
	m_strModuleName[1] = _T("工程二的可关闭面板2");
	m_strModuleName[2] = _T("工程二的自动删除面板3");
	m_strModuleName[3] = _T("工程二的可关闭面板4");

	m_bAutoDelete[0] = true;
	m_bAutoDelete[1] = false;
	m_bAutoDelete[2] = true;
	m_bAutoDelete[3] = false;

}

CSamplePanelManager::~CSamplePanelManager(void)
{
	delete m_pToolbar;
	m_pToolbar = NULL;

	delete m_pModuleManager;
}

BOOL CSamplePanelManager::RegisterDocTemplate()
{
	USE_CUSTOM_RESOURCE(_T("SamplePanel.dll"));

	CMTDocTemplate* pDocTemplate = new CMTDocTemplate(IDR_SAMPLEPANEL_MENU/*IDR_SAMPLEPANEL_TYPE*/,
		RUNTIME_CLASS(CSamplePanelDoc),
		RUNTIME_CLASS(CSamplePanelFrame),
		RUNTIME_CLASS(CSamplePanelView),
		_T("SamplePanel.dll"), _T("TestProj2.dll"));

	return m_pModuleManager->RegisterDocTemplate(pDocTemplate);
}

BOOL CSamplePanelManager::RegisterToolBar()
{
	// 切换到DLL资源
	USE_CUSTOM_RESOURCE(_T("SamplePanel.dll"));

	CMFCToolBar* pToolbar=NULL;
	if( m_pModuleManager->RegisterToolBar(IDR_SAMPLEPANEL_TOOLBAR, pToolbar) )
	{
	m_pToolbar = pToolbar;
	return TRUE;
	}
	else
	return FALSE;
}

void CSamplePanelManager::RemoveToolBar()
{
	m_pModuleManager->RemoveToolBar(m_pToolbar);
}

void CSamplePanelManager::ShowToolBar(BOOL bShow)
{
	m_pModuleManager->ShowToolBar(m_pToolbar, bShow);
}

BOOL CSamplePanelManager::RegisterDockPane(CWnd* pAttachWnd, 
											LPCTSTR lpszWndName,
											EPANE_ALIGNMENT eDockDir,
											BOOL bAutoDelete)
{	
	USE_CUSTOM_RESOURCE(_T("SamplePanel.dll"));

	HICON hIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), 
		MAKEINTRESOURCE(IDR_SAMPLEPANEL_MENU),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);

	m_pModuleManager->RegisterModulePane(
		pAttachWnd, 
		lpszWndName, 
		hIcon, 
		eDockDir, 
		true, 
		bAutoDelete);

	return TRUE;
}
BOOL CSamplePanelManager::RegisterModulePane()
{
	// 切换到DLL资源
	USE_CUSTOM_RESOURCE(_T("SamplePanel.dll"));


	//for(int i=0; i<4; ++i)
	//{
		


		//////////////////////////////////////////////////////////////////////////
		if(m_pModuleDlg1==NULL)
			m_pModuleDlg1= new CSamplePanelDialog(NULL);
		if(m_pModuleDlg1->GetSafeHwnd()==NULL)
			m_pModuleDlg1->Create(CSamplePanelDialog::IDD, NULL);
		m_pModuleDlg1->ModifyStyle(WS_CAPTION|WS_BORDER|WS_SIZEBOX,0);

		HICON hIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), 
			MAKEINTRESOURCE(IDR_SAMPLEPANEL_MENU),
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);

		m_pModuleManager->RegisterModulePane(
			m_pModuleDlg1, 
			m_strModuleName[0], 
			hIcon, 
			m_dwAlign[0], 
			true, 
			m_bAutoDelete[0]);

		//////////////////////////////////////////////////////////////////////////
		if(m_pModuleDlg2==NULL)
		m_pModuleDlg2= new CSamplePanelDialog2(NULL);
		if(m_pModuleDlg2->GetSafeHwnd()==NULL)
		m_pModuleDlg2->Create(CSamplePanelDialog2::IDD, NULL);
		m_pModuleDlg2->ModifyStyle(WS_CAPTION|WS_BORDER|WS_SIZEBOX,0);

		HICON hIcon2 = (HICON)::LoadImage(::AfxGetResourceHandle(), 
		MAKEINTRESOURCE(IDR_SAMPLEPANEL_MENU),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);

		m_pModuleManager->RegisterModulePane(
		m_pModuleDlg2, 
		m_strModuleName[1], 
		hIcon2, 
		m_dwAlign[1], 
		true, 
		m_bAutoDelete[1]);

		//////////////////////////////////////////////////////////////////////////
		if(m_pModuleDlg3==NULL)
			m_pModuleDlg3= new CSamplePanelDialog3(NULL);
		if(m_pModuleDlg3->GetSafeHwnd()==NULL)
			m_pModuleDlg3->Create(CSamplePanelDialog3::IDD, NULL);
		m_pModuleDlg3->ModifyStyle(WS_CAPTION|WS_BORDER|WS_SIZEBOX,0);

		HICON hIcon3 = (HICON)::LoadImage(::AfxGetResourceHandle(), 
			MAKEINTRESOURCE(IDR_SAMPLEPANEL_MENU),
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);

		m_pModuleManager->RegisterModulePane(
			m_pModuleDlg3, 
			m_strModuleName[2], 
			hIcon3, 
			m_dwAlign[2], 
			true, 
			m_bAutoDelete[2]);


		//////////////////////////////////////////////////////////////////////////
		if(m_pModuleDlg4==NULL)
		m_pModuleDlg4= new CSamplePanelDialog4(NULL);
		if(m_pModuleDlg4->GetSafeHwnd()==NULL)
		m_pModuleDlg4->Create(CSamplePanelDialog4::IDD, NULL);
		m_pModuleDlg4->ModifyStyle(WS_CAPTION|WS_BORDER|WS_SIZEBOX,0);

		HICON hIcon4 = (HICON)::LoadImage(::AfxGetResourceHandle(), 
		MAKEINTRESOURCE(IDR_SAMPLEPANEL_MENU),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);

		m_pModuleManager->RegisterModulePane(
		m_pModuleDlg4, 
		m_strModuleName[3], 
		hIcon4, 
		m_dwAlign[3], 
		true, 
		m_bAutoDelete[3]);


	
	//}

	return TRUE;
}

BOOL CSamplePanelManager::CanClose(CString& strMessage/*=CString(_T(""))*/)
{
	USE_CUSTOM_RESOURCE(_T("SamplePanel.dll"));


	//TODO: 添加代码替换下面一行，或者保留下面一行
	return m_pModuleManager->CanClose(strMessage);
}

void CSamplePanelManager::Terminate()
{
	// 切换到DLL资源
	USE_CUSTOM_RESOURCE(_T("SamplePanel.dll"));

	m_pModuleManager->Terminate();
	
 	m_pModuleDlg1->PostMessage(WM_CLOSE);
 	m_pModuleDlg1->DestroyWindow();
 	delete m_pModuleDlg1;

	m_pModuleDlg2->PostMessage(WM_CLOSE);
	m_pModuleDlg2->DestroyWindow();
	delete m_pModuleDlg2;

 	m_pModuleDlg3->PostMessage(WM_CLOSE);
 	m_pModuleDlg2->DestroyWindow();
 	delete m_pModuleDlg3;

	m_pModuleDlg4->PostMessage(WM_CLOSE);
	m_pModuleDlg3->DestroyWindow();
	delete m_pModuleDlg4;

	//for(int i=0; i<4; ++i)
	//{
	//	m_pModuleDlg[i]->PostMessage(WM_CLOSE);
	//	m_pModuleDlg[i]->DestroyWindow();
	//	delete m_pModuleDlg[i];
	//}

	CSamplePanelManager::DestroyInstance();

}

void CSamplePanelManager::AddFrameWnd( CFrameWnd* pFrameWnd )
{
	m_InstanceList.AddTail(pFrameWnd);
}

void CSamplePanelManager::RemoveFrameWnd( CFrameWnd* pFrameWnd )
{
	POSITION pos=NULL;
	pos = m_InstanceList.Find(pFrameWnd, pos);
	if (pos)
	{
		m_InstanceList.RemoveAt(pos);
	}

// 	if( m_InstanceList.IsEmpty() )
// 	{
// 		ShowToolBar(SW_HIDE);
// 	}
}

void CSamplePanelManager::ActivatePane( CString strWindowName )
{
	m_pModuleManager->ActivatePane(strWindowName);
}

BOOL CSamplePanelManager::UnregisterModulePane(LPCTSTR lpszWndName)
{
	return m_pModuleManager->UnregisterModulePane(lpszWndName);
}








//////////////////////////////////////////////////////////////////////////
// exported functions
// 
extern "C"
{
	void __declspec(dllexport) AddToolbar()
	{
		CSamplePanelManager::Instance()->RegisterToolBar();
	}

	void __declspec(dllexport) Init(WORD w)
	{
		CSamplePanelManager::Instance()->RegisterDocTemplate();
		//CSamplePanelManager::Instance()->RegisterToolBar();
	}

	BOOL __declspec(dllexport) GetIconResourceID(UINT& nResID)
	{
		nResID = IDR_SAMPLEPANEL_MENU;
		return TRUE;
	}

	void __declspec(dllexport) Term(WORD w)
	{
		CSamplePanelManager::Instance()->Terminate();
	}

	BOOL __declspec(dllexport) CanClose(CString& strMessage)
	{
		return TRUE;
	}

	void __declspec(dllexport) LoadModulePane(WORD w)
	{
		CSamplePanelManager::Instance()->RegisterModulePane();
	}

};
