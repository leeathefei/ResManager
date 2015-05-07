#pragma once
#include "stdafx.h"
#include "Message.h"
#include "MessageLoop.h"


LRESULT CMessage::Post()
{
   CMessageLoop::Instance()->PostMessage(this);
   return 0;
}
