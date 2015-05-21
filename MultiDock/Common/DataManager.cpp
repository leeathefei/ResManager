#include "StdAfx.h"
#include "DataManager.h"
#include "DllManager.h"

CDataManager::CDataManager(void)
{
}

CDataManager::~CDataManager(void)
{
	while ( !m_WndList.IsEmpty() )
	{
		stWndInfo* pInfo = (stWndInfo*)m_WndList.GetHead();
		m_WndList.RemoveHead();
		delete pInfo;
	}
}
HICON CDataManager::LoadHicon( UINT uiImageID )
{
	HICON l_hicon = NULL;
	l_hicon = (HICON) ::LoadImage (
		AfxGetResourceHandle (),
		MAKEINTRESOURCE (uiImageID),
		IMAGE_ICON,
		::GetSystemMetrics (SM_CXSMICON),
		::GetSystemMetrics (SM_CYSMICON),
		LR_SHARED);
	return l_hicon;
}
//发送数据函数
BOOL  CDataManager::SendDataFunction(  DWORD dwInfoCode, CHAR *pBuf, DWORD  dwLength, LPCTSTR lpszDestID)
{
	return CDllManager::SendDataToExtern( this , dwInfoCode, pBuf, dwLength, lpszDestID );
}

//创建实例函数 
BOOL CDataManager::CreateOtherExamp( LPCTSTR lpszModuleTypeID, LPCTSTR lpszTransfModeID, CString& strAimExampID, CHAR* pchSubParam , DWORD dwSubParamLen )
{	
	return CDllManager::ApplyCreateExamp(this, lpszModuleTypeID, lpszTransfModeID, strAimExampID, pchSubParam,  dwSubParamLen);
}

//控制其他实例 销毁 /显示 /隐藏等操作
BOOL CDataManager::ControlOtherExamp( BYTE byCtrlFlg,  LPCTSTR lpszAimExampID,  LPCTSTR lpszWndID )
{
	return CDllManager::ApplyControlExamp(this, byCtrlFlg , lpszAimExampID, lpszWndID );
}

BOOL CDataManager::ApplayFunctionToPlat(CHAR *pData, DWORD  dwLength)
{
	return CDllManager::ApplyFunctionToPlat(this, pData , dwLength );
}

void CDataManager::SaveWindowInfo( CWnd* pWnd, CString strClassName, CString strWindowName, HICON hIcon )
{
	ASSERT( pWnd != NULL );
	ASSERT( strClassName.GetLength() > 0 );
	ASSERT( strClassName.GetLength() < Plat_ClassNameMaxLen );
	ASSERT( strWindowName.GetLength() < Plat_WindowdNameMaxLen );
	strClassName.LockBuffer();
	strWindowName.LockBuffer();

	stWndInfo* pInfo =new stWndInfo();
 	memcpy(pInfo->chClassName,strClassName,strClassName.GetLength());
	if (strWindowName.GetLength() > 0)
	{
		memcpy(pInfo->chWindowName,strWindowName,strWindowName.GetLength());
	}
	pInfo->hIcon = hIcon;
	pInfo->pWnd = pWnd;
	m_WndList.AddTail( pInfo );
}	

