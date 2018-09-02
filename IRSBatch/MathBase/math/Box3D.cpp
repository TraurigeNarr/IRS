#include "Box3D.h"

#include <math.h>

Box3D::Box3D()
  {

  }

Box3D::Box3D(const Vector3D& i_min, const Vector3D& i_max)
  : m_min(i_min), m_max(i_max)
{

}

bool Box3D::Contains(const Vector3D& i_point) const
{

  for(int i = 0; i < 3; ++i)
  {
    if(i_point[i] < m_min[i] || i_point[i] > m_max[i])
      return false;
  }

  return true;
}

bool Box3D::Intersects(const Box3D& i_box) const
  {

  for(size_t i = 0; i < 3; ++i)
    {
    if(m_min[i] > i_box.GetMax()[i] || m_max[i] < i_box.GetMin()[i])
      return false;
    }

  return true;
  }

bool Box3D::FullyContains(const Box3D& i_box) const
  {
  return Contains(i_box.m_min) && Contains(i_box.m_max);
  }

double Box3D::DistanceSq(const Vector3D& i_point) const
  {
  if(Contains(i_point))
    return 0;
  Vector3D nearest_point(i_point);

  for(int i = 0; i < 3; ++i)
    {
    if(i_point[i] < m_min[i])
      nearest_point[i] = m_min[i];
    if(i_point[i] > m_max[i])
      nearest_point[i] = m_max[i];
    }

  return (nearest_point - i_point).LengthSq();
  }

double Box3D::Distance(const Vector3D& i_point) const
  {
  return sqrt(DistanceSq(i_point));
  }

Vector3D Box3D::GetMin() const
{
  return m_min;
}

Vector3D Box3D::GetMax() const
{
  return m_max;
}

Vector3D Box3D::GetSize() const
  {
  return Vector3D(m_max[0] - m_min[0],
    m_max[1] - m_min[1],
    m_max[2] - m_min[2]);
  }

namespace Utilities
  {
  std::vector<Vector3D> ParseVectors(const std::vector<Vector3D>& i_vectors, const Box3D& i_box)
    {
    std::vector<Vector3D> vec_contains;
    std::vector<Vector3D>::const_iterator vecIter = i_vectors.begin();

    for(;vecIter != i_vectors.end(); ++vecIter)
      {
      if(i_box.Contains(*vecIter))
        vec_contains.push_back(*vecIter);
      }
    return std::move(vec_contains);
    }
  };