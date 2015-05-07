
#include "stdafx.h"
#include "WndManager.h"

//map<LPCTSTR, PFUNC_CREATEOBJ> CWndManager::m_mapRegisters;
//
////map<const char*, Base::ClassGen> Base::class_set; // 静态成员变量定义
//
//
//CWndManager::CWndManager()
//{
//	//Register(_T("CDlgTest1"), &CDlgTest1::CreateDlgObj);
//}
//
//CWndManager::~CWndManager()
//{
//
//}
//
//void CWndManager::Register(LPCTSTR lpszClassName, PFUNC_CREATEOBJ pFunc)
//{
//	m_mapRegisters.insert(map<LPCTSTR, PFUNC_CREATEOBJ>::value_type(lpszClassName, pFunc));
//	//m_mapRegisters.insert(make_pair(lpszClassName, pFunc));
//}
//
//
//CWnd* CWndManager::CreateObj(LPCTSTR lpszClassName)
//{
//	map<LPCTSTR, PFUNC_CREATEOBJ>::iterator itFind = m_mapRegisters.find(lpszClassName);
//	if (itFind != m_mapRegisters.end())
//	{
//		return (itFind->second)();
//	}
//}



// 不得已，定义一个全局变量来完成这项光荣的任务，看起来有点龌龊

//int main()
//{
//	Base* pDerived = Base::Create("Derived"); // 类名可以动态输入
//	if(pDerived) pDerived->Print(); // 创建成功调用虚函数
//	else cout << "Create error" << endl;
//	system("pause");
//	return 0;
//}

CWndManager* CWndManager::m_pInstance = NULL;

CWndManager::CWndManager()
{
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

void CWndManager::Register(CString lpszClassName, PFUNC_CREATEOBJ pFun) 
{
	m_mapClassName2Func.insert(map<CString, PFUNC_CREATEOBJ>::value_type(lpszClassName, pFun));
}

CWnd* CWndManager::Create(CString lpszClassName) 
{
	/*map<CString, PFUNC_CREATEOBJ>::iterator iter;
	if((iter = m_mapClassName2Func.find(lpszClassName)) != m_mapClassName2Func.end())
	{
		return ((*iter).second)();
	}*/
	for (map<CString, PFUNC_CREATEOBJ>::iterator it = m_mapClassName2Func.begin();
		it != m_mapClassName2Func.end(); ++it)
	{
		CString strName =it->first;
		if (strName.CompareNoCase(lpszClassName) == 0)
		{
			return ((*it).second)();
		}
	}
	return NULL;
} 
