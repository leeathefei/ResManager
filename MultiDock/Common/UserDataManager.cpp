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
	//销毁窗口
	//if (MenuTest.GetSafeHwnd())
		//MenuTest.DestroyWindow();
}

void CUserDataManager::InitModuleFunc( LPCTSTR lpszCmdParam ,CHAR* pchSubParam , DWORD dwSubParamLen )
{

}

BOOL CUserDataManager::CreateCustomWindow( LPCTSTR lpszTransfModeID ,CWnd *pWnd)
{
	////窗口创建
	//CMDIFrameWnd *pFrameWnd = (/*CBCGPMDIFrameWnd*/CMDIFrameWnd*)pWnd;
	//MenuTest.m_pDataManager = this;
	//MenuTest.Create(pFrameWnd);
	//MenuTest.SetBarStyle(MenuTest.GetBarStyle() | CBRS_SIZE_DYNAMIC);
	//MenuTest.EnableDocking(CBRS_ALIGN_ANY);
	//pFrameWnd->DockControlBar(&MenuTest);
	//
	//HICON l_hIcon;			//窗口图标
	//CString l_strWndName;	//窗口名称
	//CString l_strClassName;	//窗口类名称

	//l_strWndName.Format(_T("主菜单"));
	//l_strClassName.Format(_T("CMenuTest"));
	//if (lpszTransfModeID == _T("TEST1") )
	//{
	//	l_hIcon= LoadHicon(IDI_ICON1);
	//}
	//else
	//{
	//	l_hIcon= LoadHicon(IDI_ICON2);
	//}

	////窗口信息存储,必不可少的代码
	//SaveWindowInfo(&MenuTest, l_strClassName, l_strWndName, l_hIcon );
	return TRUE;
}

void CUserDataManager::OnReceiveDataFunc(DWORD dwInfoCode, CHAR* pData, DWORD dwDataLen, LPCTSTR lpszSrcID)
{
	//程序员在此添加代码，解析数据帧
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
	//程序员在此添加代码，处理来自平台消息通知,具体格式见Define_PlatStruct.h

}
