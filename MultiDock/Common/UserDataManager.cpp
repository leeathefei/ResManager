#include "StdAfx.h"
#include "UserDataManager.h"

CUserDataManager::CUserDataManager(void)
{
}

CUserDataManager::~CUserDataManager(void)
{
}


void CUserDataManager::FreeDlgResource()
{
	//���ٴ���
	//if (MenuTest.GetSafeHwnd())
		//MenuTest.DestroyWindow();
}

void CUserDataManager::InitModuleFunc( LPCTSTR lpszCmdParam ,CHAR* pchSubParam , DWORD dwSubParamLen )
{

}

BOOL CUserDataManager::CreateCustomWindow( LPCTSTR lpszTransfModeID ,CWnd *pWnd)
{
	////���ڴ���
	//CMDIFrameWnd *pFrameWnd = (/*CBCGPMDIFrameWnd*/CMDIFrameWnd*)pWnd;
	//MenuTest.m_pDataManager = this;
	//MenuTest.Create(pFrameWnd);
	//MenuTest.SetBarStyle(MenuTest.GetBarStyle() | CBRS_SIZE_DYNAMIC);
	//MenuTest.EnableDocking(CBRS_ALIGN_ANY);
	//pFrameWnd->DockControlBar(&MenuTest);
	//
	//HICON l_hIcon;			//����ͼ��
	//CString l_strWndName;	//��������
	//CString l_strClassName;	//����������

	//l_strWndName.Format(_T("���˵�"));
	//l_strClassName.Format(_T("CMenuTest"));
	//if (lpszTransfModeID == _T("TEST1") )
	//{
	//	l_hIcon= LoadHicon(IDI_ICON1);
	//}
	//else
	//{
	//	l_hIcon= LoadHicon(IDI_ICON2);
	//}

	////������Ϣ�洢,�ز����ٵĴ���
	//SaveWindowInfo(&MenuTest, l_strClassName, l_strWndName, l_hIcon );
	return TRUE;
}

void CUserDataManager::OnReceiveDataFunc(DWORD dwInfoCode, CHAR* pData, DWORD dwDataLen, LPCTSTR lpszSrcID)
{
	//����Ա�ڴ���Ӵ��룬��������֡
	switch ( dwInfoCode )
	{
	case 1:
		break;
	default:
		break;
	}
}

void CUserDataManager::OnRevMsgFromPlat(  CHAR* pData, DWORD dwDataLen)
{
	//����Ա�ڴ���Ӵ��룬��������ƽ̨��Ϣ֪ͨ,�����ʽ��Define_PlatStruct.h

}
