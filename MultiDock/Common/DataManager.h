#pragma once
//

#include "afxTempl.h"
#include "DllManager.h"

// TODO:  �ڴ���Ӵ�����ͷ�ļ�
class CDataManager	
{
	friend class CDllManager;
public:
	CDataManager(void);
	~CDataManager(void);	
	
private:
	LPCTSTR							m_lpszExampID;		//ʵ����ʶ
	CList<stWndInfo*,stWndInfo*>	m_WndList;			//������Ϣ����

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
	////////////////////������Ϣ�洢 /////////////
	//����˵��:�ڴ���ĳһ���ں���ã����д��ڵ���Ϣ�洢
	//����˵��:
	//pWnd:Ҫ�洢�Ĵ�����ָ��
	//strClassName:����������
	//strWindowName:���ڵı��⣬����Ϊ��
	//hIcon:����ͼ�꣬����Ϊ��
	void SaveWindowInfo( CWnd* pWnd, CString strClassName, CString strWindowName = _T(""), HICON hIcon = NULL );	

	///////////////////////////////////////////////////////////////
	//////	//����ʵ������ /////////////
	//����˵��:��̬������������ʵ������
	//����˵��:
	//lpszModuleTypeID :(Ҫ����ʵ�������ͱ�ʶ,�����ϵͳͳһ����)
	//lpszTransfModeID:����ģʽ
	//lpszAimExampID:�����ɹ��󣬲�����ʵ����ʶ
	//pchSubParam:���Ӳ���,�û��Զ���
	//dwSubParamLen:���Ӳ�������
	//����ֵ : TRUE-�ɹ�	FALSE-ʧ��
	BOOL CreateOtherExamp( LPCTSTR lpszModuleTypeID, LPCTSTR lpszTransfModeID, CString& lpszAimExampID, char* pchSubParam = NULL, DWORD dwSubParamLen= 0  );

	///////////////ʵ�����ƺ���( ���� /��ʾ /���صȲ���)////////////
	//˵��:ֻ�迪����ֱ�ӵ��ü���,����Ŀ��ʵ�������١�����
	//����˵��:
	//byCtrlFlg:		���Ʊ�־ 0-���� 1-��ʾ 2-����
	//strAimExampID:	������Ŀ���ʵ����ʶ
	//lpszFormID:		������Ŀ���ʵ�����ڱ�ʶ,���ɵ�������,�����Ϊ����Ե������ڲ���,byCtrlFlg=0ʱ��Ч
	//����ֵ:			0-ʧ�� 1-�ɹ�
	BOOL  ControlOtherExamp( BYTE byCtrlFlg, LPCTSTR lpszAimExampID , LPCTSTR lpszWndID = NULL ); 

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////���ⲿ�������ݺ���///////////////////////////////////////////
	//˵��:ֻ�迪����ֱ�ӵ��ü���
	//����˵��

	//	dwInfoCode:���ݱ�ʶ������,��ϵͳͳһ�ƶ�
	//	pData: ��������ָ��
	//	dwLength:�������ݳ���
	//	lpszDestID: ָ�����ݷ��͵�Ŀ�Ĺ���ʵ��������Ϊ��

	//����ֵ :FALSE-ʧ�� TRUE-�ɹ�		
	BOOL  SendDataFunction(  DWORD dwInfoCode, CHAR *pData, DWORD  dwLength, LPCTSTR lpszDestID = NULL ); //�������ݺ���

	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////////��ƽ̨���빦�ܲ�������///////////////////////////////////////////
	//˵��:ֻ�迪����ֱ�ӵ��ü���
	//����˵��
	//pData: �����ʽ����ָ��,�����ʽ���ӿ��ļ�
	//dwLength:�����ʽ���ݳ���
	//����ֵ :FALSE-ʧ�� TRUE-�ɹ�	
	BOOL ApplayFunctionToPlat( CHAR* pData, DWORD  dwLength );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////�������ܺ���/////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////��ȡͼ��////////////////////////////////////////////
	//˵��:����ͼƬ��Դ
	//����˵��:
	//uiImageID:ͼƬ��Դ
	//����ֵ: ͼ��
	static HICON LoadHicon( UINT uiImageID );

	////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////��ȡʵ����ʶ////////////////////////////////////////
	//˵��:��ȡ��ǰʵ����ʶ
	//����˵��:
	//����ֵ:ʵ����ʶ
	LPCTSTR GetCurExampID() { return m_lpszExampID ;}
 };

