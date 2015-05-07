#pragma once
#include "MessageDefs.h"
#include "imc/ImcListener.h"
#include "imc/MessageReceiver.h"
#include "imc/MessageLoop.h"



#ifdef _COMMON_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

// CImc(CImcSender)类只有函数成员没有数据成员，因此适合作为函数局部对象来使用。
class DLL_EXPORT CImc
{
public:
   CImc();
   ~CImc();
   

   void SendView1ToPanel1(double dbValue);
   void SendView1ToPanel2(int nValue);
   void SendView1ToPanel3(UINT nComboSelection);
   void SendView1ToPanel4(double dbAtti,double dbLati, double dbTensity);
   void SendPanel1ToView1(double dbValue);

};

typedef CMessageReceiver   CImcReceiver;
typedef CImc               CImcSender;