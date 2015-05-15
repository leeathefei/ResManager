
/********************************************************************
	Created:	2015/05/8   11:02
	Filename:	XmlDataProc.h
	Author:		Chris Lee
	Functions:	process xml file and cache data.
*********************************************************************/

#pragma once
#include <map>
#include <list>

using namespace std;

struct stDllFrameView 
{
	BOOL	bLoaded;
	CString strFrameClassName;
	CString strViewClassName;
	stDllFrameView()
	{
		bLoaded = FALSE;
		strFrameClassName = _T("");
		strViewClassName  = _T("");
	}
};

struct stFloatWnd
{
	CString strClass;
	CRect rcWnd;
};

enum EDockType
{
	enmDockType_Left = 0,
	enmDockType_Right = 1,
	enmDockType_Top   = 2,
	enmDockType_Bottom = 3,
};
struct stDockWnd
{
	CString strClass;
	EDockType eDockType;
	CRect rcWnd;
};

struct stChildWnd
{
	CString strChildClass;
	CString strParentClass;
	CRect rcChild;
	CRect rcParent;
};

class CXmlDataProc
{
public:

	static CXmlDataProc* Instance();
	static void DestroyInstance();
	~CXmlDataProc();

	BOOL GetDllNames(vector<CString>&);
	BOOL GetClassNames(CString& strDll, vector<CString>&);

	BOOL IsFrameViewClass(CString& strClassName);
	BOOL IsFrameViewLoaded(CString& strClassname);
	BOOL SetFrameViewLoadFlag(CString& strClassname);

	//add new api
	BOOL Initialize();

protected:
	CXmlDataProc();
	static CXmlDataProc* m_pInstance;


protected:
	void AddFrameViewClassName(LPCTSTR strDll, LPCTSTR strFrame, LPCTSTR strView);
	void AddClassName(CString& strDll, CString& strClassName, UINT nIndex);
	BOOL IsDllAdded(CString& strDllName);
	BOOL IsClassNameAdded(CString& strDll, CString& strClassname);
	void ProcessFloatType(int nDllIndex);
	void ProcessDockType(int nDllIndex);
	void ProcessChildType(int nDllIndex);

	typedef map<CString, stDllFrameView> MapDllFrameView;
	MapDllFrameView	   m_mapDll2FrameView;

	typedef list<CString> ListClasses;
	typedef map<CString, ListClasses> MapDllname2Classes;
	MapDllname2Classes m_mapDll2Classnames;
	
	//workspace info
	list<stFloatWnd> m_listFloatWnds;
	list<stDockWnd>  m_listDockWnds;
	list<stChildWnd> m_listChildWnds;

protected:

};