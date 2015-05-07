#pragma once
#include "imc/Message.h"
#include <vector>
#include <map>
using namespace std;

enum IMC_MESSAGE
{
   IMC_MESSAGEID_BEGIN = WM_USER+5000,
   IMC_SAMPLE_VIEW1_PANE1 = IMC_MESSAGEID_BEGIN,
   IMC_SAMPLE_VIEW1_PANE2,
   IMC_SAMPLE_VIEW1_PANE3,
   IMC_SAMPLE_VIWE1_PANE4,
   IMC_SAMPLE_PANE1_VIEW1,
   

   // !!! NOTE !!!
   IMC_MESSAGEID_END,
};


class CMsgView1ToPanel1: public CMessage
{
protected:
	virtual ~CMsgView1ToPanel1(){};

public:
	CMsgView1ToPanel1(){ message = IMC_SAMPLE_VIEW1_PANE1;};

	double   dbValue;
	CString  strSendingModuleName;
};

class CMsgPanel1ToView1: public CMessage
{
protected:
	virtual ~CMsgPanel1ToView1(){};

public:
	CMsgPanel1ToView1(){ message = IMC_SAMPLE_PANE1_VIEW1;};

	double   dbValue;
	CString  strSendingModuleName;
};

class CMsgView1ToPanel2: public CMessage
{
protected:
	virtual ~CMsgView1ToPanel2(){};

public:
	CMsgView1ToPanel2(){ message = IMC_SAMPLE_VIEW1_PANE2;};

	int   radioValue;
	CString  strSendingModuleName;
};

class CMsgView1ToPane3: public CMessage
{
protected:
	virtual ~CMsgView1ToPane3(){};

public:
	CMsgView1ToPane3(){message = IMC_SAMPLE_VIEW1_PANE3;}

	UINT m_comboSelection;

};

class CMsgView1ToPane4: public CMessage
{
protected:
	virtual ~CMsgView1ToPane4(){};
public:
	CMsgView1ToPane4(){message = IMC_SAMPLE_VIWE1_PANE4;}

	double dbAtti;
	double dbLatti;
	double dbTensity;
};
