#include "Vector3D.h"
#include <math.h>
#include <stdlib.h>

#include <string>

Vector3D::Vector3D(double i_x, double i_y, double i_z)
  {
  m_coords[0] = i_x;
  m_coords[1] = i_y;
  m_coords[2] = i_z;
  }

Vector3D::Vector3D()
  {
  m_coords[0] = m_coords[1] = m_coords[2] = 0;
  }


Vector3D::~Vector3D()
{

}

/*
Vector3D::Vector3D(const Vector3D& i_vec)
  {
  *this = i_vec;
  }

Vector3D& Vector3D::operator = (const Vector3D& i_vec)
  {
  this->m_coords[0] = i_vec.GetX();
  this->m_coords[1] = i_vec.GetY();
  this->m_coords[2] = i_vec.GetZ();
  return (*this);
  }*/

void Vector3D::Zero()
{
  m_coords[0] = m_coords[1] = m_coords[2] = 0;
}

double Vector3D::GetX() const
  {
  return m_coords[0];
  }

double Vector3D::GetY() const
  {
  return m_coords[1];
  }

double Vector3D::GetZ() const
  {
  return m_coords[2];
  }

double Vector3D::DotProduct(const Vector3D& i_vec) const
  {
  return i_vec.m_coords[0]*this->m_coords[0] + i_vec.m_coords[1]*this->m_coords[1] + i_vec.m_coords[2]*this->m_coords[2];
  }

Vector3D Vector3D::CrossProduct(const Vector3D& i_vec)
  {
  Vector3D vec_result;
  vec_result[0] = this->GetY()*i_vec.GetZ() - this->GetZ()*i_vec.GetY();
  vec_result[1] = this->GetZ()*i_vec.GetX() - this->GetX()*i_vec.GetZ();
  vec_result[2] = this->GetX()*i_vec.GetY() - this->GetY()*i_vec.GetX();
  return vec_result;
  }

double Vector3D::LengthSq() const
  {
  return m_coords[0]*m_coords[0] + m_coords[1]*m_coords[1] + m_coords[2]*m_coords[2];
  }

double Vector3D::Length() const
  {
  return sqrt(LengthSq());
  }

Vector3D& Vector3D::Normalize()
  {
  (*this) /= this->Length();
  return (*this);
  }

void Vector3D::Randomize()
  {
  m_coords[0] = rand()%100 - 50;
  m_coords[1] = rand()%100 - 50;
  m_coords[2] = rand()%100 - 50;
  Normalize();
  }

double Vector3D::DistanceSq(const Vector3D& i_vec) const
{
  Vector3D vec = i_vec - *this;
  return vec.LengthSq();
}

double Vector3D::Distance(const Vector3D& i_vec) const
{
  return sqrt(DistanceSq(i_vec));
}

Vector3D& Vector3D::operator += (const Vector3D& i_vec)
  {
  this->m_coords[0] += i_vec.GetX();
  this->m_coords[1] += i_vec.GetY();
  this->m_coords[2] += i_vec.GetZ();
  return (*this);
  }

Vector3D& Vector3D::operator -= (const Vector3D& i_vec)
  {
  this->m_coords[0] -= i_vec.GetX();
  this->m_coords[1] -= i_vec.GetY();
  this->m_coords[2] -= i_vec.GetZ();
  return (*this);
  }

Vector3D& Vector3D::operator *= (double i_value)
  {
  this->m_coords[0] *= i_value;
  this->m_coords[1] *= i_value;
  this->m_coords[2] *= i_value;
  return *this;
  }

Vector3D& Vector3D::operator /= (double i_value)
  {
  this->m_coords[0] /= i_value;
  this->m_coords[1] /= i_value;
  this->m_coords[2] /= i_value;
  return *this;
  }

double& Vector3D::operator[](size_t i_index)
  {
  return m_coords[i_index];
  }

double Vector3D::operator[](size_t i_index) const
  {
  return m_coords[i_index];
  }

bool Vector3D::operator==(const Vector3D& i_vector) const
{
  return i_vector[0] == m_coords[0] && i_vector[1] == m_coords[1] && i_vector[2] == m_coords[2];
}

Vector3D operator + (const Vector3D& i_vec1, const Vector3D& i_vec2)
  {
  Vector3D vec_result(i_vec1);
  return vec_result += i_vec2;
  }

Vector3D operator - (const Vector3D& i_vec1, const Vector3D& i_vec2)
  {
  Vector3D vec_result(i_vec1);;
  return vec_result -= i_vec2;
  }

Vector3D operator / (const Vector3D& i_vec, double i_val)
  {
  Vector3D vec_result(i_vec);
  return vec_result /= i_val;
  }

Vector3D operator * (const Vector3D& i_vec, double i_val)
  {
  Vector3D vec_result(i_vec);
  return vec_result *= i_val;
  }

double DotProduct(const Vector3D& i_vec1, const Vector3D& i_vec2)
  {
  Vector3D vec_result(i_vec1);
  return vec_result.DotProduct(i_vec2);
  }

double Vector3D::Precision()
  {
  return 1e-10;
  }

std::wistream& operator >> (std::wistream& io_stream, Vector3D& o_vector)
  {
  //vertex;4.000000;0.000000;0.000000
  std::wstring temp;
  getline(io_stream, temp);

  size_t pos = 0;


  for(int i = 0; i < 3; ++i)
    {
    pos = temp.find(';', pos);
    ++pos;
    o_vector[i] = _wtof(temp.substr(pos).c_str());
    }
  return io_stream;
  }

std::wostream& operator << (std::wostream& io_stream, const Vector3D& i_vector)
{
  return io_stream << L"vertex;" << i_vector[0] << L";"
                    << i_vector[1] << L";"
                    << i_vector[2];
}