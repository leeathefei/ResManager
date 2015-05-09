#include "StdAfx.h"
#include "SampleViewerManager.h"
#include "SampleViewerDoc.h"
#include "SampleViewerFrame.h"
#include "SampleViewerView.h"
#include "SampleViewerDialog.h"
#include "Resource.h"

CSampleViewerManager* CSampleViewerManager::ms_pInstance=NULL;

CSampleViewerManager* CSampleViewerManager::Instance()
{
   if(!ms_pInstance)
   {
      ms_pInstance = new CSampleViewerManager;
   }

   return ms_pInstance;
}

CSampleViewerManager::CSampleViewerManager(void)
{
   m_pModuleManager = new CModuleManager;

   m_iToolbarRefCnt = 0;
   m_pToolbar = NULL;
   for(int i=0; i<4; ++i)
   {
      m_pModuleDlg[i] = NULL;
   }
   
   m_dwAlign[0] = ALIGN_VERTICAL;
   m_dwAlign[1] = ALIGN_VERTICAL;
   m_dwAlign[2] = ALIGN_HORIZONTAL;
   m_dwAlign[3] = ALIGN_HORIZONTAL;


   m_strModuleName[0] = _T("����һ�Ĵ�ֱ�Զ�ɾ�����1");
   m_strModuleName[1] = _T("����һ�Ĵ�ֱ�ɹر����2");
   m_strModuleName[2] = _T("����һ��ˮƽ�Զ�ɾ�����3");
   m_strModuleName[3] = _T("����һ��ˮƽ�ɹر����4");

   m_bAutoDelete[0] = true;
   m_bAutoDelete[1] = false;
   m_bAutoDelete[2] = true;
   m_bAutoDelete[3] = false;

}

CSampleViewerManager::~CSampleViewerManager(void)
{
   delete m_pToolbar;
   m_pToolbar = NULL;

   delete m_pModuleManager;
}

void CSampleViewerManager::DestroyInstance()
{
	delete ms_pInstance;
	ms_pInstance = NULL;
}

BOOL CSampleViewerManager::RegisterDocTemplate()
{
   // �л���DLL��Դ
   USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));


   // ����һ���ĵ�ģ�����
   CMTDocTemplate* pDocTemplate = new CMTDocTemplate(IDR_SAMPLEVIEWER_TYPE,
      RUNTIME_CLASS(CSampleViewerDoc),
      RUNTIME_CLASS(CSampleViewerFrame),
      RUNTIME_CLASS(CSampleViewerView),
      _T("SampleViewer.dll"), _T("SampleViewer"));


   // ���û���ĺ���ʵ���������ĵ�ģ��ע��
   return m_pModuleManager->RegisterDocTemplate(pDocTemplate);
}

BOOL CSampleViewerManager::RegisterToolBar()
{
   // �л���DLL��Դ
   USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));

   CMFCToolBar* pToolbar=NULL;
   if( m_pModuleManager->RegisterToolBar(IDR_SAMPLEVIEWER_TOOLBAR, pToolbar) )
   {
      m_pToolbar = pToolbar;
      return TRUE;
   }
   else
      return FALSE;

}

void CSampleViewerManager::RemoveToolBar()
{
   m_pModuleManager->RemoveToolBar(m_pToolbar);
}

void CSampleViewerManager::ShowToolBar(BOOL bShow)
{
   m_pModuleManager->ShowToolBar(m_pToolbar, bShow);
}

BOOL CSampleViewerManager::RegisterDockPane(CWnd* pAttachWnd, 
											LPCTSTR lpszWndName,
											EPANE_ALIGNMENT eDockDir,
											BOOL bAutoDelete)
{	
	USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));

	HICON hIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), 
		MAKEINTRESOURCE(IDR_SAMPLEVIEWER_TYPE),
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

BOOL CSampleViewerManager::RegisterModulePane()
{
   // �л���DLL��Դ
   USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));

   /*  CDialog* pDlg = (CDialog*)CWndManager::Instance()->Create(_T("CSampleViewerDialog"));
   CBaseObj*pBase = dynamic_cast<CBaseObj*>(pDlg);
   if (NULL != pBase)
   {
   pBase->CreateWnd(NULL);
   pDlg->ShowWindow(SW_SHOW);
   }

   */
   for(int i=0; i<2; ++i)
   {
      if(m_pModuleDlg[i]==NULL)
         m_pModuleDlg[i] = new CSampleViewerDialog(NULL);
      if(m_pModuleDlg[i]->GetSafeHwnd()==NULL)
         m_pModuleDlg[i]->Create(CSampleViewerDialog::IDD, NULL);

      HICON hIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), 
         MAKEINTRESOURCE(IDR_SAMPLEVIEWER_TYPE),
         IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);

      m_pModuleManager->RegisterModulePane(
         m_pModuleDlg[i], 
         m_strModuleName[i], 
         hIcon, 
         m_dwAlign[i], 
         true, 
         m_bAutoDelete[i]);
   }

   return TRUE;
}

BOOL CSampleViewerManager::CanClose(CString& strMessage/*=CString(_T(""))*/)
{
   USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));


   //TODO: ��Ӵ����滻����һ�У����߱�������һ��
   return m_pModuleManager->CanClose(strMessage);
}

void CSampleViewerManager::Terminate()
{
   // �л���DLL��Դ
   USE_CUSTOM_RESOURCE(_T("SampleViewer.dll"));
   
   //TODO: ��Ӵ����滻����һ�У����߱�������һ��
   m_pModuleManager->Terminate();
   
   /*for(int i=0; i<4; ++i)
   {
	   m_pModuleDlg[i]->PostMessage(WM_CLOSE);
	   m_pModuleDlg[i]->DestroyWindow();
	   delete m_pModuleDlg[i];
   }*/


   CSampleViewerManager::DestroyInstance();

}

void CSampleViewerManager::AddFrameWnd( CFrameWnd* pFrameWnd )
{
   m_InstanceList.AddTail(pFrameWnd);
}

void CSampleViewerManager::RemoveFrameWnd( CFrameWnd* pFrameWnd )
{
   POSITION pos=NULL;
   pos = m_InstanceList.Find(pFrameWnd, pos);
   if (pos)
   {
      m_InstanceList.RemoveAt(pos);
   }

   if( m_InstanceList.IsEmpty() )
   {
      ShowToolBar(SW_HIDE);
   }
}

void CSampleViewerManager::ActivatePane( CString strWindowName )
{
   m_pModuleManager->ActivatePane(strWindowName);
}

BOOL CSampleViewerManager::UnregisterModulePane(LPCTSTR lpszWndName)
{
   return m_pModuleManager->UnregisterModulePane(lpszWndName);
}








//////////////////////////////////////////////////////////////////////////
// exported functions
// 
extern "C"
{
	//new api for register toolbar.
	void __declspec(dllexport) AddToolbar()
	{
		CSampleViewerManager::Instance()->RegisterToolBar();
	}

   void __declspec(dllexport) Init(WORD w)
   {
      CSampleViewerManager::Instance()->RegisterDocTemplate();
      //CSampleViewerManager::Instance()->RegisterToolBar();
   }

   BOOL __declspec(dllexport) GetIconResourceID(UINT& nResID)
   {
      nResID = IDR_SAMPLEVIEWER_TYPE;
      return TRUE;
   }

   void __declspec(dllexport) Term(WORD w)
   {
      CSampleViewerManager::Instance()->Terminate();

   }

   BOOL __declspec(dllexport) CanClose(CString& strMessage)
   {
      return TRUE;
   }

   void __declspec(dllexport) LoadModulePane(WORD w)
   {
     // CSampleViewerManager::Instance()->RegisterModulePane();
   }

};
