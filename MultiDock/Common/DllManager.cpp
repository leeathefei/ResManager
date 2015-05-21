#include "StdAfx.h"
#include "DllManager.h"
#include "DataManager.h"
#include "UserDataManager.h"



/////////////////////////////////////////////


//���ݽ�����Ϣ
UINT PLAT_DLL_DATAREVMSG = ::RegisterWindowMessage (_T("PLAT_DLL_DATAREVMSG"));
//���ڼ�����Ϣ
UINT PLAT_DLL_WNDACTIVEMSG = ::RegisterWindowMessage (_T("PLAT_DLL_WNDACTIVEMSG"));
//�˵���������Ӧ��Ϣ
UINT PLAT_DLL_COMMANDMSG = ::RegisterWindowMessage (_T("PLAT_DLL_COMMANDMSG"));

PROC_SENDDATA	g_SendDataProc;	
PROC_CREATEEXAMP g_CreateExampProc;
PROC_CTRLEXAMP g_ControlExampProc;
PROC_APPLAYTOPLAT g_ApplyFunctionProc;

CDllManager::CDllManager()
{
//	g_pDllManager = this;
}

CDllManager::~CDllManager()
{
}
///////////////////////////////////////////////////////////////////
///////////////////��̬���ʼ������/////////////////////////////
//pDB:Ҫ���ʵ����ݿ�ָ��

///////////////////////////////////////////////////////////////////////
//////////////////////��̬�ⴰ����Ϣ��ȡ����////////////////////////////

void CDllManager::Sys_InitDllFunc(LPCTSTR  lpszExampID, DWORD& pExampClass ,  LPCTSTR lpszCmdParam, char* pchSubParam, DWORD dwSubParamLen)
{
	CUserDataManager* l_pDataManager = new CUserDataManager();
	l_pDataManager->m_lpszExampID = lpszExampID;
	l_pDataManager->InitModuleFunc( lpszCmdParam,pchSubParam, dwSubParamLen);
	pExampClass = (DWORD)l_pDataManager;

}

BOOL CDllManager::Sys_CreateDllWindow(CList<stWndInfo*,stWndInfo*> &pList,  LPCTSTR lpszTransfModeID, DWORD dwpClass,CWnd *pWnd)
{
	CUserDataManager * lpDataManager = (CUserDataManager *)dwpClass;
	if( !lpDataManager->CreateCustomWindow(lpszTransfModeID,pWnd))
	{
		lpDataManager->FreeDlgResource();
		return FALSE;
	}
	pList.AddTail(&lpDataManager->m_WndList);

	const CRuntimeClass* pClass=NULL;
    AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
    for (pClass = pModuleState->m_classList; pClass != NULL;pClass = pClass->m_pNextClass)
	{
        if (lstrcmp(LPCWSTR("CMenuTest"), LPCWSTR(pClass->m_lpszClassName)) == 0)
		{
            break;
		}
	}
    if(pClass)
	{
		AfxMessageBox(_T("sun"));
		(pClass->m_pfnCreateObject)();
	}
	return true;
}
BOOL CDllManager::Sys_ReceiveData( DWORD dwInfoCode, char* pBuf ,DWORD dwBufLen, DWORD dwExampClass , LPCTSTR lpszSrcID )
{
	CUserDataManager* pExampClass = (CUserDataManager*)dwExampClass;
	if (pExampClass != NULL)
	{
		pExampClass->OnReceiveDataFunc(dwInfoCode, pBuf,dwBufLen, lpszSrcID);
	}
	return TRUE;
}
BOOL	CDllManager::Sys_RevMsgFromPlat (  char* pBuf ,DWORD dwBufLen, DWORD dwExampClass )
{
	CUserDataManager* pExampClass = (CUserDataManager*)dwExampClass;
	if (pExampClass != NULL)
	{
		pExampClass->OnRevMsgFromPlat(pBuf,dwBufLen);
	}
	return TRUE;
}

void CDllManager::Sys_SendData(PROC_SENDDATA _Proc)  // ��������
{
	g_SendDataProc = _Proc;
}
void CDllManager::Sys_ApplyCreateExamp(PROC_CREATEEXAMP _Proc)  // ��������
{
	g_CreateExampProc = _Proc;
}

void CDllManager::Sys_ApplyControlExamp(PROC_CTRLEXAMP _Proc)  // ��������
{
	g_ControlExampProc = _Proc;
}

void CDllManager::Sys_ApplyFunction(PROC_APPLAYTOPLAT _Proc)  // ��������
{
	g_ApplyFunctionProc = _Proc;
}

BOOL CDllManager::Sys_FreeDlgResource(DWORD dwExampClass) 
{	
	CUserDataManager* pExampClass = (CUserDataManager*)dwExampClass;

	if (pExampClass)
	{
		pExampClass->FreeDlgResource();
		delete pExampClass;
		pExampClass = NULL;
	}
	return TRUE;
}
BOOL	CDllManager::SendDataToExtern(CDataManager* pDataManager,  DWORD dwInfoCode, char* pBuf ,DWORD dwBufLen , LPCTSTR lpszDstID )
 {
	return  g_SendDataProc( pDataManager->m_lpszExampID, dwInfoCode, pBuf, dwBufLen, lpszDstID);
 }

BOOL	CDllManager::ApplyCreateExamp(  CDataManager* pDataManager, LPCTSTR lpszModuleTypeID, LPCTSTR lpszTransfModeID, CString& lpszAimExampID, char* pchSubParam , DWORD dwSubParamLen,CWnd *pWnd )
{
	return g_CreateExampProc(pDataManager->m_lpszExampID, lpszModuleTypeID, lpszTransfModeID, lpszAimExampID, pchSubParam, dwSubParamLen,pWnd);
}
BOOL	CDllManager::ApplyControlExamp(  CDataManager* pDataManager, BYTE byCtrlMode,  LPCTSTR lpszAimExampID , LPCTSTR lpszWndID )
{
	return g_ControlExampProc( pDataManager->m_lpszExampID, byCtrlMode, lpszAimExampID, lpszWndID );
}

BOOL	CDllManager::ApplyFunctionToPlat(CDataManager* pDataManager, char* pBuf ,DWORD dwBufLen  )
{
	return  g_ApplyFunctionProc(pDataManager->m_lpszExampID, pBuf, dwBufLen);
}

