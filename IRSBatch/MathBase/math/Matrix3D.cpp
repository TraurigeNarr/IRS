#include "Matrix3D.h"
#include <math.h>

Matrix3D::Matrix3D()
{
  for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j)
    {
      m_matrix[i][j] = i == j ? 1 : 0;
    }
}

double& Matrix3D::operator () (size_t i_row, size_t i_column)
{
  return m_matrix[i_row][i_column];
}

double Matrix3D::operator () (size_t i_row, size_t i_column) const
{
  return m_matrix[i_row][i_column];
}

void Matrix3D::MakeRandomRotatation()
{
  Vector3D e1;
  e1.Randomize();
  Vector3D support_vec;
  support_vec.Randomize();
  Vector3D e2 = e1.CrossProduct(support_vec);
  e2.Normalize();
  Vector3D e3 = e1.CrossProduct(e2);
  e3.Normalize();

  m_matrix[0][0] = e1[0];
  m_matrix[1][0] = e1[1];
  m_matrix[2][0] = e1[2];

  m_matrix[0][1] = e2[0];
  m_matrix[1][1] = e2[1];
  m_matrix[2][1] = e2[2];

  m_matrix[0][2] = e3[0];
  m_matrix[1][2] = e3[1];
  m_matrix[2][2] = e3[2];
}

Vector3D Matrix3D::TransformVector(const Vector3D& i_vector)
{
  Vector3D transform_vector;

  for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j)
      transform_vector[i] += m_matrix[i][j]*i_vector[j];

  return transform_vector;
}

bool Matrix3D::GetRotationEigenVector(Vector3D& o_eigen_vec) const
{
  Vector3D e1(m_matrix[0][0],m_matrix[1][0], m_matrix[2][0]);
  Vector3D e2(m_matrix[0][1],m_matrix[1][1], m_matrix[2][1]);
  Vector3D e3(m_matrix[0][2],m_matrix[1][2], m_matrix[2][2]);
  if(Vector3D::Precision() < fabs(e1.DotProduct(e2)) || 
    Vector3D::Precision() < fabs(e1.DotProduct(e3)) || 
    Vector3D::Precision() < fabs(e2.DotProduct(e3)))
    return false;
  o_eigen_vec = Vector3D(m_matrix[2][1]-m_matrix[1][2], 
    m_matrix[0][2]-m_matrix[2][0],
    m_matrix[1][0]-m_matrix[0][1]);
  return true;
}

Matrix3D Matrix3D::GetTranposeMatrix() const
{
  Matrix3D m;
  for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j)
      m(i,j) = m_matrix[j][i];
  return m;
}