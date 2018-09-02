#pragma once

#include "Types.h"

#include "IDivisionFunction.h"

#include "math/Vector3D.h"

#include <map>
#include <memory>
#include <vector>
#include <string>
#include <functional>



class ClassHandler 
{
protected:
  std::wstring m_class_name;

  //take ownage of core points
  MathBase::TPointCloud m_center_points;
  //not in our power
  MathBase::TPointCloud m_linked_points;

  mutable std::shared_ptr<IDivisionFunction> mh_division_function;
  mutable Vector3D m_cluster_center;
  mutable bool m_valid;
protected:
  void _Validate() const;

public:
  ClassHandler(const std::wstring& i_name);
  ClassHandler(const ClassHandler& i_cluster);
  virtual ~ClassHandler();

/*TODO must be a classificator
  void        SetDefaultDivisionFunction();
  void        SetDivisionFunction(std::function<double(double)> ip_function);*/
  template <typename TDivisionFunction>
  void          SetDivisionFunction();
  std::shared_ptr<IDivisionFunction> GetDivisionFunction() const;
  double        GetY_FromDivisionFunction(double i_x) const;
  void          ClearLinkedPoints();
  void          ClearCorePoints();
  void          AddCorePoint(Vector3D* i_point);
  const         MathBase::TPointCloud& GetCorePoints() const;
  const         MathBase::TPointCloud& GetLinkedPoints() const;
  void          LinkPoint(Vector3D* i_point);
  void          RemovePoint(Vector3D* ip_point);
  bool          RemoveCorePoint(Vector3D* ip_point);
  //get distance to nearest center point (squared)
  double        GetDistanceToNearestPoint(const Vector3D& i_vector);
  //get distance to cluster center -> calculates average from all points
  //and get distance to that point
  double        GetDistanceToCenterSq(const Vector3D& i_vector, bool i_validate = true);
  void          SetName(const std::wstring& i_name);
  Vector3D      GetCenter() const;
  std::wstring  GetName() const;
  void          Invalidate();

  double        AverageDistanceBetweenPointsAndCenter() const;
  void          MaxMinLinkedPointDistance(double& o_min, double& o_max) const;
};

std::wistream& operator >> (std::wistream& io_stream, ClassHandler& o_class_handler);
std::wostream& operator << (std::wostream& io_stream, const ClassHandler& i_class_handler);

template <typename TDivisionFunction>
void ClassHandler::SetDivisionFunction()
{
  mh_division_function = std::make_shared<TDivisionFunction>(this);
  mh_division_function->UpdateCoefficients();
}