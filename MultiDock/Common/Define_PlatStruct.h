/********************************************************************
	created:	2012/07/24
	created:	24:7:2012   16:20
	author:		WenJianguo

	purpose:	平台框架与构件实例之间交换信息的格式与内容
*********************************************************************/
#pragma once

//	平台信息功能编码
enum CETC54_PLAT_INNER_FUNCCODE_PTV2
{
	CETC54_PLAT_CLOSE	 =				0,		//平台退出			
	CETC54_PLAT_KeepCode =				1000,	// 保留

	CETC54_PLAT_PTV2_SUBC =				1001,	// 订阅信息请求
	CETC54_PLAT_PTV2_DISSUBC =			1002,	// 退订信息请求
	CETC54_PLAT_PTV2_LINKSTATUS =		1003,	// 链路状态信息
	CETC54_PLAT_PTV2_UNCONNECT =		1004,	// 断开链路请求

	CETC54_PLAT_PTV2_BNTCOMMAND =		1005,	// 工具栏菜单按钮响应
	CETC54_PLAT_PTV2_INITBNT =			1006,	// 初始化工具栏菜单按钮(工具条管理组件接收)
	CETC54_PLAT_PTV2_RESETBNT =			1007,	// 设置工具栏菜单按钮显示(工具条管理组件发送)
	CETC54_PLAT_PTV2_APYRESETBNT =		1008,	// 申请改变工具栏菜单按钮显示(构件使用)
	CETC54_PLAT_PTV2_APYRESETSYSCAPTION =	1009,	// 申请改变软件标题
	CETC54_PLAT_PTV2_DllIMPORTOK = 	1010,	//动态库加载完成通知(无帧体)

};

//	全局的常量定义
const USHORT C_HeadMarkLen	= 10;
const USHORT C_IDLen		= 10;
const USHORT C_IPLen		= 15;
const USHORT C_MSGLen		= 30;
const USHORT C_CapGLen		= 500;

#pragma pack(push,1)   // n=1

// 平台信息头
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


// 订阅信息
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

// 退订消息
typedef struct CETC54_PLAT_SUBC  CETC54_PLAT_DISSUBC;
typedef CETC54_PLAT_DISSUBC *PLAT_DISSUBC;

// 链路状态信息
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

// 断开链路请求
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


// 工具栏菜单按钮响应消息
typedef struct CETC54_PLAT_BNTCOMMAND
{
	char	chMsgCode[C_MSGLen];		// 响应消息ID

	CETC54_PLAT_BNTCOMMAND()
	{
		ZeroMemory(chMsgCode, C_MSGLen);
	}
	inline unsigned long GetStructSize()
	{
		return sizeof(CETC54_PLAT_BNTCOMMAND);
	}
}*PLAT_BNTCOMMAND;

// 按钮状态: 0-不可用 1-可用未激活 2-可用激活
typedef enum EBntStateType
{
	EBntStateType_unAble = 0,
	EBntStateType_unActive,
	EBntStateType_Active, 
	EBntStateType_HIDE,
}eType_EBntStateType;

// 工具栏菜单按钮信息
typedef struct CETC54_PLAT_BNTINFO
{
	char	chMsgCode[C_MSGLen];		//	响应消息ID
	BYTE	byChangeBntState;			//	是否更改按钮状态 0-不更改 1-更改
	BYTE	byBntState;					//	按钮状态  
	BYTE	byChangeText;				//	是否更改按钮名称 0-不更改 1-更改
	char	chText[20];					//	按钮名称
	BYTE	byChangeImage;				//	是否更改按钮图片 0-不更改 1-更改
	short	shImage;					//	图片序号
	BYTE	byChangeTip;				//	是否更改按钮说明 0-不更改 1-更改
	char	chBntTip[100];				//	按钮说明
	
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

// 申请改变工具栏菜单按钮显示
typedef struct CETC54_PLAT_APYRESETBNT
{
	CETC54_PLAT_BNTINFO BntInfo;			
	ULONG	ulSubParamLen;				//	附加参数长度
	char*	pSubParamBuf;				//	附加参数指针
	
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


// 设置工具栏菜单按钮显示, 平台与按钮状态管理组件交互接口
typedef struct CETC54_PLAT_RESETBNT
{
	ULONG	ulBntCount;					//	按钮数量
	CETC54_PLAT_BNTINFO BntInfo[1];		//	按钮信息，iBntCount*CETC54_PLAT_BNTCOMMANDINFO
	
	CETC54_PLAT_RESETBNT()
	{
		ulBntCount = 0;
	}
	inline unsigned long GetStructSize()
	{
		return sizeof(ulBntCount) + ulBntCount* BntInfo[0].GetStructSize();
	}
}*PLAT_RESETBNT;
//改变软件标题
typedef struct CETC54_PLAT_RESETSYSCAPTION
{
	char	chCaption[C_CapGLen];			//	标题名称
	
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