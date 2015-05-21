#pragma once
//#include "StdAfx.h"
#pragma pack(push,1)   // n=1
#define Plat_ClassNameMaxLen 30
#define Plat_WindowdNameMaxLen 40
struct stWndInfo 
{
	char				chClassName[Plat_ClassNameMaxLen];			//窗口类标识
	char				chWindowName[Plat_WindowdNameMaxLen];			//窗口标题
	CWnd*				pWnd;					//窗口指针
	HICON				hIcon;					//窗口功能图标
	stWndInfo()
	{
		memset(chClassName,0,Plat_ClassNameMaxLen);
		memset(chWindowName,0,Plat_WindowdNameMaxLen);
		hIcon = NULL;
		pWnd = NULL;
	}
};
#pragma pack(pop)

typedef BOOL (*PROC_SENDDATA)(LPCTSTR lpszSrcID, DWORD dwInfoCode,  char* pBuf ,DWORD iBufLen, LPCTSTR lpszDstID );
typedef BOOL (*PROC_CREATEEXAMP)(LPCTSTR lpszSrcID, LPCTSTR lpszModuleTypeID, LPCTSTR lpszTransfModeID, CString& lpszAimExampID, char* pchSubParam , DWORD dwSubParamLen,CWnd *pWnd);
typedef BOOL (*PROC_CTRLEXAMP)(LPCTSTR lpszSrcID, BYTE byCtrlMode , LPCTSTR lpszAimExampID, LPCTSTR lpszFormID);
typedef BOOL (*PROC_APPLAYTOPLAT)(LPCTSTR lpszSrcID, char* pBuf ,DWORD dwBufLen );

extern UINT		PLAT_DLL_DATAREVMSG;	//数据接收消息
extern UINT		PLAT_DLL_WNDACTIVEMSG;	//窗口激活消息
extern UINT		PLAT_DLL_COMMANDMSG;	//菜单工具栏响应消息

class CDataManager;
class CDllManager
{
private:
	CDllManager(void);
	CDllManager(CDllManager const& org);
	CDllManager& operator=(CDllManager const& org);
	virtual ~CDllManager(void);

public:	
	//勿动 -- 否则会引起异常
	void		Sys_InitDllFunc(LPCTSTR lpszExampID, DWORD& pExampClass , LPCTSTR lpszCmdParam , char* pchSubParam=NULL, DWORD dwSubParamLen = 0);
	BOOL		Sys_CreateDllWindow(CList<stWndInfo*,stWndInfo*> &pList,  LPCTSTR lpszTransfModeID, DWORD dwpClass ,CWnd *pWnd= NULL);
	void		Sys_SendData(PROC_SENDDATA _Proc);  // 
	void		Sys_ApplyCreateExamp(PROC_CREATEEXAMP _Proc);
	void		Sys_ApplyControlExamp(PROC_CTRLEXAMP _Proc);
	void		Sys_ApplyFunction(PROC_APPLAYTOPLAT _Proc);
	BOOL	Sys_FreeDlgResource(DWORD dwExampClass) ;
	BOOL	Sys_ReceiveData( DWORD dwInfoCode, char* pBuf ,DWORD dwBufLen, DWORD dwExampClass,LPCTSTR lpszSrcID = NULL );
	BOOL	Sys_RevMsgFromPlat (  char* pBuf ,DWORD dwBufLen,DWORD dwExampClass );
public:
	static BOOL	SendDataToExtern( CDataManager* pDataManager,  DWORD dwInfoCode, char* pBuf ,DWORD dwBufLen , LPCTSTR lpszDstID );
	static BOOL	ApplyCreateExamp(  CDataManager* pDataManager, LPCTSTR lpszModuleTypeID, LPCTSTR lpszTransfModeID, CString& lpszAimExampID, char* pchSubParam=NULL, DWORD dwSubParamLen = 0,CWnd *pWnd = NULL);
	static BOOL	ApplyControlExamp(  CDataManager* pDataManager, BYTE byCtrlMode,  LPCTSTR lpszAimExampID , LPCTSTR lpszWndID = NULL);
	static BOOL	ApplyFunctionToPlat( CDataManager* pDataManager,  char* pBuf ,DWORD dwBufLen  );

};
