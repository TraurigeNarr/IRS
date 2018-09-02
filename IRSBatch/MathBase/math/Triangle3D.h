#pragma once

#include "Vector3D.h"
#include <istream>
#include <memory>

class Triangle3D
  {
  public:
                            Triangle3D(const Vector3D& i_first, const Vector3D& i_second, const Vector3D& i_third);

    const Vector3D&         operator [] (size_t i_index) const;
    Vector3D&               operator [] (size_t i_index);                
  private:
    //array of vertices of basic model
    Vector3D                m_vertices[3];
  };