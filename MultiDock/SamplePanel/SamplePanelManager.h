#pragma once
#include "..\common\ModuleManager.h"

class CSamplePanelDialog;
class CSamplePanelDialog2;
class CSamplePanelDialog3;
class CSamplePanelDialog4;

class CSamplePanelManager
{
public:
	CSamplePanelManager(void);
	~CSamplePanelManager(void);

	static CSamplePanelManager* Instance();
	static CSamplePanelManager* ms_pInstance;
	static void DestroyInstance();


public:
	BOOL RegisterDocTemplate();
	BOOL RegisterToolBar();
	void RemoveToolBar();
	void ShowToolBar(BOOL bShow=TRUE);
	void ActivatePane(CString strWindowName);
	BOOL RegisterModulePane();
	BOOL UnregisterModulePane(LPCTSTR);
	BOOL CanClose(CString& strMessage);
	void Terminate();

public:

	void AddFrameWnd(CFrameWnd* pFrmaeWnd);
	void RemoveFrameWnd(CFrameWnd* pFrmeWnd);

private:
	CModuleManager* m_pModuleManager;


	CSamplePanelDialog*   m_pModuleDlg1;
	CSamplePanelDialog2*  m_pModuleDlg2;
	CSamplePanelDialog3*  m_pModuleDlg3;
	CSamplePanelDialog4*  m_pModuleDlg4;

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
