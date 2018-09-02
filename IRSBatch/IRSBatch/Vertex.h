#pragma once

#include "IRenderable.h"

#include <MathBase\math\Vector3D.h>

class Renderer;
class Grid;

class Vertex 
  : public IRenderable
  , public Vector3D
{
  float                   m_scale_factor;
  size_t                  m_index;
  int                     m_color;

public:
  Vertex (const Vector3D& i_point, size_t i_index);
  ~Vertex ();

  //set scale factor
  void SetScale(float i_scale);
  void SetColor(int i_color);
  
  size_t GetIndex() const;
  void SetIndex(size_t i_index) { m_index = i_index;}
  //overrides
  virtual void Draw(IRenderer* ip_renderer) const override;
  virtual Vector3D GetPosition() const override;
  virtual void SetPosition(double i_x, double i_y, double i_z) override;
};