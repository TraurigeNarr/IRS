#pragma once

#include "IMouse.h"

class MouseCorePoint : public IMouse
{
public:
  MouseCorePoint();
  ~MouseCorePoint();

  virtual void MouseClicked(CView* ip_view, WPARAM wParam, LPARAM lParam) override;
  virtual void OnActivate() override;
  virtual void OnDeactivate() override;
};