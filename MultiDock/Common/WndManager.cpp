
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
		pBase->CreateWnd((CWnd*)AfxGetMainWnd(), ALIGN_VERTICAL/*ALIGN_FLOAT*/);//Ĭ��float��parent��mainframe��
		pDlg->ShowWindow(SW_SHOW);
	}
}

void CWndManager::CreateDockWnd(CWnd* pParent, CString& strClass, EPANE_ALIGNMENT etype)
{
	CWnd* pDlg = (CWnd*)CreateObj(strClass);
	CBaseObj*pBase = dynamic_cast<CBaseObj*>(pDlg);
	if (NULL != pBase)
	{
		pBase->CreateWnd(pParent, etype);//Ĭ��dock��parent��mainframe��
		pDlg->ShowWindow(SW_SHOW);
	}
}

void CWndManager::CreateChildWnd(CWnd* pParent, CString& strClass,CRect& rect,CString&strWndName)
{
	CWnd* pChildWnd = (CWnd*)CreateObj(strClass);
	CBaseObj* pBase = dynamic_cast<CBaseObj*>(pChildWnd);
	if (NULL != pBase)
	{
		pBase->CreateWnd(pParent, ALIGN_CHILD);
		pChildWnd->SetWindowText(strWndName);
		pChildWnd->ShowWindow(SW_SHOW);
		
		//add child to its parent:for resize.
		AddChild(pParent, pChildWnd, rect, strWndName);
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

void CWndManager::AddChild(CWnd* pParent, CWnd* pChildWnd, CRect& rect, CString& strChildName)
{
	if (NULL != pChildWnd && pParent != NULL)
	{
		MapParent2ChildWnds::iterator itFind = m_mapParent2Childs.find(pParent);
		if (itFind != m_mapParent2Childs.end())
		{
			ListChildWnd& listChilds = itFind->second;
			for (ListChildWnd::iterator it = listChilds.begin(); it != listChilds.end(); ++it)
			{
				stChildWnd& item = *it;
				if (item.pChild != NULL && item.pChild == pChildWnd)
				{
					return;
				}
			}

			//add new child item;
			stChildWnd oneItem;
			oneItem.pChild = pChildWnd;
			oneItem.rcChild = rect;
			oneItem.strChildWndName = strChildName;
			listChilds.push_back(oneItem);
		}
		//add new parent wih its childs.
		else
		{
			stChildWnd oneChild;
			oneChild.pChild = pChildWnd;
			oneChild.rcChild = rect;
			oneChild.strChildWndName = strChildName;
			ListChildWnd listChilds;
			listChilds.push_back(oneChild);

			m_mapParent2Childs.insert(make_pair(pParent, listChilds));
		}
	}
}

BOOL CWndManager::GetChildWnds(CWnd* pParent, ListChildWnd& mapChilds)
{
	MapParent2ChildWnds::iterator itFind = m_mapParent2Childs.find(pParent);
	if (itFind != m_mapParent2Childs.end())
	{
		mapChilds = itFind->second;
		return TRUE;
	}

	return FALSE;
}


//�����и����⣬�����û��Զ����������ڵ�ʱ����ôcache�е�ֵ����old���ˡ���
//�����ڣ�һ��ʼ��ʱ�򣬾�û�з�ֵ��������ô�޸ĵ��Ǹ��״��ڣ��ͻ�ʧ�ܡ�
//����ֻ���޸�child���ڡ�
CWnd* CWndManager::UpdateChildWndSizeAndName(CWnd* pSelChildWnd, CRect& rcNew, CString& strNewName)
{
	for(MapParent2ChildWnds::iterator it = m_mapParent2Childs.begin(); 
		it != m_mapParent2Childs.end(); ++it)
	{
		ListChildWnd& listChilds = it->second;
		for (ListChildWnd::iterator itChild = listChilds.begin();
			itChild != listChilds.end(); ++itChild)
		{
			stChildWnd& oneChild = *itChild;
			if (oneChild.pChild == pSelChildWnd)
			{
				oneChild.rcChild = rcNew;
				oneChild.strChildWndName = strNewName;
				return it->first;//return parent window to send resize message.
			}
		}
	}
	return NULL;

}