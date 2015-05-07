#pragma once
#include "Message.h"
#include <vector>


#ifdef _COMMON_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

class ImcListener;
class CMessageLoop;
class DLL_EXPORT CMessageReceiver
{
public:
   typedef void (*MESSAGE_HANDLE_FUNC)(LPVOID lpHandlingClass, CMessage* pMessage);
   typedef MESSAGE_HANDLE_FUNC MESSAGE_HANDLER_FUNC;
   typedef std::vector<UINT> CMessageIDList;
   friend class CMessageLoop;
   CMessageReceiver();
   ~CMessageReceiver();
   LRESULT HandleMessage(CMessage* pMessage);
   void Listen(UINT messageId);
   void CancelListen(UINT messageId);
   void Register(ImcListener*);
   void Unregister();

private:
   //LPVOID  m_lpHandlingObject;
   //MESSAGE_HANDLER_FUNC  m_lpHandlingFunc;
   ImcListener*  m_pListener;
   CMessageIDList m_messageIdList;
};

typedef std::vector<CMessageReceiver*> CReceiverList;

