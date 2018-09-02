#pragma once

#include "IMouse.h"

class MousePoints : public IMouse
{
public:
  MousePoints();
  ~MousePoints();

  virtual void MouseClicked(CView* ip_view, WPARAM wParam, LPARAM lParam) override;
  virtual void OnActivate() override;
  virtual void OnDeactivate() override;
};