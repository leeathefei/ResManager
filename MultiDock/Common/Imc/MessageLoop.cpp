#include "stdafx.h"
#include "MessageLoop.h"
#include <afxmt.h>
#include <afxwin.h>
#include <iostream>
#include <process.h>
#include "..\MessageDefs.h"

using namespace std;
CCriticalSection CMessageLoop::m_csLock;
CMessageLoop* CMessageLoop::m_pInstance=NULL;

CMessageLoop* CMessageLoop::Instance()
{
   if( m_pInstance )
      return m_pInstance;

   m_csLock.Lock();
   if( m_pInstance==NULL )
   {
      m_pInstance = new CMessageLoop;
   }
   m_csLock.Unlock();

   return m_pInstance;
}

CMessageLoop* CMessageLoop::GetInstance()
{
   return m_pInstance;
}

void CMessageLoop::DestroyInstance()
{
   m_csLock.Lock();
   delete m_pInstance;
   m_pInstance = NULL;
   m_csLock.Unlock();
   return;
}


CMessageLoop::CMessageLoop()
{
   m_lpHanles[0] = m_hQuit = CreateEvent(NULL, FALSE, FALSE, NULL);
   m_lpHanles[1] = m_hNewMessageIncoming = CreateEvent(NULL, FALSE, FALSE, NULL);
   m_hThread = NULL;
   Run();
}

// When this function was called, we can sure the Run function was exited.
CMessageLoop::~CMessageLoop()
{
	try
	{
	   Quit(1000);
	   CloseHandle(m_hNewMessageIncoming);
	   m_hNewMessageIncoming=NULL;
	   CloseHandle(m_hQuit);
	   m_hQuit=NULL;
	   CloseHandle(m_hThread);
	   m_hThread=NULL;


	   while( !m_WorkQueue.empty() )
	   {
	      CMessage* pMessage = m_WorkQueue.front();
	      pMessage->DecRef();
	      m_WorkQueue.pop();
	   }

	   while( !m_IncomingQueue.empty() )
	   {
	      CMessage* pMessage = m_IncomingQueue.front();
	      pMessage->DecRef();
	      m_IncomingQueue.pop();
	   }
	}
	catch(...)
	{
		TRACE(_T("Exception thrown from ~CMessageLoop"));
	}
}


void CMessageLoop::RegisterReceiver(CMessageReceiver* pReceiver)
{
   if(m_pInstance==NULL)
   {
      ASSERT(FALSE);
      return;
   }

   CRWWriter locker(m_ReceiverListLock);

   for(size_t i=0; i<pReceiver->m_messageIdList.size(); ++i)
   {
      UINT message = pReceiver->m_messageIdList.at(i);
      CReceiverList& recvList = m_ReceiverList[message];
      CReceiverList::iterator it = find(recvList.begin(), recvList.end(), pReceiver);
      if(it==recvList.end())
         recvList.push_back(pReceiver);
   }
}


void CMessageLoop::UnregisterReceiver( CMessageReceiver* pReceiver )
{
   if(m_pInstance==NULL)
   {
      ASSERT(FALSE);
      return;
   }

   CRWWriter locker(m_ReceiverListLock);

   map<UINT, CReceiverList>::iterator it, tempIt;
   for(it=m_ReceiverList.begin(); it!=m_ReceiverList.end(); )
   {
      tempIt = it++;
      CReceiverList& recvList = tempIt->second;
      CReceiverList::iterator it2 = find( recvList.begin(), recvList.end(), pReceiver);
      if(it2!=recvList.end() )
      {
         recvList.erase(it2);
      }
   }
}


// PostMessage function will put the message into incoming queue.
// If the incoming queue is empty before, we also need to signal 
// Run thread that ...
void CMessageLoop::PostMessage(CMessage* pMessage)
{
   if(m_pInstance==NULL)
   {
      ASSERT(FALSE);
      return;
   }
  
   m_IncomingQueueLock.Lock();
   
   // Add reference 
   pMessage->AddRef();

   // 
   m_IncomingQueue.push(pMessage);

   // Notify thread that Incoming queue is not empty
   if(m_IncomingQueue.size()==1)
   {
      SetEvent(m_hNewMessageIncoming);
   }
   
   m_IncomingQueueLock.Unlock();
};

void CMessageLoop::PostQuitMessage()
{
   if(m_pInstance==NULL)
   {
      ASSERT(FALSE);
      return;
   }
   CMsg* pMessage = new CMsg(WM_QUIT);
   PostMessage(pMessage);
   pMessage->DecRef();
}

void CMessageLoop::Run()
{
   m_hThread = (HANDLE)_beginthreadex(NULL, 0, RunLoop, (LPVOID)this, 0, NULL);
}

void CMessageLoop::Quit(DWORD dwTimeout)
{
   QuitThread(dwTimeout);
}

//virtual 
void CMessageLoop::QuitThread(DWORD dwTimeout)
{
   SetEvent(m_hQuit);
   PostQuitMessage();
   if( WAIT_TIMEOUT == WaitForSingleObject(m_hThread, dwTimeout) )
   {
      TerminateThread(m_hThread,0);
      WaitForSingleObject(m_hThread, dwTimeout);
   }
}

//virtual 
void CMessageLoop::DispatchMessage(CMessage* pMessage)
{
   CRWReader locker(m_ReceiverListLock);
   map<UINT, CReceiverList>::const_iterator kiter = m_ReceiverList.find(pMessage->message);

   if(kiter!=m_ReceiverList.end())
   {
      const CReceiverList& recvList=kiter->second;
      for(size_t i=0; i<recvList.size(); ++i)
      { 
         recvList.at(i)->HandleMessage(pMessage); 
      }
   }
}


BOOL CMessageLoop::GetMessage(CMessage* &pMessage)
{
   // TODO: Could be optimized by replacing WaitFor with Interlocked
   if( WaitForSingleObject(m_hQuit,0)==WAIT_OBJECT_0 )
   {
      return FALSE;
   }

   if( m_WorkQueue.size()>0 )
   {
      pMessage = m_WorkQueue.front();
      m_WorkQueue.pop();
      return TRUE;
   }

   switch( WaitForMultipleObjects(2, m_lpHanles, FALSE, INFINITE) )
   {
   case WAIT_OBJECT_0+1:
      {
         ReloadWorkQueue();
         ASSERT(m_WorkQueue.size()>0);
         return GetMessage(pMessage);
      } 
   case WAIT_OBJECT_0:
      return FALSE;
   default:
      return FALSE;
   }
}

void CMessageLoop::ReloadWorkQueue()
{
   m_IncomingQueueLock.Lock();

   m_WorkQueue.swap(m_IncomingQueue);

   m_IncomingQueueLock.Unlock();
}

//static 
UINT CMessageLoop::RunLoop( LPVOID lpVoid)
{
   CMessageLoop* pThis = (CMessageLoop*) lpVoid;
   CMessage* pMessage;
   while( pThis->GetMessage(pMessage) )
   {
	   if( pMessage->message==WM_QUIT )
	   {
		   pMessage->DecRef();
		   break;
	   }

	   pThis->DispatchMessage(pMessage);
	   pMessage->DecRef();
   }
  
   return 0;
}
