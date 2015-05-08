

/********************************************************************
	Created:	2015/05/7   10:02
	Filename:	WndManager.h
	Author:		Chris Lee
	Functions:	Manage to dynamic create window
*********************************************************************/

#pragma once

#include "GeneralMacroDefine.h"
#include "ModuleDefs.h"
#include <map>
#include <list>

using namespace std;

class IObjCreatedEvent
{
public:
	virtual void OnCreateObject(CWnd* pWnd, CString& strClassName) = 0;
};


typedef CWnd* (*PFUNC_CREATEOBJ)();
class COMMON_DLLEXPORT CBaseObj
{
public:
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType)
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
	void AddEventHandler(IObjCreatedEvent* pEvent);
	void Register(CString lpszClassName, PFUNC_CREATEOBJ pFun);
	CWnd* CreateObj(CString lpszClassName);

protected:
	CWndManager();
	void ProcessEvent(CWnd*& pWnd, CString& strClass);

protected:
	static CWndManager* m_pInstance;
	map<CString, PFUNC_CREATEOBJ> m_mapClassName2Func;
	list<IObjCreatedEvent*> m_listHandlers;
};