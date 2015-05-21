#pragma once
//

#include "afxTempl.h"
#include "DllManager.h"

// TODO:  在此添加窗口类头文件
class CDataManager	
{
	friend class CDllManager;
public:
	CDataManager(void);
	~CDataManager(void);	
	
private:
	LPCTSTR							m_lpszExampID;		//实例标识
	CList<stWndInfo*,stWndInfo*>	m_WndList;			//窗口信息链表

private:


	virtual void InitModuleFunc( LPCTSTR lpszCmdParam = NULL ,CHAR* pchSubParam = NULL, DWORD dwSubParamLen= 0 ){}

	virtual BOOL CreateCustomWindow( LPCTSTR lpszTransfModeID )
	{
		return TRUE;
	}

	virtual void OnReceiveDataFunc(DWORD dwInfoCode,  CHAR* pData, unsigned long ulDataLen, LPCTSTR lpszSrcID = NULL){}

	virtual void OnRevMsgFromPlat(  CHAR* pBuf ,DWORD dwBufLen ){}
	
	virtual void FreeDlgResource()	{}


public:

	///////////////////////////////////////////////////////////////
	////////////////////窗口信息存储 /////////////
	//函数说明:在创建某一窗口后调用，进行窗口的信息存储
	//参数说明:
	//pWnd:要存储的窗口类指针
	//strClassName:窗口类名称
	//strWindowName:窗口的标题，可以为空
	//hIcon:窗口图标，可以为空
	void SaveWindowInfo( CWnd* pWnd, CString strClassName, CString strWindowName = _T(""), HICON hIcon = NULL );	

	///////////////////////////////////////////////////////////////
	//////	//创建实例函数 /////////////
	//函数说明:动态创建其他构件实例函数
	//参数说明:
	//lpszModuleTypeID :(要创建实例的类型标识,这个由系统统一分配)
	//lpszTransfModeID:创建模式
	//lpszAimExampID:创建成功后，产生的实例标识
	//pchSubParam:附加参数,用户自定义
	//dwSubParamLen:附加参数长度
	//返回值 : TRUE-成功	FALSE-失败
	BOOL CreateOtherExamp( LPCTSTR lpszModuleTypeID, LPCTSTR lpszTransfModeID, CString& lpszAimExampID, char* pchSubParam = NULL, DWORD dwSubParamLen= 0  );

	///////////////实例控制函数( 销毁 /显示 /隐藏等操作)////////////
	//说明:只需开发者直接调用即可,控制目标实例的销毁、显隐
	//参数说明:
	//byCtrlFlg:		控制标志 0-销毁 1-显示 2-隐藏
	//strAimExampID:	被控制目标的实例标识
	//lpszFormID:		被控制目标的实例窗口标识,不可单独销毁,如果不为空则对单个窗口操作,byCtrlFlg=0时无效
	//返回值:			0-失败 1-成功
	BOOL  ControlOtherExamp( BYTE byCtrlFlg, LPCTSTR lpszAimExampID , LPCTSTR lpszWndID = NULL ); 

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////向外部发送数据函数///////////////////////////////////////////
	//说明:只需开发者直接调用即可
	//参数说明

	//	dwInfoCode:数据标识功能码,由系统统一制定
	//	pData: 发送数据指针
	//	dwLength:发送数据长度
	//	lpszDestID: 指定数据发送的目的构件实例，可以为空

	//返回值 :FALSE-失败 TRUE-成功		
	BOOL  SendDataFunction(  DWORD dwInfoCode, CHAR *pData, DWORD  dwLength, LPCTSTR lpszDestID = NULL ); //发送数据函数

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////向平台申请功能操作函数///////////////////////////////////////////
	//说明:只需开发者直接调用即可
	//函数说明
	//pData: 申请格式数据指针,具体格式见接口文件
	//dwLength:申请格式数据长度
	//返回值 :FALSE-失败 TRUE-成功	
	BOOL ApplayFunctionToPlat( CHAR* pData, DWORD  dwLength );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////辅助功能函数/////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////获取图标////////////////////////////////////////////
	//说明:加载图片资源
	//参数说明:
	//uiImageID:图片资源
	//返回值: 图标
	static HICON LoadHicon( UINT uiImageID );

	////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////获取实例标识////////////////////////////////////////
	//说明:获取当前实例标识
	//参数说明:
	//返回值:实例标识
	LPCTSTR GetCurExampID() { return m_lpszExampID ;}
 };

