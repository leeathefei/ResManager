#pragma once
#include <afxwin.h>


#ifdef _COMMON_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

class DLL_EXPORT CMessage
{
public:
   UINT     message;

   CMessage():m_nRefCount(1)
   {
   };

   // The function returns the resulting incremented value.
   int  AddRef() 
   { 
      return InterlockedIncrement(&m_nRefCount); 
   }
   
   void DecRef() 
   {
      if(0==InterlockedDecrement(&m_nRefCount))
      {
         delete this;  //Îö¹¹»ùÀà
      }
   }

   LRESULT Post();


// Overrides
protected:
   virtual ~CMessage() {};


private:
   long  m_nRefCount;
};


class CMsg : public CMessage
{
public:
   CMsg(UINT messageId=0, WPARAM wp=0, LPARAM lp=0)
   {
      message = messageId;  
   }

protected:
   virtual ~CMsg(){};

private:
   WPARAM   wParam;
   LPARAM   lParam;
};


class CMessagePtr
{
public:
   CMessagePtr(CMessage* pMessage)
      :m_pMessage(pMessage)
   {
   }

   ~CMessagePtr()
   {
      m_pMessage->DecRef();
   }

   CMessage* m_pMessage;
};