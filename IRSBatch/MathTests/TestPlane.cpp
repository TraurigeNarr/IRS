#include "stdafx.h"

#include <MathBase\math\Plane3D.h>

#include <UnitTest++.h>

SUITE(PlaneTests)
{
  TEST(PlaneIntersectionTest)
  {
    Vector3D norm(1,1,1);
    Vector3D vec1(20,20,20);
    Vector3D vec2(-15,-15,-7);

    Plane3D plane(norm,3);
    Vector3D point;
    CHECK_EQUAL(true, plane.GetIntersectionPoint(point, vec1, vec2));
    CHECK_CLOSE(-plane.GetDistance(), point.DotProduct(plane.GetNormal()),1e-10);
  }
}