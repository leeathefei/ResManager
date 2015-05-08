#include "stdafx.h"
#include "XmlDataProc.h"
#include "..\Common\FileHelper.h"
#include "..\Common\XmlConfig.h"

CXmlDataProc* CXmlDataProc::m_pInstance = NULL;


CXmlDataProc::CXmlDataProc()
{
	Init();
}

CXmlDataProc::~CXmlDataProc()
{
}

CXmlDataProc* CXmlDataProc::Instance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CXmlDataProc;
	}

	return m_pInstance;
}

void CXmlDataProc::DestroyInstance()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


BOOL CXmlDataProc::Init()
{
	UINT nDllCount = AppXml()->GetAttributeInt(_T("DllCount"), 0);
	if (nDllCount > 0)
	{
		//read dll names.
		for (int i=0; i<nDllCount; i++)
		{
			CString strNode;
			strNode.Format(_T("Dll_%d\\DllName"), i);
			std::wstring strDllValue = AppXml()->GetAttributeText(strNode.GetString(), _T(""));
			if (!strDllValue.empty() && !IsDllAdded(CString(strDllValue.c_str())))
			{
				UINT nCount = m_mapDllName2Index.size();
				m_mapDllName2Index.insert(make_pair(strDllValue.c_str(), nCount));
			}
			
			//read child frame+view class name
			strNode.Format(_T("Dll_%d\\FrameWndClassName"), i);
			std::wstring strFramename = AppXml()->GetAttributeText(strNode.GetString(), _T(""));
			strNode.Format(_T("Dll_%d\\ViewClassName"), i);
			std::wstring strViewname  = AppXml()->GetAttributeText(strNode.GetString(), _T(""));
			if (!strFramename.empty() && !strViewname.empty())
			{
				AddFrameViewClassName(strDllValue.c_str(),strFramename.c_str(), strViewname.c_str());
			}

			//read class names.
			strNode.Format(_T("Dll_%d\\Panels\\GroupCount"), i);
			UINT nGroupCount = AppXml()->GetAttributeInt(strNode.GetString(), 0);
			if (nGroupCount>0)
			{
				for (int j=0; j<nGroupCount; j++)
				{
					CString strTypeNode;
					strTypeNode.Format(_T("Dll_%d\\Panels\\Group_%d\\PanelType"), i,j);
					std::wstring strType = AppXml()->GetAttributeText(strTypeNode, _T(""));
					CString strValue(strType.c_str());
					if (strValue.CompareNoCase(_T("FLOAT")) == 0)
					{
						ProcessFloatType(i,j,CString(strDllValue.c_str()));
					}
					else if (strValue.CompareNoCase(_T("DOCK")) == 0)
					{
						ProcessDockType(i,j,CString(strDllValue.c_str()));
					}
					else if (strValue.CompareNoCase(_T("CHILD")) == 0)
					{
						ProcessChildType(i,j,CString(strDllValue.c_str()));
					}

				}
			}
		}
	}

	return TRUE;
}

BOOL CXmlDataProc::GetDllNames(vector<CString>& vecDlls)
{
	vecDlls.clear();
	for (MapName2Index::iterator it = m_mapDllName2Index.begin();
		it != m_mapDllName2Index.end(); ++it)
	{
		vecDlls.push_back(it->first);
	}

	return vecDlls.size()>0;
}

BOOL CXmlDataProc::GetClassNames(CString& strDll, vector<CString>& vecClasses)
{
	vecClasses.clear();
	
	MapDll2ClassNames::iterator itFind = m_mapAllDllClassNames.find(strDll);
	if (itFind != m_mapAllDllClassNames.end())
	{
		MapName2Index& mapAllClasses = itFind->second;
		for (MapName2Index::iterator it = mapAllClasses.begin(); 
			it != mapAllClasses.end(); ++it)
		{
			vecClasses.push_back(it->first);
		}
	}
	
	return vecClasses.size() > 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL CXmlDataProc::IsClassNameAdded(CString & strDll, CString& strClassname)
{
	MapDll2ClassNames::iterator itFind = m_mapAllDllClassNames.find(strDll);
	if (itFind != m_mapAllDllClassNames.end())
	{
		MapName2Index& mapClasses = itFind->second;
		MapName2Index::iterator it = mapClasses.find(strClassname);
		if (it != mapClasses.end())
		{
			return TRUE;
		}
	}

	return FALSE;
}
BOOL CXmlDataProc::IsDllAdded(CString& strDllName)
{
	map<CString, UINT>::iterator itFind = m_mapDllName2Index.find(strDllName);
	return itFind != m_mapDllName2Index.end();
}

void CXmlDataProc::ProcessFloatType(UINT uDllIndex, UINT uGroupIndex, CString& strDllName)
{
	CString strFloatNode;
	strFloatNode.Format(_T("Dll_%d\\Panels\\Group_%d\\PanelCount"), uDllIndex, uGroupIndex);
	int nFloatPaneCount = AppXml()->GetAttributeInt(strFloatNode.GetString(), 0);
	for (int i=0; i<nFloatPaneCount; i++)
	{
		//pane's parent's name
		CString strParentNode;
		strParentNode.Format(_T("Dll_%d\\Panels\\Group_%d\\Pane_%d\\ParentWnd\\ClassName"), uDllIndex, uGroupIndex, i);
		std::wstring strParentName = AppXml()->GetAttributeText(strParentNode.GetString(), _T(""));
		if (!strParentName.empty() && !IsClassNameAdded(strDllName, CString(strParentName.c_str())))
		{
			int nCount = GetClassesCount(strDllName);
			AddClassName(strDllName, CString(strParentName.c_str()), nCount);
		}

		//pane's class name.
		CString strClassNode;
		strClassNode.Format(_T("Dll_%d\\Panels\\Group_%d\\Pane_%d\\ClassName"), uDllIndex, uGroupIndex, i);
		std::wstring strName = AppXml()->GetAttributeText(strClassNode.GetString(), _T(""));
		if (!strName.empty() && !IsClassNameAdded(strDllName, CString(strName.c_str())))
		{
			int nExistCount = GetClassesCount(strDllName);
			AddClassName(strDllName, CString(strName.c_str()), nExistCount);
		}
	}
}

void CXmlDataProc::ProcessDockType(UINT uDllIndex, UINT uGroupIndex, CString& strDllName)
{
	ProcessFloatType(uDllIndex, uGroupIndex, strDllName);
}

//may be iteration for many times.
void CXmlDataProc::ProcessChildType(UINT uDllIndex, UINT uGroupIndex, CString& strDllName)
{
	ProcessFloatType(uDllIndex, uGroupIndex, strDllName);

	//process position info.

}
void CXmlDataProc::AddClassName(CString& strDll, CString& strClassName, UINT nIndex)
{
	MapDll2ClassNames::iterator itFind = m_mapAllDllClassNames.find(strDll);
	if (itFind != m_mapAllDllClassNames.end())
	{
		MapName2Index& mapClasses = itFind->second;
		mapClasses.insert(make_pair(strClassName, nIndex));

		return;
	}

	//dll have not added yet.
	MapName2Index mapClasses;
	mapClasses.insert(make_pair(strClassName, nIndex));
	m_mapAllDllClassNames.insert(make_pair(strDll, mapClasses));
}

UINT CXmlDataProc::GetClassesCount(CString& strDll)
{
	MapDll2ClassNames::iterator itFind = m_mapAllDllClassNames.find(strDll);
	if (itFind != m_mapAllDllClassNames.end())
	{
		return itFind->second.size();
	}

	return 0;
}

void CXmlDataProc::AddFrameViewClassName(LPCTSTR strDll, LPCTSTR strFrame, LPCTSTR strView)
{
	MapDllFrameView::iterator itFind = m_mapDll2FrameView.find(strDll);
	if (itFind != m_mapDll2FrameView.end())
	{
		return;
	}

	stDllFrameView oneItem;
	oneItem.strFrameClassName = strFrame;
	oneItem.strViewClassName  = strView;

	m_mapDll2FrameView.insert(make_pair(strDll, oneItem));
}

BOOL CXmlDataProc::IsFrameViewClass(CString& strClassName)
{
	for (MapDllFrameView::iterator it = m_mapDll2FrameView.begin();
		it != m_mapDll2FrameView.end(); ++it)
	{
		stDllFrameView& oneItem = it->second;
		if (strClassName.CompareNoCase(oneItem.strFrameClassName) == 0||
			strClassName.CompareNoCase(oneItem.strViewClassName) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//We assumpt that frame+view class names are different in each dll project.
BOOL CXmlDataProc::IsFrameViewLoaded(CString& strClassName)
{
	for (MapDllFrameView::iterator it = m_mapDll2FrameView.begin();
			it != m_mapDll2FrameView.end(); ++it)
	{
		stDllFrameView& oneItem = it->second;
		if (strClassName.CompareNoCase(oneItem.strFrameClassName) == 0||
			strClassName.CompareNoCase(oneItem.strViewClassName) == 0)
		{
			return oneItem.bLoaded;
		}
	}

	return FALSE;
}

BOOL CXmlDataProc::SetFrameViewLoadFlag(CString& strClassName)
{
	for (MapDllFrameView::iterator it = m_mapDll2FrameView.begin();
			it != m_mapDll2FrameView.end(); ++it)
	{
		stDllFrameView& oneItem = it->second;
		if (strClassName.CompareNoCase(oneItem.strFrameClassName) == 0||
			strClassName.CompareNoCase(oneItem.strViewClassName) == 0)
		{
			oneItem.bLoaded = TRUE;
		}
	}

	return FALSE;
}