#pragma once
#include <queue>
#include <map>
#include <list>
#include <afxmt.h>
#include "MessageQueue.hpp"
#include "MessageReceiver.h"
#include "..\RWLock.h"

#ifdef _COMMON_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

class CMessage;
class DLL_EXPORT CMessageLoop
{
public:
   
   static CMessageLoop* GetInstance();
   static void          DestroyInstance();
   static CMessageLoop* Instance();
   static CMessageLoop* m_pInstance;
   static CCriticalSection m_csLock;



   typedef CMessageQueue<CMessage*>  MessageQueueType;
   CMessageLoop();
   virtual ~CMessageLoop();


   void RegisterReceiver(CMessageReceiver* pReceiver);
   void UnregisterReceiver(CMessageReceiver* pReceiver);
   void PostMessage(CMessage* pMessage);
   void PostQuitMessage();
   void Run();
   void Quit(DWORD dwTimeout);

private:
   BOOL GetMessage(CMessage* &pMessage);
   void ReloadWorkQueue();
   UINT static __stdcall RunLoop(LPVOID lpVoid);


protected:
   virtual void QuitThread(DWORD dwTimeout=1000);
   virtual void DispatchMessage(CMessage* pMessage);
  
   HANDLE m_hThread;
   HANDLE m_hNewMessageIncoming;
   HANDLE m_hQuit;
   HANDLE m_lpHanles[2];

private:
   MessageQueueType  m_WorkQueue;
   MessageQueueType  m_IncomingQueue;
   CCriticalSection  m_IncomingQueueLock;
   std::map<UINT, CReceiverList> m_ReceiverList;
   CRWLock          m_ReceiverListLock;

};

#define MessageLoop  CMessageLoop::Instance

void GxPostMessage(CMessage* pMessage);
void GxSendMessage(CMessage* pMessage);