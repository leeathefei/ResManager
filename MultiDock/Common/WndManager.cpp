
#include "stdafx.h"
#include "WndManager.h"
#include "XmlConfig.h"

CWndManager* CWndManager::m_pInstance = NULL;

CWndManager::CWndManager()
{
	m_nViewIndex = 0;
}

CWndManager::~CWndManager()
{
}

CWndManager* CWndManager::Instance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CWndManager;
	}

	return m_pInstance;
}

void CWndManager::DestroyInstance()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void CWndManager::AddEventHandler(IObjCreatedEvent* pEvent)
{
	if (NULL != pEvent)
	{
		m_listHandlers.push_back(pEvent);
	}
}

void CWndManager::Register(CString lpszClassName, PFUNC_CREATEOBJ pFun) 
{
	m_mapClassName2Func.insert(map<CString, PFUNC_CREATEOBJ>::value_type(lpszClassName, pFun));
}

CWnd* CWndManager::CreateObj(CString strClass) 
{
	CWnd* pWnd = NULL;

	for (map<CString, PFUNC_CREATEOBJ>::iterator it = m_mapClassName2Func.begin();
		it != m_mapClassName2Func.end(); ++it)
	{
		CString strName =it->first;
		if (strName.CompareNoCase(strClass) == 0)
		{
			pWnd = ((*it).second)();
		}
	}

	if (NULL != pWnd)
	{
		AddCreatedWnd(pWnd,strClass);
	}

	return pWnd;
} 

 void CWndManager::ProcessEvent(CWnd*& pWnd, CString& strClass)
 {
 	for (list<IObjCreatedEvent*>::iterator it = m_listHandlers.begin();
 		it != m_listHandlers.end(); ++it)
 	{
 		IObjCreatedEvent* pEvent = (*it);
 		if (NULL != pEvent)
 		{
 			pEvent->OnObjectCreated(pWnd, strClass);
 		}
 	}
 }

UINT CWndManager::GetNextViewIndex()
{
	return m_nViewIndex++;
}

void CWndManager::CreateFloatWnd(CWnd* pParent, CString& strClass)
{
	//create panes.
	CWnd* pDlg = (CWnd*)CreateObj(strClass);
	CBaseObj*pBase = dynamic_cast<CBaseObj*>(pDlg);
	if (NULL != pBase)
	{
		pBase->CreateWnd((CWnd*)AfxGetMainWnd(), ALIGN_NON);//默认float的parent是mainframe！
		pDlg->ShowWindow(SW_SHOW);
	}
}

void CWndManager::CreateDockWnd(CWnd* pParent, CString& strClass, EPANE_ALIGNMENT etype)
{
	CWnd* pDlg = (CWnd*)CreateObj(strClass);
	CBaseObj*pBase = dynamic_cast<CBaseObj*>(pDlg);
	if (NULL != pBase)
	{
		pBase->CreateWnd(pParent, etype);//默认dock的parent是mainframe！
		pDlg->ShowWindow(SW_SHOW);
	}
}

void CWndManager::CreateChildWnd(CWnd* pParent, CString& strClass,CRect& rect)
{
	CWnd* pChildWnd = (CWnd*)CreateObj(strClass);
	CBaseObj* pBase = dynamic_cast<CBaseObj*>(pChildWnd);
	if (NULL != pBase)
	{
		pBase->CreateWnd(pParent, ALIGN_NON);
		pChildWnd->ShowWindow(SW_SHOW);
		
		//add child to its parent:for resize.
		CLayoutObj* pLayout = dynamic_cast<CLayoutObj*>(pParent);
		if (pLayout != NULL)
		{
			pLayout->AddChild(pChildWnd, rect);
		}
	}
}

void CWndManager::AddCreatedWnd(CWnd* pWnd, CString strClass)
{
	//cache
	CString strHinst;
	strHinst.Format(_T("0x%08x"), pWnd);
	MapWnd2Classname::iterator itFind = m_mapHins2Classname.find(strHinst);
	if (itFind == m_mapHins2Classname.end())
	{
		stWndInfoItem oneItem;
		oneItem.strClassName = strClass;
		oneItem.strHinstance = strHinst;
		oneItem.pWnd		 = pWnd;

		m_mapHins2Classname.insert(make_pair(strHinst, oneItem));
	}

	//notify to update parent window listctrl.
	ProcessEvent(pWnd, strClass);
}

BOOL CWndManager::GetCreatedWnd(MapWnd2Classname& mapAllCreated)
{
	mapAllCreated = m_mapHins2Classname;

	return mapAllCreated.size() > 0;
}

//////////////////////////////////////////////////////////////////////////
void CLayoutObj::AddChild(CWnd* pChildWnd, CRect& rect)
{
	if (NULL != pChildWnd)
	{
		m_mapChildWnds.insert(make_pair(pChildWnd, rect));
	}
}

BOOL CLayoutObj::GetChildWnds(map<CWnd*, CRect>& mapChilds)
{
	mapChilds = m_mapChildWnds;
	return mapChilds.size()>0;	
}