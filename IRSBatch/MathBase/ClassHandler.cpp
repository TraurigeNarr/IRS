#include "ClassHandler.h"

#include "LinearDivisionFunction.h"

#include "math/Vector3D.h"

#include <algorithm>

ClassHandler::ClassHandler(const std::wstring& i_name)
  : m_class_name(i_name)
  , m_valid(false)
{
  SetDivisionFunction<LinearDivisionFunction>();
}

ClassHandler::ClassHandler(const ClassHandler& i_cluster)
  : m_class_name(i_cluster.GetName())
  , m_valid(false)
{
  for (size_t i = 0; i < i_cluster.m_center_points.size(); ++i)
    m_center_points.push_back(i_cluster.m_center_points[i]);

  for (size_t i = 0; i < i_cluster.m_linked_points.size(); ++i)
    m_linked_points.push_back(i_cluster.m_linked_points[i]);

  _Validate();
}

ClassHandler::~ClassHandler()
{
  //clear core points
  for (size_t i = 0; i < m_center_points.size(); ++i)
    delete m_center_points[i];
  m_center_points.clear();
}

void ClassHandler::ClearCorePoints()
{
  m_center_points.clear();
}

void ClassHandler::ClearLinkedPoints()
{
  m_linked_points.clear();
}

std::shared_ptr<IDivisionFunction> ClassHandler::GetDivisionFunction() const
{
  mh_division_function->UpdateCoefficients();
  return mh_division_function;
}

void ClassHandler::AddCorePoint(Vector3D* i_point)
{
  //add point
  m_valid = false;
  const Vector3D& i_position = *i_point;
  MathBase::TPointCloud::iterator it = std::find_if(m_center_points.begin(), m_center_points.end(), [&i_position](Vector3D* i_vinfo)->bool
  {
    return *i_vinfo == i_position;
  });
  if (it == m_center_points.end())
    m_center_points.push_back(i_point);
}

const MathBase::TPointCloud& ClassHandler::GetCorePoints() const
{
  return m_center_points;
}

const MathBase::TPointCloud& ClassHandler::GetLinkedPoints() const
{
  return m_linked_points;
}

void ClassHandler::LinkPoint(Vector3D* i_point)
{
  const Vector3D& i_position = *i_point;
  MathBase::TPointCloud::iterator it = std::find_if(m_linked_points.begin(), m_linked_points.end(), [&i_position](Vector3D* i_vinfo)->bool
  {
    return *i_vinfo == i_position;
  });
  if (it == m_linked_points.end())
  {
    MathBase::TPointCloud::iterator it = std::find_if(m_center_points.begin(), m_center_points.end(), [&i_position](Vector3D* i_vinfo)->bool
    {
      return *i_vinfo == i_position;
    });

    if (it == m_center_points.end())
    {
      m_linked_points.push_back(i_point);
      m_valid = false;
    }
  }
}

void ClassHandler::RemovePoint(Vector3D* ip_point)
{
  if (m_linked_points.size() == 0)
    return;

  MathBase::TPointCloud::iterator it = std::find(m_linked_points.begin(), m_linked_points.end(), ip_point);
  
  if (it == m_linked_points.end())
  {
    m_linked_points.erase(it);
    m_valid = false;
  }
}

bool ClassHandler::RemoveCorePoint(Vector3D* ip_point)
{
  if (m_center_points.size() == 1)
    return false;

  MathBase::TPointCloud::iterator it = std::find(m_center_points.begin(), m_center_points.end(), ip_point);

  if (it == m_center_points.end())
  {
    m_center_points.erase(it);
    m_valid = false;
    return true;
  }

  return false;
}

double ClassHandler::GetDistanceToNearestPoint(const Vector3D& i_vector)
{
  MathBase::TPointCloud::iterator it           = m_center_points.begin();
  MathBase::TPointCloud::const_iterator it_end = m_center_points.end();

  double min_distance = std::numeric_limits<double>::max();

  for ( ; it != it_end; ++it)
    min_distance = std::min(min_distance, (*it)->DistanceSq(i_vector));

  return min_distance;
}

double ClassHandler::GetDistanceToCenterSq(const Vector3D& i_vector, bool i_validate /*= true*/)
{
  if (i_validate)
    _Validate();
  return m_cluster_center.DistanceSq(i_vector);
}

Vector3D ClassHandler::GetCenter() const
{
  _Validate();
  return m_cluster_center;
}

void ClassHandler::_Validate() const
{
  if (m_valid)
    return;

  m_cluster_center.Zero();

  if (m_center_points.size() == 0)
  {
    m_valid = true;
    return;
  }

  //class center
  MathBase::TPointCloud::const_iterator it = m_center_points.cbegin();
  MathBase::TPointCloud::const_iterator it_end = m_center_points.cend();

  for ( ; it != it_end; ++it)
    m_cluster_center += **it;

  m_cluster_center /= m_center_points.size();

  m_valid = true;
}

void ClassHandler::SetName(const std::wstring& i_name)
{
  m_class_name = i_name;
}

std::wstring ClassHandler::GetName() const
{
  return m_class_name;
}

void ClassHandler::Invalidate()
{
  m_valid = false;
}


double ClassHandler::AverageDistanceBetweenPointsAndCenter() const
{
  if (m_linked_points.size() == 0)
    return 0.0;

  double average = 0.0;

  Vector3D center = GetCenter();

  for (size_t i = 0; i < m_linked_points.size(); ++i)
    average += center.DistanceSq(*m_linked_points[i]);

  return average/m_linked_points.size();
}

void ClassHandler::MaxMinLinkedPointDistance(double& o_min, double& o_max) const
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

std::wistream& operator >> (std::wistream& io_stream, ClassHandler& o_class_handler)
{
  //class;<number of ethalons>
  //vertex;4.000000;0.000000;0.000000
  std::wstring temp;
  getline(io_stream, temp);

  size_t pos = 0;

  size_t number = _wtoi(temp.c_str() + 6);

  for(size_t i = 0; i < number; ++i)
  {
    Vector3D* ethalon = new Vector3D();
    io_stream >> *ethalon;
    o_class_handler.AddCorePoint(ethalon);
  }
  return io_stream;
}

std::wostream& operator << (std::wostream& io_stream, const ClassHandler& i_class_handler)
{
  io_stream << L"class;" << i_class_handler.GetCorePoints().size() << std::endl;

  const MathBase::TPointCloud& ethalons = i_class_handler.GetCorePoints();

  std::for_each(ethalons.begin(), ethalons.end(), [&io_stream](Vector3D* ip_vector)
  {
    io_stream << *ip_vector << std::endl;
  });

  return io_stream;
}