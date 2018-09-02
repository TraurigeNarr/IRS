#pragma once
#include "Vector3D.h"

class Plane3D
  {
  public:
              Plane3D(const Vector3D& i_norm, double i_distance);
              Plane3D();

    Vector3D  GetNormal() const;
    double    GetDistance() const;
    Vector3D  GetProjection(const Vector3D& i_point) const;
    bool      GetIntersectionPoint(Vector3D& o_intersection, const Vector3D& i_first_point, const Vector3D& i_second_point);
  private:
    Vector3D  m_norm;
    double    m_distance;
  };