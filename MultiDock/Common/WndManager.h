

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
 	virtual void OnObjectCreated(/*CWnd* pWnd, CString& strClassName*/) = 0;
	virtual void OnWndClosed(/*CWnd* pWnd, CString& strClassName*/)		= 0;	
};

typedef CWnd* (*PFUNC_CREATEOBJ)();
class COMMON_DLLEXPORT CBaseObj
{
public:
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType, CString strWndName)	{	return TRUE;	}
};

struct stCreateWndItem
{
	CString strHinstance;	
	CString strClassName;
	CWnd*   pWnd;
	stCreateWndItem()
	{
		pWnd = NULL;
	}
};
typedef map<CString, stCreateWndItem> MapCreatedWnd;

struct stChildWnd
{
	CWnd* pChild;
	CRect rcChild;
	CString strChildWndName;
	stChildWnd()
	{
		pChild = NULL;
		strChildWndName = _T("");
	}
};
typedef list<stChildWnd> ListChildWnd;
typedef map<CWnd*, ListChildWnd> MapParent2ChildWnds;


class COMMON_DLLEXPORT CWndManager
{
public:
	static CWndManager* Instance();
	static void DestroyInstance();
	~CWndManager();

public:
	void AddEventHandler(IObjCreatedEvent* pEvent);
	void Register(CString lpszClassName, PFUNC_CREATEOBJ pFun);
	CWnd* CreateObj(CString lpszClassName, CString& strWndName);
	UINT GetNextViewIndex();

	//for different createing window page wndcofigdlg.h
	//Create pane+dialogs.
	void CreateFloatWnd(CWnd* pParent, CString& strClass, CString& strWndName);
	void CreateDockWnd(CWnd* pParent, CString& strClass, EPANE_ALIGNMENT etype, CString& strWndName);
	void CreateChildWnd(CWnd* pParent, CString& strClass, CRect& rect, CString&strWndName);
	CWnd* UpdateChildWndSizeAndName(CWnd* pSelChildWnd, CRect& rcNew, CString& strNewName);

	void AddCreatedWnd(CWnd* pWnd, CString strClass, CString& strWndName);
	BOOL GetCreatedWnd(MapCreatedWnd& mapAllCreated);
	BOOL GetChildWnds(CWnd* pParent, ListChildWnd& mapChilds);

	BOOL RemoveCreatedWnd(CWnd* pRemoved/*, CString strClassname*/);

protected:
	CWndManager();
	void NotifyWndCreated(/*CWnd*& pWnd, CString& strClass*/);
	void NotifyWndRemoved();
	void AddChild(CWnd* pParent, CWnd* pChildWnd, CRect& rect, CString& strChildName);

protected:
	UINT m_nViewIndex;	
	static CWndManager* m_pInstance;
	map<CString, PFUNC_CREATEOBJ> m_mapClassName2Func;
	list<IObjCreatedEvent*> m_listHandlers;
	
	MapCreatedWnd m_mapCreatedWnds;
	MapParent2ChildWnds m_mapParent2Childs;
};