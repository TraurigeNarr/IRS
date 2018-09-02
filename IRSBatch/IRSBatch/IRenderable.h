#pragma once

#include "ISelectable.h"

#include <d2d1.h>

class Renderer;
class IRenderer;

class IRenderable : public ISelectable
{
public:
  virtual ~IRenderable(){}

  virtual Vector3D GetPosition() const = 0;
  virtual void SetPosition(double i_x, double i_y, double i_z){}

  virtual void Draw(IRenderer* ip_renderer) const = 0;
};