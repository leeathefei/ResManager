#include "stdafx.h"
#include "Imc.h"
#include "imc/MessageLoop.h"


CImc::CImc()
{

}

CImc::~CImc()
{

}

void CImc::SendView1ToPanel1(double dbValue)
{
	CMsgView1ToPanel1* pMessage = new CMsgView1ToPanel1;
	pMessage->dbValue = dbValue;
	pMessage->Post();
	pMessage->DecRef();
}

void CImc::SendView1ToPanel2(int nValue)
{
	CMsgView1ToPanel2* pMessage = new CMsgView1ToPanel2;
	pMessage->radioValue= nValue;
	pMessage->Post();
	pMessage->DecRef();
}

void CImc::SendView1ToPanel3(UINT nComboSelection)
{
	CMsgView1ToPane3* pMessage = new CMsgView1ToPane3;
	pMessage->m_comboSelection = nComboSelection;
	pMessage->Post();
	pMessage->DecRef();
}

void CImc::SendView1ToPanel4(double dbAtti,double dbLati, double dbTensity)
{
	CMsgView1ToPane4* pMessage = new CMsgView1ToPane4;
	pMessage->dbAtti = dbAtti;
	pMessage->dbLatti = dbLati;
	pMessage->dbTensity = dbTensity;
	pMessage->Post();
	pMessage->DecRef();
}

void CImc::SendPanel1ToView1(double dbValue)
{
	CMsgPanel1ToView1* pMessage = new CMsgPanel1ToView1;
	pMessage->dbValue = dbValue;
	pMessage->Post();
	pMessage->DecRef();
}
