#pragma once
#include "..\common\ModuleManager.h"

class CSampleViewerDialog;
class CSampleViewerManager
{
public:
   CSampleViewerManager(void);
   ~CSampleViewerManager(void);
   
   static CSampleViewerManager* Instance();
   static CSampleViewerManager* ms_pInstance;
   static void DestroyInstance();


public:
   BOOL RegisterDocTemplate();
   BOOL RegisterToolBar();
   void RemoveToolBar();
   void ShowToolBar(BOOL bShow=TRUE);
   void ActivatePane(CString strWindowName);
   BOOL RegisterModulePane();

   BOOL RegisterDockPane(CWnd* pAttachWnd, 
	   LPCTSTR lpszWndName,
	   EPANE_ALIGNMENT eDockDir,
	   BOOL bAutoDelete);

   BOOL UnregisterModulePane(LPCTSTR);
   BOOL CanClose(CString& strMessage);
   void Terminate();

public:

   void AddFrameWnd(CFrameWnd* pFrmaeWnd);
   void RemoveFrameWnd(CFrameWnd* pFrmeWnd);

private:
   CModuleManager* m_pModuleManager;

  
   CSampleViewerDialog* m_pModuleDlg[4];
   CString   m_strModuleName[4];
   bool      m_bAutoDelete[4];
   EPANE_ALIGNMENT   m_dwAlign[4];


   //�����ӿ��ʹ�õ���ͬһ����������������Ҫ�����ӿ�ܵĸ���ά��һ��
   //�����������ü������Ӷ���֤����ȷ��ʱ����ʾ���غ��ͷŹ�������Դ
   //���dll����Ҫ�������Ͳ���Ҫ�˳�Ա����Ӧ�Ĺ����������ĺ�����
   CList<CFrameWnd*, CFrameWnd*> m_InstanceList;
   CMFCToolBar*    m_pToolbar;
   int m_iToolbarRefCnt;

};
