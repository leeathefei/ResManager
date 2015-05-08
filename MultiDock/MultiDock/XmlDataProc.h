
/********************************************************************
	Created:	2015/05/8   11:02
	Filename:	XmlDataProc.h
	Author:		Chris Lee
	Functions:	process xml file and cache data.
*********************************************************************/

#pragma once
#include <map>
#include <vector>

using namespace std;

struct stDllFrameView 
{
	CString strFrameClassName;
	CString strViewClassName;
};

class CXmlDataProc
{
public:

	static CXmlDataProc* Instance();
	static void DestroyInstance();
	~CXmlDataProc();

	BOOL Init();
	BOOL GetDllNames(vector<CString>&);
	BOOL GetClassNames(CString& strDll, vector<CString>&);

protected:
	CXmlDataProc();
	static CXmlDataProc* m_pInstance;


protected:
	UINT GetClassesCount(CString& strDll);
	void AddFrameViewClassName(LPCTSTR strDll, LPCTSTR strFrame, LPCTSTR strView);
	void AddClassName(CString& strDll, CString& strClassName, UINT nIndex);
	BOOL IsDllAdded(CString& strDllName);
	BOOL IsClassNameAdded(CString& strDll, CString& strClassname);
	void ProcessFloatType(UINT uDllIndex, UINT uGroupIndex, CString& strDllName);
	void ProcessDockType(UINT uDllIndex, UINT uGroupIndex, CString& strDllName);
	void ProcessChildType(UINT uDllIndex, UINT uGroupIndex, CString& strDllName);

	typedef map<CString, UINT> MapName2Index;
	typedef map<CString, MapName2Index> MapDll2ClassNames;
	typedef map<CString, stDllFrameView> MapDllFrameView;

	map<CString, UINT> m_mapDllName2Index;
	MapDll2ClassNames  m_mapAllDllClassNames;
	MapDllFrameView	   m_mapDll2FrameView;


protected:

};