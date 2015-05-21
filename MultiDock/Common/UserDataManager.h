#pragma once
#include "datamanager.h"
//#include "MenuTest.h"

class CUserDataManager :	public CDataManager
{
public:
	CUserDataManager(void);
	virtual ~CUserDataManager(void);
public:
	//////////������ʼ������///////////////
	//˵��������ʵ����ʼ��������������Ա�����ڴ˺����н���ģ���Ԥ��ʼ��׼��
	//����˵����
	//dwpDB:����ָ��
	//lpszCmdParam:�������
	virtual void InitModuleFunc( LPCTSTR lpszCmdParam = NULL ,CHAR* pchSubParam = NULL, DWORD dwSubParamLen= 0 );

	// ///////////////�������ں���/////////////////
	//˵�����ڴ˺�������Ҫ�����߱�д��ش��ڴ�������
	//����˵����
	//lpszTransfModeID:����ģʽ���ɹ�������������
	//����ֵ ��FALSE-ʧ�� TRUE-�ɹ�	
	virtual BOOL CreateCustomWindow( LPCTSTR lpszTransfModeID,CWnd *pWnd=NULL);

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////�ⲿ���ݽ��պ���///////////////////////////////////////////\
	//˵��:�������Թ����ⲿ��ҵ�����ݣ�������Ա���ڴ˺��������ݽ��н���
	//����˵��:	
	//dwInfoCode:������Ϣ��
	//pData:��������ָ��; 
	//dwDataLen:���յ����ݳ���
	//lpszSrcID:������Դ
	virtual void OnReceiveDataFunc( DWORD dwInfoCode, CHAR* pData, DWORD dwDataLen, LPCTSTR lpszSrcID = NULL);

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////ƽ̨�·�֪ͨ��Ϣ����///////////////////////////////////////////
	//˵��:��������ƽ̨֪ͨ��������Ա���ڴ˺��������ݽ��н���
	//����˵��:
	//pData:����ָ�룬�����ʽ���ӿ��ļ�Define_PlatStruct.h
	//dwDataLen:���ݳ���; 

	virtual void OnRevMsgFromPlat(  CHAR* pData, DWORD dwDataLen );


	/////////////////////////////////////////////////////////////////////////////////
	////////////////////////��Դ�ͷź���///////////////////////////////////////////	
	//˵�����ڴ˺����У���Ҫ�����߽��������Դ�ͷŲ���
	//����ֵ ��FALSE-ʧ�� TRUE-�ɹ�	
	virtual void FreeDlgResource();

public:

	// TODO:  �ڴ������Ҫ�Զ���Ĵ�����ָ�뼰����ָ��
 	//CMenuTest			MenuTest;

};
