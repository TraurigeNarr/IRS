#include "stdafx.h"

#include "MouseController.h"
#include "MouseCorePoint.h"
#include "MousePoints.h"
#include "SimpleMouse.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////

MouseController::MouseController()
  : m_current(nullptr)
{
  _InitMice();
}

MouseController::~MouseController()
{
  std::for_each(m_mice.begin(), m_mice.end(), [](IMouse* ip_mouse)
  {
    delete ip_mouse;
    ip_mouse = nullptr;
  });
  m_mice.clear();
}

void MouseController::_InitMice()
{
  m_mice.push_back(new SelectionMouse());
  m_mice.push_back(new MouseCorePoint());
  m_mice.push_back(new MousePoints());

  m_current = m_mice[0];
  m_current->OnActivate();
}

MouseController* MouseController::GetInstance()
{
  static MouseController instance;

  return &instance;
}

void MouseController::SetMouse(MouseType i_type)
{
  ASSERT(m_current != nullptr);

  if (m_current->GetType() == i_type)
    return;

  TMice::iterator it = std::find_if(m_mice.begin(), m_mice.end(), [i_type](IMouse* ip_mouse)
  {
    return ip_mouse->GetType() == i_type;
  });

  if (it != m_mice.end())
  {
    m_current->OnDeactivate();
    m_current = *it;
    m_current->OnActivate();
  }
}

void MouseController::MouseClicked(CView* ip_view, WPARAM wParam, LPARAM lParam)
{
  ASSERT(nullptr != m_current);
  m_current->MouseClicked(ip_view, wParam, lParam);
}

bool MouseController::SetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  ASSERT(nullptr != m_current);
  if (m_current->GetCursor() == nullptr)
    return false;
  ::SetCursor(m_current->GetCursor());
  return  true;
}

void MouseController::EnableActiveMouse()
{
  if (m_current->GetCursor() != nullptr)
    m_current->Enable();
}

void MouseController::DisableActiveMouse()
{
  if (m_current->GetCursor() != nullptr)
    m_current->Disable();
}