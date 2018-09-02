#pragma once
#include "PropertyGrid.h"

class ICustomItem
{
public:
  virtual ~ICustomItem(){}
  // basic required stuff
  virtual CPropertyGrid::EEditMode GetEditMode() = 0;
  virtual void DrawItem(CDC& dc, CRect rc, bool focused) = 0;

  // validation
  virtual void ValidateChanges() {}

  // mouse stuff
  virtual bool OnLButtonDown(CRect rc, CPoint pt) { return false; }
  virtual void OnMouseMove(CRect rc, CPoint pt) {}
  virtual void OnLButtonUp(CRect rc, CPoint pt) {}

  // in-place edit
  virtual std::wstring GetStringForInPlaceEdit() { return L""; }
  virtual bool OnItemEdited(std::wstring strNewValue) { return false; }

  // dropdown edit
  virtual void ShowDropDown(CRect rc) {}

  // modal edit
  virtual bool OnEditItem() { return false; }

protected:
  friend class CPropertyGrid;
  CPropertyGrid* m_pGrid;
};
