#pragma once

enum MouseType
{
  MOUSE_NONE = -1,
  MOUSE_SIMPLE,
  MOUSE_CORE_POINT,
  MOUSE_POINTS,
};

class IMouse
{
private:
  MouseType m_type;

protected:
  HCURSOR m_cursor;
  bool m_active;

public:
  IMouse(MouseType i_type)
    : m_type(i_type)
    , m_cursor(nullptr)
    , m_active(true)
  {}
  virtual ~IMouse(){}

  virtual void MouseClicked(CView* ip_view, WPARAM wParam, LPARAM lParam) = 0;
  virtual void OnActivate() = 0;
  virtual void OnDeactivate() = 0;

  void Enable()
  {
    m_active = true;
  }

  void Disable()
  {
    m_active = false;
  }

  MouseType GetType() const
  {
    return m_type;
  }

  HCURSOR GetCursor() const
  {
    return m_cursor;
  }
};