#include "stdafx.h"

#include <MathBase\math\Matrix3D.h>

#include <UnitTest++.h>

SUITE(MatrixTests)
{
  TEST(MatrixRotationTest)
  {
    Matrix3D matrix;
    matrix.MakeRandomRotatation();

    Vector3D e1(matrix(0,0), matrix(1,0),matrix(2,0));
    Vector3D e2(matrix(0,1), matrix(1,1),matrix(2,1));
    Vector3D e3(matrix(0,2), matrix(1,2),matrix(2,2));
    CHECK_CLOSE(0,e1.DotProduct(e2),Vector3D::Precision());
    CHECK_CLOSE(0,e1.DotProduct(e3),Vector3D::Precision());
    CHECK_CLOSE(0,e2.DotProduct(e3),Vector3D::Precision());
  }

  TEST(CheckTransformation)
  {
    Matrix3D matrix;
    matrix.MakeRandomRotatation();

    Vector3D some_vector;
    some_vector.Randomize();
    Vector3D transform_vector = matrix.TransformVector(some_vector);

    CHECK_CLOSE(some_vector.LengthSq(), transform_vector.LengthSq(), Vector3D::Precision());
  }
  TEST(CheckEigenVector)
  {
    Matrix3D matrix;
    matrix.MakeRandomRotatation();

    Vector3D some_vector;
    some_vector.Randomize();
    Vector3D transform_vector = matrix.TransformVector(some_vector);
    Matrix3D transpose_m = matrix.GetTranposeMatrix();

    transform_vector = transpose_m.TransformVector(transform_vector);

    CHECK_CLOSE(0, (some_vector - transform_vector).LengthSq(), Vector3D::Precision());
  }
}