#include "StdAfx.h"
#include "MessageReceiver.h"
#include "MessageLoop.h"
#include "ImcListener.h"
#include <algorithm>

CMessageReceiver::CMessageReceiver()
{
   //m_lpHandlingObject=NULL;
   //m_lpHandlingFunc=NULL;
   m_pListener = NULL;
}

CMessageReceiver::~CMessageReceiver()
{
   m_messageIdList.clear();
}

// void CMessageReceiver::HandleMessage(CMessage* pMessage)
// {
//    ASSERT(m_lpHandlingFunc);
//    ASSERT(m_lpHandlingObject);
//    if( m_lpHandlingObject && m_lpHandlingFunc )
//    {
//       m_lpHandlingFunc(m_lpHandlingObject, pMessage);
//       return;
//    }
// }

LRESULT CMessageReceiver::HandleMessage(CMessage* pMessage)
{
   ASSERT(m_pListener);
   if( m_pListener )
   {
      return m_pListener->OnMessageReceived(pMessage);
   }

   return -1;
}

void CMessageReceiver::Listen(UINT messageId)
{
   m_messageIdList.push_back(messageId);
}

void CMessageReceiver::CancelListen(UINT messageId)
{
   CMessageIDList::iterator it = std::find(m_messageIdList.begin(), m_messageIdList.end(), messageId);
   if( it!=m_messageIdList.end() )
      m_messageIdList.erase(it);
}

// void CMessageReceiver::Register(LPVOID lpObj, MESSAGE_HANDLER_FUNC lpFunc)
// {
//    m_lpHandlingObject = lpObj;
//    m_lpHandlingFunc   = lpFunc;
//    ASSERT(m_lpHandlingObject);
//    ASSERT(m_lpHandlingFunc);
//    CMessageLoop::Instance()->RegisterReceiver(this);
// }

void CMessageReceiver::Register(ImcListener* pListener)
{
   ASSERT(pListener);
   m_pListener = pListener;
   CMessageLoop* pInstance = CMessageLoop::Instance();
   ASSERT(pInstance);
   if(pInstance)
      pInstance->RegisterReceiver(this);
}

void CMessageReceiver::Unregister()
{
   CMessageLoop* pInstance = CMessageLoop::GetInstance();
   ASSERT(pInstance);
   if(pInstance)
      pInstance->UnregisterReceiver(this);
}

