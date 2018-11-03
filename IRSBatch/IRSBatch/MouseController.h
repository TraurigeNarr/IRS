#pragma once

#include "IMouse.h"

class MouseController
{
public:
  typedef std::vector<IMouse*> TMice;
private:
   TMice m_mice;
   IMouse* m_current;
private:
  MouseController();
  ~MouseController();

  MouseController(const MouseController& i_other);
  MouseController& operator=(const MouseController& i_other);

  void _InitMice();
public:
  static MouseController* GetInstance();


  void SetMouse(MouseType i_type);
  void MouseClicked(CView* ip_view, WPARAM wParam, LPARAM lParam);
  virtual void MouseDown(CView* ip_view, int x, int y);
  virtual void MouseUp(CView* ip_view, int x, int y);
  virtual void Move(CView* ip_view, int x, int y);
  bool SetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

  void EnableActiveMouse();
  void DisableActiveMouse();
};

#define MouseControllerInstance MouseController::GetInstance()