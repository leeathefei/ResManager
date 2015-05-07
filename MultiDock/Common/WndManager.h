#pragma once

#include "GeneralMacroDefine.h"
#include <map>

using namespace std;

typedef CWnd* (*PFUNC_CREATEOBJ)();
///*typedef PFUNC_CREATEOBJ LPFUNC_CREATEOBJ;*/
//
//class CWndManager
//{
//public:
//	CWndManager();
//	~CWndManager();
//
//	static void Register(LPCTSTR lpszClassName, PFUNC_CREATEOBJ);
//	static CWnd* CreateObj(LPCTSTR lpszClassName);
//	BOOL CheckIfParentCreated();
//protected:
//
//	static map<LPCTSTR, PFUNC_CREATEOBJ> m_mapRegisters;
//};

class COMMON_DLLEXPORT CBaseObj
{
public:
	virtual BOOL CreateWnd(CWnd* pParent)
	{
		return TRUE;
	}
};

class COMMON_DLLEXPORT CWndManager
{
public:
	static CWndManager* Instance();
	static void DestroyInstance();
	~CWndManager();

public:
	void Register(LPCTSTR lpszClassName, PFUNC_CREATEOBJ pFun);
	CWnd* Create(LPCTSTR lpszClassName);

protected:
	CWndManager();

	static CWndManager* m_pInstance;
	map<LPCTSTR, PFUNC_CREATEOBJ> m_mapClassName2Func;
};