#pragma once

#include "IMouse.h"

class SelectionMouse : public IMouse
{
public:
  SelectionMouse();
  ~SelectionMouse();

  virtual void MouseClicked(CView* ip_view, WPARAM wParam, LPARAM lParam) override;
  virtual void OnActivate() override;
  virtual void OnDeactivate() override;
};