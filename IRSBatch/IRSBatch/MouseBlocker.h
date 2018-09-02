#pragma once

#include "MouseController.h"

class ActiveMouseBlocker
{
public:
  ActiveMouseBlocker()
  {
    MouseControllerInstance->DisableActiveMouse();
  }

  ~ActiveMouseBlocker()
  {
    MouseControllerInstance->EnableActiveMouse();
  }
};