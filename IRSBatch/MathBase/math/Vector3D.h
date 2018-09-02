#pragma once

#include <istream>

class Vector3D
  {
  public:
              Vector3D(double i_x, double i_y, double i_z);
              Vector3D();
              virtual ~Vector3D();
              /*Vector3D(const Vector3D& i_vec);
    Vector3D& operator = (const Vector3D& i_vec);*/

    void      Zero();
    double    GetX() const;
    double    GetY() const;
    double    GetZ() const;

    double    DotProduct(const Vector3D& i_vec) const;
    Vector3D  CrossProduct(const Vector3D& i_vec);
    double    Length() const;
    double    LengthSq() const;
    Vector3D& Normalize();
    void      Randomize();

    double    DistanceSq(const Vector3D& i_vec) const;
    double    Distance(const Vector3D& i_vec) const;

    Vector3D& operator += (const Vector3D& i_vec);
    Vector3D& operator -= (const Vector3D& i_vec);
    Vector3D& operator *= (double i_value);
    Vector3D& operator /= (double i_value);
    double&   operator [] (size_t i_index);
    double    operator [] (size_t i_index) const;
    bool      operator == (const Vector3D& i_vector) const;

    static double Precision();
  private:
    double    m_coords[3];
  };

Vector3D  operator + (const Vector3D& i_vec1, const Vector3D& i_vec2);
Vector3D  operator - (const Vector3D& i_vec1, const Vector3D& i_vec2);
Vector3D  operator / (const Vector3D& i_vec1,double i_val);
Vector3D  operator * (const Vector3D& i_vec, double i_val);
double    DotProduct(const Vector3D& i_vec1, const Vector3D& i_vec2);
Vector3D  CrossProduct(const Vector3D& i_vec1, const Vector3D& i_vec2);

std::wistream& operator >> (std::wistream& io_stream, Vector3D& o_vector);
std::wostream& operator << (std::wostream& io_stream, const Vector3D& i_vector);
