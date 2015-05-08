
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

	//write to xml
	
	//notify to read xml and update parent window tree.
	ProcessEvent(pWnd, strClass);

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