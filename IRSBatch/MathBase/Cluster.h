#pragma once

#include "Types.h"

#include "math/Vector3D.h"

#include <map>
#include <vector>
#include <string>


class Cluster 
{
protected:
  std::wstring m_cluster_name;

/*  MathBase::TPointCloud m_center_points;*/
  MathBase::TPointCloud m_linked_points;

  mutable Vector3D m_cluster_center;
  mutable bool m_valid;
protected:
  void _Validate() const;

public:
  Cluster();
  Cluster(const std::wstring& i_name);
  Cluster(const Cluster& i_cluster);
  virtual ~Cluster();

  void        ClearLinkedPoints();
  //void        ClearCorePoints();
  //void        AddCorePoint(Vector3D* i_point);
  //const       MathBase::TPointCloud& GetCorePoints() const;
  void        LinkPoint(Vector3D* i_point);
  void        RemovePoint(Vector3D* i_point);
  bool        Contains(Vector3D* i_point) const;
  //get distance to nearest center point (squared)
  double      GetDistanceToNearestPoint(const Vector3D& i_vector);
  //get distance to cluster center -> calculates average from all points
  //and get distance to that point
  double      GetDistanceToCenterSq(const Vector3D& i_vector, bool i_validate = true) const;
  void        SetName(const std::wstring& i_name);
  Vector3D    GetCenter() const;
  std::wstring GetName() const;
  const       MathBase::TPointCloud& GetLinkedPoints() const;

  double        AverageDistanceBetweenPointsAndCenter() const;
  void          MaxMinLinkedPointDistance(double& o_min, double& o_max) const;
  double        GetDispersion() const;
  double        GetDeviation() const;
};