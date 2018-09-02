#include "stdafx.h"

#include <MathBase\math\Box3D.h>
#include <MathBase\math\KDTree.h>

#include <unittest++.h>

#include <algorithm>

SUITE(KDTreeTests)
  {
  TEST(PointsInBox)
    {
    std::vector<Vector3D> initial_points;
    for(int i = 0; i < 100; ++i)
      {
      Vector3D vec(((rand()%10000 - 5000)/5000.0)*3,
        ((rand()%10000 - 5000)/5000.0)*3,
        ((rand()%10000 - 5000)/5000.0)*3);
      initial_points.push_back(vec);
      }

    Box3D box(Vector3D(-2,-2,-2), Vector3D(2,2,2));
    std::vector<Vector3D> points_from_box = Utilities::ParseVectors(initial_points, box);
    KDTree kd_tree(initial_points);
    std::vector<Vector3D> points_from_kd = kd_tree.SelectPointsInBox(box);
    
    
    CHECK_EQUAL(points_from_box.size(), points_from_kd.size());

    for(size_t i = 0; i < points_from_kd.size(); ++i)
      {
      bool point_exist = false;
      for(size_t j = 0; j < points_from_box.size(); ++j)
        {
        int coords = 0;
        for(int k = 0; k < 3; ++k)
          {
          if((points_from_box[j][k] - points_from_kd[i][k]) <= Vector3D::Precision())
            {
            ++coords;
            }
          if(3 == coords)
            {
            point_exist = true;
            break;
            }
          }//for 3 coordinates
        }//for all points from kd
      CHECK_EQUAL(true, point_exist);
      }//for all points in box
    }


  TEST(NearestNeighbourRecursevly)
    {
    std::vector<Vector3D> initial_points;
    for(int i = 0; i < 100; ++i)
      {
      Vector3D vec(((rand()%10000 - 5000)/5000.0)*3,
        ((rand()%10000 - 5000)/5000.0)*3,
        ((rand()%10000 - 5000)/5000.0)*3);
      initial_points.push_back(vec);
      }
    Vector3D rand_point(10,10,10);
    KDTree kd_tree(initial_points);

    Vector3D neighbour = kd_tree.FindNearestNeighbour(rand_point);

    double distance = std::numeric_limits<double>::max();

    for(size_t i = 0; i < initial_points.size(); ++i)
      {
      double temp = (initial_points[i] - rand_point).LengthSq();
      if(temp < distance)
        distance = temp;
      }

    double from_kd = (rand_point - neighbour).LengthSq();
    CHECK_CLOSE(distance, from_kd, Vector3D::Precision());
    }

  TEST(NearestNeighbour)
    {
    std::vector<Vector3D> initial_points;
    srand(0);
    for(int i = 0; i < 100; ++i)
      {
      Vector3D vec(((rand()%10000 - 5000)/5000.0)*3,
        ((rand()%10000 - 5000)/5000.0)*3,
        ((rand()%10000 - 5000)/5000.0)*3);
      initial_points.push_back(vec);
      }
    Vector3D rand_point(10,10,10);
    KDTree kd_tree(initial_points);

    Vector3D neighbour = kd_tree.FindNearestNeighbourRecursevly(rand_point);

    double distance = std::numeric_limits<double>::max();

    for(size_t i = 0; i < initial_points.size(); ++i)
      {
      double temp = (initial_points[i] - rand_point).LengthSq();
      if(temp < distance)
        distance = temp;
      }

    double from_kd = (rand_point - neighbour).LengthSq();
    CHECK_CLOSE(distance, from_kd, Vector3D::Precision());

    }
  }