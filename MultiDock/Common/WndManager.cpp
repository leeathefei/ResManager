
#include "stdafx.h"
#include "WndManager.h"
#include "XmlConfig.h"
#include "XmlDataProc.h"

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

CWnd* CWndManager::CreateObj(CString strClass, CString& strWndName, CString&strDll) 
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
		AddCreatedWnd(pWnd,strClass, strWndName,strDll);
	}

	return pWnd;
} 

 void CWndManager::NotifyWndCreated(/*CWnd*& pWnd, CString& strClass*/)
 {
 	for (list<IObjCreatedEvent*>::iterator it = m_listHandlers.begin();
 		it != m_listHandlers.end(); ++it)
 	{
 		IObjCreatedEvent* pEvent = (*it);
 		if (NULL != pEvent)
 		{
 			pEvent->OnObjectCreated(/*pWnd, strClass*/);
 		}
 	}
 }

 void CWndManager::NotifyWndRemoved()
 {
	 for (list<IObjCreatedEvent*>::iterator it = m_listHandlers.begin();
		 it != m_listHandlers.end(); ++it)
	 {
		 IObjCreatedEvent* pEvent = (*it);
		 if (NULL != pEvent)
		 {
			 pEvent->OnWndClosed();
		 }
	 }
 }

UINT CWndManager::GetNextViewIndex()
{
	return m_nViewIndex++;
}

void CWndManager::CreateFloatWnd(CWnd* pParent, CString& strClass, CString& strWndName, CString&strDll)
{
	//create panes.
	CWnd* pDlg = (CWnd*)CreateObj(strClass, strWndName, strDll);
	CBaseObj*pBase = dynamic_cast<CBaseObj*>(pDlg);
	if (NULL != pBase)
	{
		pBase->CreateWnd(pParent, ALIGN_FLOAT, strWndName);//默认float的parent是mainframe！
		pDlg->ShowWindow(SW_SHOW);
	}
}

void CWndManager::CreateDockWnd(CWnd* pParent, CString& strClass, EPANE_ALIGNMENT etype, CString& strWndName, CString&strDll)
{
	CWnd* pDlg = (CWnd*)CreateObj(strClass, strWndName,strDll);
	CBaseObj*pBase = dynamic_cast<CBaseObj*>(pDlg);
	if (NULL != pBase)
	{
		pBase->CreateWnd(pParent, etype, strWndName);//默认dock的parent是mainframe！
		pDlg->ShowWindow(SW_SHOW);
	}
}

CWnd* CWndManager::CreateChildWndEx(CString strParentClass, CString strChildClass, CRect rcChild, 
									CRect rcParent, CString strChildWndname, CString strDllname )
{
	CWnd* pParent = NULL;
	for(MapCreatedWnd::iterator it = m_mapCreatedWnds.begin(); it != m_mapCreatedWnds.end(); ++it)
	{
		stCreateWndItem& oneCreatedWnd = it->second;
		//先找到父窗口的实例是否存在。
		if (oneCreatedWnd.strDllname.CompareNoCase(strDllname) == 0 && 
			oneCreatedWnd.strClassName.CompareNoCase(strParentClass) == 0)
		{
			pParent = oneCreatedWnd.pWnd;
			break;
		}
	}

	//根据父窗口来创建子窗口。
	if (pParent != NULL)
	{
		CreateChildWnd(pParent, strChildClass, rcChild, CString(_T("")), strDllname, true);
	}

	return pParent;
}

void CWndManager::CreateChildWnd(CWnd* pParent, CString& strChildClass, CRect& rect,CString&strWndName,CString& strDll,bool bWithTitle)
{
	CWnd* pChildWnd = (CWnd*)CreateObj(strChildClass, strWndName,strDll);
	CBaseObj* pBase = dynamic_cast<CBaseObj*>(pChildWnd);
	if (NULL != pBase)
	{
		if (bWithTitle)
		{
			pBase->CreateWnd(pParent, ALIGN_CHILD_WITH_TITLE, strWndName);
			pChildWnd->SetWindowText(strWndName);
		}
		else
		{
			pBase->CreateWnd(pParent, ALIGN_CHILD_WITH_NO_TITLE, strWndName);
		}
		
		
		pChildWnd->ShowWindow(SW_SHOW);

		//add child to its parent:for resize.
		AddChild(pParent, pChildWnd, rect, strWndName, strChildClass);

		//Write to xml instancely.
		//RefreshChildGroup();
	}
}

void CWndManager::AddCreatedWnd(CWnd* pWnd, CString strClass, CString& strWndName, CString strDll)
{
	//cache
	CString strHinst;
	strHinst.Format(_T("0x%08x"), pWnd);
	CString strAlias;
	strAlias.Format(_T("%s(%s)"), strWndName, strHinst);
	MapCreatedWnd::iterator itFind = m_mapCreatedWnds.find(strHinst);
	if (itFind == m_mapCreatedWnds.end())
	{
		stCreateWndItem oneItem;
		oneItem.strClassName = strClass;
		oneItem.strHinstance = strAlias/*strHinst*/;
		oneItem.strDllname	 = strDll;
		oneItem.pWnd		 = pWnd;

		m_mapCreatedWnds.insert(make_pair(strHinst, oneItem));
	}

	//notify to update parent window listctrl.
	NotifyWndCreated(/*pWnd, strClass*/);
}

BOOL CWndManager::GetCreatedWnd(MapCreatedWnd& mapAllCreated)
{
	mapAllCreated = m_mapCreatedWnds;

	return mapAllCreated.size() > 0;
}

void CWndManager::AddChild(CWnd* pParent, CWnd* pChildWnd, CRect& rect, CString& strChildName, CString& strChildClass)
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
			oneItem.strChildClassname = strChildClass;
			listChilds.push_back(oneItem);
		}
		//add new parent wih its childs.
		else
		{
			stChildWnd oneChild;
			oneChild.pChild = pChildWnd;
			oneChild.rcChild = rect;
			oneChild.strChildWndName = strChildName;
			oneChild.strChildClassname = strChildClass;
			ListChildWnd listChilds;
			listChilds.push_back(oneChild);

			m_mapParent2Childs.insert(make_pair(pParent, listChilds));
		}
	}
}

void CWndManager::RefreshChildGroup()
{
	map<CString, UINT> mapDll2GroupCount;
	for (MapParent2ChildWnds::iterator itParent = m_mapParent2Childs.begin();
		itParent != m_mapParent2Childs.end(); ++itParent)
	{
		CWnd* pParent = itParent->first;
		CString strParent;
		strParent.Format(_T("0x%08x"), pParent);
		MapCreatedWnd::iterator it2 = m_mapCreatedWnds.find(strParent);
		if (it2 != m_mapCreatedWnds.end())
		{
			map<CString, UINT>::iterator it3 = mapDll2GroupCount.find(it2->second.strDllname);
			if (it3 != mapDll2GroupCount.end())
			{
				UINT& nValue = it3->second;
				nValue++;
			}
			else
			{
				mapDll2GroupCount.insert(make_pair(it2->second.strDllname, 1));
			}
		}

	}
	
	//
	MapParent2ChildWnds mapTempPert2Childs = m_mapParent2Childs;
	for(map<CString, UINT>::iterator iterIdx = mapDll2GroupCount.begin(); iterIdx != mapDll2GroupCount.end(); ++iterIdx)
	{
		CString strDllname = iterIdx->first;
		UINT nGroupCount = iterIdx->second;
		int nGroupIndex = 0;
		for (MapParent2ChildWnds::iterator iterP = mapTempPert2Childs.begin(); iterP != mapTempPert2Childs.end(); /*++iterP*/)
		{
			CWnd* pParent = iterP->first;
			CString strParent;
			strParent.Format(_T("0x%08x"), pParent);
			MapCreatedWnd::iterator itFind = m_mapCreatedWnds.find(strParent);
			if (itFind != m_mapCreatedWnds.end())
			{
				if (strDllname.CompareNoCase(itFind->second.strDllname) != 0)
				{
					++iterP;
					continue;
				}


				stCreateWndItem& oneParent = itFind->second;
				CString strParentClassname = oneParent.strClassName;
				UINT nDllIndex = CXmlDataProc::Instance()->GetDllIndex(oneParent.strDllname);
				CString strNode;
				strNode.Format(_T("Dll_%d\\CHILD_GROUP\\GroupCount"), nDllIndex);
				AppXml()->SetAttributeInt(strNode, nGroupCount);
				AppXml()->FlushData();


				//write parent name
				strNode.Format(_T("Dll_%d\\CHILD_GROUP\\Group_%d\\ParentWnd\\ClassName"), nDllIndex, nGroupIndex);
				AppXml()->SetAttribute(strNode, strParentClassname);
				AppXml()->FlushData();


				//write parent rect.
				CRect rcParent;
				pParent->GetWindowRect(&rcParent);
				strNode.Format(_T("Dll_%d\\CHILD_GROUP\\Group_%d\\ParentWnd\\left"), nDllIndex, nGroupIndex);
				AppXml()->SetAttributeInt(strNode, rcParent.left);
				strNode.Format(_T("Dll_%d\\CHILD_GROUP\\Group_%d\\ParentWnd\\top"), nDllIndex, nGroupIndex);
				AppXml()->SetAttributeInt(strNode, rcParent.top);
				strNode.Format(_T("Dll_%d\\CHILD_GROUP\\Group_%d\\ParentWnd\\right"), nDllIndex, nGroupIndex);
				AppXml()->SetAttributeInt(strNode, rcParent.right);
				strNode.Format(_T("Dll_%d\\CHILD_GROUP\\Group_%d\\ParentWnd\\bottom"), nDllIndex, nGroupIndex);
				AppXml()->SetAttributeInt(strNode, rcParent.bottom);
				AppXml()->FlushData();


				//write child info
				ListChildWnd& allChilds = iterP->second;
				strNode.Format(_T("Dll_%d\\CHILD_GROUP\\Group_%d\\ChildWnds\\ChildCount"), nDllIndex, nGroupIndex);
				AppXml()->SetAttributeInt(strNode, allChilds.size());
				AppXml()->FlushData();

				int nChildIndex=0;
				for (ListChildWnd::iterator itChild = allChilds.begin();
					itChild != allChilds.end(); ++itChild)
				{
					stChildWnd& oneChild = *itChild;

					//set child classname.
					strNode.Format(_T("Dll_%d\\CHILD_GROUP\\Group_%d\\ChildWnds\\Child_%d\\ClassName"), nDllIndex, nGroupIndex, nChildIndex);
					AppXml()->SetAttribute(strNode, oneChild.strChildClassname);

					//child rect.
					strNode.Format(_T("Dll_%d\\CHILD_GROUP\\Group_%d\\ChildWnds\\Child_%d\\left"), nDllIndex, nGroupIndex, nChildIndex);
					AppXml()->SetAttributeInt(strNode, oneChild.rcChild.left);

					strNode.Format(_T("Dll_%d\\CHILD_GROUP\\Group_%d\\ChildWnds\\Child_%d\\top"), nDllIndex, nGroupIndex, nChildIndex);
					AppXml()->SetAttributeInt(strNode, oneChild.rcChild.top);

					strNode.Format(_T("Dll_%d\\CHILD_GROUP\\Group_%d\\ChildWnds\\Child_%d\\right"), nDllIndex, nGroupIndex, nChildIndex);
					AppXml()->SetAttributeInt(strNode, oneChild.rcChild.right);

					strNode.Format(_T("Dll_%d\\CHILD_GROUP\\Group_%d\\ChildWnds\\Child_%d\\bottom"), nDllIndex, nGroupIndex, nChildIndex);
					AppXml()->SetAttributeInt(strNode, oneChild.rcChild.bottom);

					AppXml()->FlushData();
					nChildIndex++;
				}
				
				iterP = mapTempPert2Childs.erase(iterP);
			}

			nGroupIndex++;
			if (nGroupIndex>= nGroupCount)
			{
				break;
			}
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


//但是有个问题，对于用户自动调整父窗口的时候，那么cache中的值就是old的了。。
//父窗口，一开始的时候，就没有放值进来，那么修改的是父亲窗口，就会失败。
//所以只能修改child窗口。
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

BOOL CWndManager::RemoveCreatedWnd(CWnd* pRemoved)
{
	//1.read m_mapParent2Childs cache to get children of pRemove 
	//  and remove its child first in m_mapCreatedWnds cache.
	ListChildWnd listChilds;
	if (GetChildWnds(pRemoved, listChilds))
	{
		for (ListChildWnd::iterator itChild = listChilds.begin(); 
			itChild != listChilds.end(); ++itChild)
		{
			if (RemoveCreatedWnd(itChild->pChild))
			{

			}
		}
	}

	//2.remove it in m_mapCreatedWnds cache.
	BOOL bRemoved = FALSE;
	for(MapCreatedWnd::iterator it = m_mapCreatedWnds.begin(); 
		it != m_mapCreatedWnds.end(); ++it)
	{
		stCreateWndItem& CreatedWnd = it->second;
		if (pRemoved == CreatedWnd.pWnd )
		{
			//remove myself.
			if (pRemoved->GetSafeHwnd() != NULL)
			{
				pRemoved->DestroyWindow();
			}
			
			//delete pRemoved;
			m_mapCreatedWnds.erase(it);
			bRemoved = TRUE;
			break;
		}
	}

	//3. update m_mapParent2Childs cache!
	MapParent2ChildWnds::iterator itParent = m_mapParent2Childs.find(pRemoved);
	if ( itParent != m_mapParent2Childs.end())
	{
		m_mapParent2Childs.erase(itParent);
	}


	if (bRemoved)
	{
		NotifyWndRemoved();
	}

	return bRemoved;
}
