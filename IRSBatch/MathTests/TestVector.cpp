#include "stdafx.h"

#include <MathBase\math\Vector3D.h>

#include <UnitTest++.h>

SUITE(Vector3DTests)
  {
  TEST(CheckConstructorForZeroing)
    {
    Vector3D vec;
    CHECK_EQUAL(0, vec[0]);
    CHECK_EQUAL(0, vec[1]);
    CHECK_EQUAL(0, vec[2]);
    }
  TEST(CopyConstructorTest)
    {
    Vector3D vec(1,2,3);
    Vector3D vec1(vec);
    CHECK_EQUAL(vec[0], vec1[0]);
    CHECK_EQUAL(vec[1], vec1[1]);
    CHECK_EQUAL(vec[2], vec1[2]);
    }
  TEST(PlusVectors)
    {
    Vector3D vec(1,2,3);
    Vector3D vec2(vec);

    Vector3D vec_result = vec + vec2;
    CHECK_EQUAL(2, vec_result[0]);
    CHECK_EQUAL(4, vec_result[1]);
    CHECK_EQUAL(6, vec_result[2]);
    }
  TEST(MinusVectors)
    {
    Vector3D vec(1,2,3);
    Vector3D vec2(vec);

    Vector3D vec_result = vec - vec2;
    CHECK_EQUAL(0, vec_result[0]);
    CHECK_EQUAL(0, vec_result[1]);
    CHECK_EQUAL(0, vec_result[2]);
    }
  TEST(MultipleVectorAndValue)
    {
    Vector3D vec(1,2,3);
    Vector3D vec2(vec);
    vec2 *= 10;

    CHECK_EQUAL(10, vec2[0]);
    CHECK_EQUAL(20, vec2[1]);
    CHECK_EQUAL(30, vec2[2]);
    }
  TEST(DotProductVector)
    {
    Vector3D vec(1,2,3);
    Vector3D vec2(vec);
    vec2 *= 10;
    double dot_result = DotProduct(vec2, vec);
    CHECK_EQUAL(140, dot_result);
    }
  TEST(NormalizationTest)
    {
    Vector3D vec;
    vec.Randomize();
    CHECK_CLOSE(1, vec.LengthSq(),1e-8);
    }
  TEST(CrossProductTest)
    {
    Vector3D vec;
    vec.Randomize();
    CHECK_CLOSE(0, vec.CrossProduct(vec).LengthSq(),1e-8);
    }
  }