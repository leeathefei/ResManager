/********************************************************************
	created:	2012/07/24
	created:	24:7:2012   16:20
	author:		WenJianguo

	purpose:	ƽ̨����빹��ʵ��֮�佻����Ϣ�ĸ�ʽ������
*********************************************************************/
#pragma once

//	ƽ̨��Ϣ���ܱ���
enum CETC54_PLAT_INNER_FUNCCODE_PTV2
{
	CETC54_PLAT_CLOSE	 =				0,		//ƽ̨�˳�			
	CETC54_PLAT_KeepCode =				1000,	// ����

	CETC54_PLAT_PTV2_SUBC =				1001,	// ������Ϣ����
	CETC54_PLAT_PTV2_DISSUBC =			1002,	// �˶���Ϣ����
	CETC54_PLAT_PTV2_LINKSTATUS =		1003,	// ��·״̬��Ϣ
	CETC54_PLAT_PTV2_UNCONNECT =		1004,	// �Ͽ���·����

	CETC54_PLAT_PTV2_BNTCOMMAND =		1005,	// �������˵���ť��Ӧ
	CETC54_PLAT_PTV2_INITBNT =			1006,	// ��ʼ���������˵���ť(�����������������)
	CETC54_PLAT_PTV2_RESETBNT =			1007,	// ���ù������˵���ť��ʾ(�����������������)
	CETC54_PLAT_PTV2_APYRESETBNT =		1008,	// ����ı乤�����˵���ť��ʾ(����ʹ��)
	CETC54_PLAT_PTV2_APYRESETSYSCAPTION =	1009,	// ����ı��������
	CETC54_PLAT_PTV2_DllIMPORTOK = 	1010,	//��̬��������֪ͨ(��֡��)

};

//	ȫ�ֵĳ�������
const USHORT C_HeadMarkLen	= 10;
const USHORT C_IDLen		= 10;
const USHORT C_IPLen		= 15;
const USHORT C_MSGLen		= 30;
const USHORT C_CapGLen		= 500;

#pragma pack(push,1)   // n=1

// ƽ̨��Ϣͷ
typedef struct CETC54_PLAT_INNER_HEAD
{
	char			Mark[C_HeadMarkLen];

	ULONG			Length;
	ULONG			FuncCode;

	CETC54_PLAT_INNER_HEAD(): Length(0), FuncCode(CETC54_PLAT_KeepCode)
	{
		CopyMemory(Mark, _T("PGCWJLCB01"), C_HeadMarkLen);
	}
	inline void InitPionter()
	{
		CopyMemory(Mark, _T("PGCWJLCB01"), C_HeadMarkLen);
		Length = 0;
		FuncCode = CETC54_PLAT_KeepCode;
	}
	inline unsigned long GetStructSize()
	{
		return sizeof(CETC54_PLAT_INNER_HEAD);
	}
}*PLAT_PHEAD;


// ������Ϣ
struct SUBC_CHILD
{
	ULONG			FuncCode;
	char			SendID[C_IDLen];

	SUBC_CHILD():FuncCode(0)
	{
		ZeroMemory(SendID, C_IDLen);
	}
	inline void InitPionter()
	{
		FuncCode = 0;
		ZeroMemory(SendID, C_IDLen);
	}
	inline unsigned long GetStructSize()
	{
		return sizeof(SUBC_CHILD);
	}
};
typedef struct CETC54_PLAT_SUBC
{
	char			RevID[C_IDLen];
	ULONG			SubCount;
	SUBC_CHILD		Info[1];

	CETC54_PLAT_SUBC():SubCount(0)
	{
		ZeroMemory(RevID, C_IDLen);
	}
	inline void InitPionter()
	{
		ZeroMemory(RevID, C_IDLen);
		SubCount = 0;
	}
	inline unsigned long GetStructSize()
	{
		return sizeof(RevID) + sizeof(SubCount) + SubCount * Info[0].GetStructSize();
	}
}*PLAT_PSUBC;

// �˶���Ϣ
typedef struct CETC54_PLAT_SUBC  CETC54_PLAT_DISSUBC;
typedef CETC54_PLAT_DISSUBC *PLAT_DISSUBC;

// ��·״̬��Ϣ
typedef struct CETC54_PLAT_LINKSTATUS
{
	char				IP[C_IPLen];
	ULONG				PortNum;
	ULONG				Status;
	char				LinkID[C_IDLen];

	CETC54_PLAT_LINKSTATUS():PortNum(0), Status(0)
	{
		ZeroMemory(IP, C_IPLen);
		ZeroMemory(LinkID, C_IDLen);
	}
	inline unsigned long GetStructSize()
	{
		return sizeof(CETC54_PLAT_LINKSTATUS);
	}
}*PLAT_PLINKSTATUS;

// �Ͽ���·����
typedef struct CETC54_PLAT_UNCONNECT
{
	char	LinkID[C_IDLen];

	CETC54_PLAT_UNCONNECT()
	{
		ZeroMemory(LinkID, C_IDLen);
	}
	inline unsigned long GetStructSize()
	{
		return sizeof(CETC54_PLAT_UNCONNECT);
	}
}*PLAT_PUNCONNECT;


// �������˵���ť��Ӧ��Ϣ
typedef struct CETC54_PLAT_BNTCOMMAND
{
	char	chMsgCode[C_MSGLen];		// ��Ӧ��ϢID

	CETC54_PLAT_BNTCOMMAND()
	{
		ZeroMemory(chMsgCode, C_MSGLen);
	}
	inline unsigned long GetStructSize()
	{
		return sizeof(CETC54_PLAT_BNTCOMMAND);
	}
}*PLAT_BNTCOMMAND;

// ��ť״̬: 0-������ 1-����δ���� 2-���ü���
typedef enum EBntStateType
{
	EBntStateType_unAble = 0,
	EBntStateType_unActive,
	EBntStateType_Active, 
	EBntStateType_HIDE,
}eType_EBntStateType;

// �������˵���ť��Ϣ
typedef struct CETC54_PLAT_BNTINFO
{
	char	chMsgCode[C_MSGLen];		//	��Ӧ��ϢID
	BYTE	byChangeBntState;			//	�Ƿ���İ�ť״̬ 0-������ 1-����
	BYTE	byBntState;					//	��ť״̬  
	BYTE	byChangeText;				//	�Ƿ���İ�ť���� 0-������ 1-����
	char	chText[20];					//	��ť����
	BYTE	byChangeImage;				//	�Ƿ���İ�ťͼƬ 0-������ 1-����
	short	shImage;					//	ͼƬ���
	BYTE	byChangeTip;				//	�Ƿ���İ�ť˵�� 0-������ 1-����
	char	chBntTip[100];				//	��ť˵��
	
	CETC54_PLAT_BNTINFO():byChangeBntState(0), byBntState(EBntStateType_unActive), byChangeText(0), 
		byChangeImage(0), shImage(-1), byChangeTip(0)
	{
		ZeroMemory(chMsgCode, C_MSGLen);
		ZeroMemory(chText, 20);
		ZeroMemory(chBntTip, 100);
	}
	inline unsigned long GetStructSize()
	{
		return sizeof(CETC54_PLAT_BNTINFO);
	}
}*PLAT_BNTINFO ;

// ����ı乤�����˵���ť��ʾ
typedef struct CETC54_PLAT_APYRESETBNT
{
	CETC54_PLAT_BNTINFO BntInfo;			
	ULONG	ulSubParamLen;				//	���Ӳ�������
	char*	pSubParamBuf;				//	���Ӳ���ָ��
	
	CETC54_PLAT_APYRESETBNT()
	{
		ulSubParamLen = 0;
		pSubParamBuf = NULL;
	}
	~ CETC54_PLAT_APYRESETBNT()
	{
		if (NULL != pSubParamBuf){delete[]pSubParamBuf; pSubParamBuf = NULL;}
	}
	inline unsigned long GetStructSize()
	{
		return BntInfo.GetStructSize() + sizeof(ulSubParamLen);
	}
}*PLAT_APYRESETBNT;


// ���ù������˵���ť��ʾ, ƽ̨�밴ť״̬������������ӿ�
typedef struct CETC54_PLAT_RESETBNT
{
	ULONG	ulBntCount;					//	��ť����
	CETC54_PLAT_BNTINFO BntInfo[1];		//	��ť��Ϣ��iBntCount*CETC54_PLAT_BNTCOMMANDINFO
	
	CETC54_PLAT_RESETBNT()
	{
		ulBntCount = 0;
	}
	inline unsigned long GetStructSize()
	{
		return sizeof(ulBntCount) + ulBntCount* BntInfo[0].GetStructSize();
	}
}*PLAT_RESETBNT;
//�ı��������
typedef struct CETC54_PLAT_RESETSYSCAPTION
{
	char	chCaption[C_CapGLen];			//	��������
	
	CETC54_PLAT_RESETSYSCAPTION()
	{
		ZeroMemory(chCaption,C_CapGLen);
	}
	inline unsigned long GetStructSize()
	{
		return C_CapGLen;
	}
}*PLAT_RESETSYSCAPTION;


#pragma pack(pop)