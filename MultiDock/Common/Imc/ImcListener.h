#pragma once

class CMessage;
class ImcListener
{
public:
   // 返回值的含义由消息决定
   virtual LRESULT OnMessageReceived(CMessage* pMessage)=0;
};
