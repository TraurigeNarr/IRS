#pragma once
#include "Vector3D.h"

class Matrix3D
  {
  public:
    Matrix3D();

    void      MakeRandomRotatation();
    Vector3D  TransformVector(const Vector3D& i_vector);
    Matrix3D  GetTranposeMatrix() const;
    bool      GetRotationEigenVector(Vector3D& o_eigen_vec) const;
    double&   operator () (size_t i_row, size_t i_column);
    double    operator()(size_t i_row, size_t i_column) const;
  private:
    double  m_matrix[3][3];
  };