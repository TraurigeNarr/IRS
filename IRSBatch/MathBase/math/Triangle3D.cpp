#include "Triangle3D.h"

#include <string>
#include <cassert>

Triangle3D::Triangle3D(const Vector3D& i_first, const Vector3D& i_second, const Vector3D& i_third)
  {
  m_vertices[0] = i_first;
  m_vertices[1] = i_second;
  m_vertices[2] = i_third;
  }

const Vector3D& Triangle3D::operator [] (size_t i_index) const
  {
  assert(i_index < 3);
  return m_vertices[i_index];
  }

Vector3D& Triangle3D::operator[] (size_t i_index)
  {
  assert(i_index < 3);
  return m_vertices[i_index];
  }