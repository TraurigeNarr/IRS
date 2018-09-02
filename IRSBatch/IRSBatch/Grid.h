#pragma once

#include "IRenderable.h"
#include <MathBase\math\Vector3D.h>


class Grid : public IRenderable
{
private:
  size_t  m_divisions_number;
  float   m_scale_factor[2];
  POINT   m_size;
  float   m_minimum[2];
  float   m_maximum[2];

private:
  void _SetDefaults();

public:
  Grid ();
  virtual ~Grid();

  //set number of divisions on axis
  void SetDivisionsNumber(size_t i_number);
  //set scale factor
  void OnSize(size_t i_x, size_t i_y);
  
  void GetMaxMin_X_Axis(float& o_min, float& o_max) const;
  void GetMaxMin_Y_Axis(float& o_min, float& o_max) const;

  void SetMaxMin_X_Axis(float i_min, float i_max);
  void SetMaxMin_Y_Axis(float i_min, float i_max);

  //returns point in internal units
  //@param i_x - x position of mouse
  //@param i_y - y position of mouse
  Vector3D GetPoint(int i_x, int i_y) const;
  void TransformToCurrent(Vector3D& o_position) const;
  D2D1_POINT_2F TransformToRendererCoords(const Vector3D& i_position) const;
  //resets window; call each time after resizing
  void Reset();

  //overrides
  virtual void Draw(IRenderer* ip_renderer) const override;
  virtual Vector3D GetPosition() const;
};