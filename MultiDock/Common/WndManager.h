

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
 		virtual void OnObjectCreated(CWnd* pWnd, CString& strClassName) = 0;
};

typedef CWnd* (*PFUNC_CREATEOBJ)();
class COMMON_DLLEXPORT CBaseObj
{
public:
	virtual BOOL CreateWnd(CWnd* pParent, EPANE_ALIGNMENT eDockType)	{	return TRUE;	}
};


//After each child window added to parent window,call parent's 'AdjustLayout' to reposition child.
//so all parent container window must inherit CLayoutObj::AdjustLayout();
class COMMON_DLLEXPORT CLayoutObj
{
public:
	CLayoutObj()
	{
		m_mapChildWnds.insert(make_pair((CWnd*)NULL, CRect(0,0,200,300)));
	}
	~CLayoutObj()
	{
		m_mapChildWnds.clear();
	}
	virtual void AddChild(CWnd* pChildWnd, CRect& rect);
	virtual BOOL GetChildWnds(map<CWnd*, CRect>& mapChilds);

public:
	virtual void AdjustLayout(){ }

protected:
	map<CWnd*, CRect> m_mapChildWnds;
};
struct stWndInfoItem
{
	CString strHinstance;	
	CString strClassName;
	CWnd*   pWnd;
	stWndInfoItem()
	{
		pWnd = NULL;
	}
};
typedef map<CString, stWndInfoItem> MapWnd2Classname;


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
	UINT GetNextViewIndex();

	//for different createing window page wndcofigdlg.h
	//Create pane+dialogs.
	void CreateFloatWnd(CWnd* pParent, CString& strClass);
	void CreateDockWnd(CWnd* pParent, CString& strClass, EPANE_ALIGNMENT etype);
	void CreateChildWnd(CWnd* pParent, CString& strClass, CRect& rect);

	void AddCreatedWnd(CWnd* pWnd, CString strClass);
	BOOL GetCreatedWnd(MapWnd2Classname& mapAllCreated);

protected:
	CWndManager();
	void ProcessEvent(CWnd*& pWnd, CString& strClass);

protected:
	UINT m_nViewIndex;	
	static CWndManager* m_pInstance;
	map<CString, PFUNC_CREATEOBJ> m_mapClassName2Func;
	list<IObjCreatedEvent*> m_listHandlers;
	
	MapWnd2Classname m_mapHins2Classname;
};