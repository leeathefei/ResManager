#pragma once

class CMessage;
class ImcListener
{
public:
   // ����ֵ�ĺ�������Ϣ����
   virtual LRESULT OnMessageReceived(CMessage* pMessage)=0;
};
