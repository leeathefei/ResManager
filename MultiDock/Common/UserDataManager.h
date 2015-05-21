#pragma once
#include "datamanager.h"
//#include "MenuTest.h"

class CUserDataManager :	public CDataManager
{
public:
	CUserDataManager(void);
	virtual ~CUserDataManager(void);
public:
	//////////构件初始化函数///////////////
	//说明：构件实例初始化函数，开发人员可以在此函数中进行模块的预初始化准备
	//参数说明：
	//dwpDB:数据指针
	//lpszCmdParam:命令参数
	virtual void InitModuleFunc( LPCTSTR lpszCmdParam = NULL ,CHAR* pchSubParam = NULL, DWORD dwSubParamLen= 0 );

	// ///////////////创建窗口函数/////////////////
	//说明：在此函数中需要开发者编写相关窗口创建代码
	//参数说明：
	//lpszTransfModeID:创建模式，由构件能力而定。
	//返回值 ：FALSE-失败 TRUE-成功	
	virtual BOOL CreateCustomWindow( LPCTSTR lpszTransfModeID,CWnd *pWnd=NULL);

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////外部数据接收函数///////////////////////////////////////////\
	//说明:接收来自构件外部的业务数据，开发人员需在此函数对数据进行解析
	//参数说明:	
	//dwInfoCode:数据信息号
	//pData:接收数据指针; 
	//dwDataLen:接收的数据长度
	//lpszSrcID:数据来源
	virtual void OnReceiveDataFunc( DWORD dwInfoCode, CHAR* pData, DWORD dwDataLen, LPCTSTR lpszSrcID = NULL);

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////平台下发通知消息函数///////////////////////////////////////////
	//说明:接收来自平台通知，开发人员需在此函数对数据进行解析
	//参数说明:
	//pData:数据指针，具体格式见接口文件Define_PlatStruct.h
	//dwDataLen:数据长度; 

	virtual void OnRevMsgFromPlat(  CHAR* pData, DWORD dwDataLen );


	/////////////////////////////////////////////////////////////////////////////////
	////////////////////////资源释放函数///////////////////////////////////////////	
	//说明：在此函数中，需要开发者进行相关资源释放操作
	//返回值 ：FALSE-失败 TRUE-成功	
	virtual void FreeDlgResource();

public:

	// TODO:  在此添加需要自定义的窗口类指针及数据指针
 	//CMenuTest			MenuTest;

};
