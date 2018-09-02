#include "Plane3D.h"

#include <math.h>

Plane3D::Plane3D(const Vector3D& i_norm, double i_distance)
  : m_norm(i_norm), m_distance(i_distance)
{
  Vector3D tmp(i_norm);
  m_distance = i_distance/tmp.Length();
  m_norm = tmp.Normalize();
}

Plane3D::Plane3D()
  : m_norm(), m_distance(0)
{

}

Vector3D Plane3D::GetNormal() const
{
  return m_norm;
}

double Plane3D::GetDistance() const
{
  return m_distance;
}

Vector3D Plane3D::GetProjection(const Vector3D& i_point) const
{
  Vector3D vec;

  vec = i_point - m_norm*(DotProduct(i_point,m_norm)+m_distance);

  return vec;
}

bool Plane3D::GetIntersectionPoint(Vector3D& o_intersection, const Vector3D& i_first_point, const Vector3D& i_second_point)
{
  Vector3D V (i_second_point - i_first_point);
  if(abs(V.DotProduct(m_norm)) < Vector3D::Precision())
    return false;

  double t = (-m_distance - m_norm.DotProduct(i_first_point))/V.DotProduct(m_norm);
  o_intersection = i_first_point + V*t;
  return true;
}