#include "Cluster.h"

#include "math/Vector3D.h"

#include <algorithm>

Cluster::Cluster()
  : m_cluster_name(L"")
  , m_valid(false)
{

}

Cluster::Cluster(const std::wstring& i_name)
  : m_cluster_name(i_name)
  , m_valid(false)
{

}

Cluster::Cluster(const Cluster& i_cluster)
  : m_cluster_name(i_cluster.GetName())
  , m_valid(false)
{
  /*for (size_t i = 0; i < i_cluster.m_center_points.size(); ++i)
    m_center_points.push_back(i_cluster.m_center_points[i]);*/

  for (size_t i = 0; i < i_cluster.m_linked_points.size(); ++i)
    m_linked_points.push_back(i_cluster.m_linked_points[i]);

  m_valid = false;
  _Validate();
}

Cluster::~Cluster()
{

}

/*
void Cluster::ClearCorePoints()
{
  m_center_points.clear();
}*/

void Cluster::ClearLinkedPoints()
{
  m_linked_points.clear();
}

void Cluster::RemovePoint(Vector3D* ip_point)
{
  MathBase::TPointCloud::iterator it = std::find(m_linked_points.begin(), m_linked_points.end(), ip_point);

  if (it == m_linked_points.end())
  {
    m_linked_points.erase(it);
    m_valid = false;
  }
}

bool Cluster::Contains(Vector3D* i_point) const
{
  return std::find(m_linked_points.begin(), m_linked_points.end(), i_point) != m_linked_points.end();
}

double Cluster::AverageDistanceBetweenPointsAndCenter() const
  {
  if (m_linked_points.size() == 0)
    return 0.0;

  double average = 0.0;

  Vector3D center = GetCenter();

  for (size_t i = 0; i < m_linked_points.size(); ++i)
    average += center.DistanceSq(*m_linked_points[i]);

  return average/m_linked_points.size();
  }

void Cluster::MaxMinLinkedPointDistance(double& o_min, double& o_max) const
  {
  if (m_linked_points.size() == 0)
    {
    o_max = o_min = 0.0;
    return;
    }

  o_max = 0;
  o_min = std::numeric_limits<double>::max();

  Vector3D center = GetCenter();

  for (size_t i = 0; i < m_linked_points.size(); ++i)
    {
    double dist = center.DistanceSq(*m_linked_points[i]);

    o_max = std::max(o_max, dist);
    o_min = std::min(o_min, dist);
    }
  }

double Cluster::GetDispersion() const
  {
  if (m_linked_points.size() == 0)
    return 0.0;

  Vector3D center = GetCenter();
  double dispersion = 0.0;
  for (size_t i = 0; i < m_linked_points.size(); ++i)
    dispersion += center.DistanceSq(*m_linked_points[i]);

  return dispersion;
  }

double Cluster::GetDeviation() const
  {
  if (m_linked_points.size() == 0)
    return 0.0;

  Vector3D center = GetCenter();
  double dispersion = 0.0;
  for (size_t i = 0; i < m_linked_points.size(); ++i)
    dispersion += center.DistanceSq(*m_linked_points[i]);

  return dispersion / m_linked_points.size();
  }

void Cluster::LinkPoint(Vector3D* i_point)
{
  const Vector3D& i_position = *i_point;
  MathBase::TPointCloud::iterator it = std::find_if(m_linked_points.begin(), m_linked_points.end(), [&i_position](Vector3D* i_vinfo)->bool
  {
    return *i_vinfo == i_position;
  });
  if (it == m_linked_points.end())
  {
   /* MathBase::TPointCloud::iterator it = std::find_if(m_center_points.begin(), m_center_points.end(), [&i_position](Vector3D* i_vinfo)->bool
    {
      return *i_vinfo == i_position;
    });

    if (it == m_center_points.end())
    {*/
      m_linked_points.push_back(i_point);
      m_valid = false;
    //}
  }
}

double Cluster::GetDistanceToNearestPoint(const Vector3D& i_vector)
{
  MathBase::TPointCloud::iterator it = m_linked_points.begin();
  MathBase::TPointCloud::const_iterator it_end = m_linked_points.end();

  double min_distance = std::numeric_limits<double>::max();

  for ( ; it != it_end; ++it)
    min_distance = std::min(min_distance, (*it)->DistanceSq(i_vector));

  return min_distance;
}

double Cluster::GetDistanceToCenterSq(const Vector3D& i_vector, bool i_validate /*= true*/) const
{
  if (i_validate)
    _Validate();
  return m_cluster_center.DistanceSq(i_vector);
}

Vector3D Cluster::GetCenter() const
{
  _Validate();
  return m_cluster_center;
}

void Cluster::_Validate() const
{
  if (m_valid)
    return;

  m_cluster_center.Zero();

  MathBase::TPointCloud::const_iterator it = m_linked_points.begin();
  MathBase::TPointCloud::const_iterator it_end = m_linked_points.end();

  for ( ; it != it_end; ++it)
    m_cluster_center += **it;

  m_cluster_center /= m_linked_points.size();

  m_valid = true;
}

void Cluster::SetName(const std::wstring& i_name)
{
  m_cluster_name = i_name;
}

std::wstring Cluster::GetName() const
{
  return m_cluster_name;
}

const MathBase::TPointCloud& Cluster::GetLinkedPoints() const
{
  return m_linked_points;
}