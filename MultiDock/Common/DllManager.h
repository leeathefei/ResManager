#pragma once
//#include "StdAfx.h"
#pragma pack(push,1)   // n=1
#define Plat_ClassNameMaxLen 30
#define Plat_WindowdNameMaxLen 40
struct stWndInfo 
{
	char				chClassName[Plat_ClassNameMaxLen];			//�������ʶ
	char				chWindowName[Plat_WindowdNameMaxLen];			//���ڱ���
	CWnd*				pWnd;					//����ָ��
	HICON				hIcon;					//���ڹ���ͼ��
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

extern UINT		PLAT_DLL_DATAREVMSG;	//���ݽ�����Ϣ
extern UINT		PLAT_DLL_WNDACTIVEMSG;	//���ڼ�����Ϣ
extern UINT		PLAT_DLL_COMMANDMSG;	//�˵���������Ӧ��Ϣ

class CDataManager;
class CDllManager
{
private:
	CDllManager(void);
	CDllManager(CDllManager const& org);
	CDllManager& operator=(CDllManager const& org);
	virtual ~CDllManager(void);

public:	
	//�� -- ����������쳣
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
