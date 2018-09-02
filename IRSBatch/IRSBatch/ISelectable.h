#pragma once

class ISelectable
{
private:
  bool m_selected;

public:
  ISelectable() : m_selected(false){}
  virtual ~ISelectable(){}

  bool IsSelected() const {return m_selected;}
  void Select() { m_selected = true; }
  void Deselect() { m_selected = false; }
};