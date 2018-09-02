#pragma once
#include "CustomItem.h"

#include <MathBase/math/Vector3D.h>

class CPointItem : public ICustomItem
{
public:
  CPointItem(const Vector3D& i_position);
  ~CPointItem(void);

  virtual CPropertyGrid::EEditMode GetEditMode() override;
  virtual void DrawItem(CDC& dc, CRect rc, bool focused) override;
  virtual bool OnEditItem() override;

  Vector3D GetPosition() const { return m_position; }

protected:
  Vector3D m_position;
};
