#pragma once

class CResourceProcssor
{
public:
	CResourceProcssor();
	~CResourceProcssor();

	BOOL Init();
	
	CString GetDllName(int index);
	UINT GetDllCount();


protected:

};