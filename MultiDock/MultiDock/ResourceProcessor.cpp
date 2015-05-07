#include "stdafx.h"
#include "ResourceProcessor.h"
#include "..\Common\FileHelper.h"
#include "..\Common\XmlConfig.h"

CResourceProcssor::CResourceProcssor()
{
	Init();
}

CResourceProcssor::~CResourceProcssor()
{

}

BOOL CResourceProcssor::Init()
{
	CString strModulesXml = CFileHelper::GetModuleDir()+_T("\\Config\\dllResources.xml");
	CXmlConfig::Instance(strModulesXml);

	return TRUE;
}

UINT CResourceProcssor::GetDllCount()
{
	return 0;
}

CString CResourceProcssor::GetDllName(int index)
{
	return _T("");
}