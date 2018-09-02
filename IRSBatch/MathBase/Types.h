#pragma once

#include <vector>

class Vector3D;
class Cluster;
class ClassHandler;

namespace MathBase
{
  typedef std::vector<Vector3D*> TPointCloud;
  typedef std::vector<Cluster*> TClusters;
  typedef std::vector<ClassHandler*> TClasses;
}