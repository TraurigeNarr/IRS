#pragma once
#include "Vector3D.h"

#include <vector>

class Box3D
  {
  public:
              Box3D();
              Box3D(const Vector3D& i_min, const Vector3D& i_max);
    bool      Contains(const Vector3D& i_point) const;
    bool      Intersects(const Box3D& i_box) const;
    bool      FullyContains(const Box3D& i_box) const;
    double    Distance(const Vector3D& i_point) const;
    double    DistanceSq(const Vector3D& i_point) const;
    Vector3D  GetMin() const;
    Vector3D  GetMax() const;
    Vector3D  GetSize() const;
  private:
    Vector3D  m_min;
    Vector3D  m_max;
  };

namespace Utilities
  {
  std::vector<Vector3D> ParseVectors(const std::vector<Vector3D>& i_vectors, const Box3D& i_box);
  }