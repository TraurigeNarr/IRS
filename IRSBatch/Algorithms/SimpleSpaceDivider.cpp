#include "stdafx.h"

#include "SimpleSpaceDivider.h"

#include "AlgorithmsUtilities.h"
#include "Reporter.h"

#include <MathBase/ClassHandler.h>
#include <MathBase/math/Vector3D.h>

#include <sstream>

//////////////////////////////////////////////////////////////////////////
using namespace Algorithms;

//////////////////////////////////////////////////////////////////////////

SimpleSpaceDivider::SimpleSpaceDivider(const std::wstring i_mode_name, int i_available_mode)
  : IAnalyzer<SimpleSpaceDividerParameters>(i_mode_name)
  , m_mode(i_available_mode)
{

}

SimpleSpaceDivider::~SimpleSpaceDivider()
{

}


bool SimpleSpaceDivider::IsAvailableInMode(int i_mode) const
{
  return i_mode == m_mode;
}

bool SimpleSpaceDivider::Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter /*= nullptr*/)
{
  using namespace MathBase;
  using namespace Algorithms::Reporters;
  
  TPointCloud& points = GetParameters().m_points;
  TClasses& classes = GetParameters().m_clusters;
  if (classes.size() == 0)
    return true;
  TClasses::const_iterator clusters_end = classes.end();
  TClasses::iterator nearest_cluster = classes.begin();
  if (i_reporter)
  {
    i_reporter->Report(L"Classes number");
    i_reporter->Report(Delimiter);
    i_reporter->Report(classes.size());
    i_reporter->Report(NewLine);
    i_reporter->Report(L"Images number");
    i_reporter->Report(Delimiter);
    i_reporter->Report(points.size());
    i_reporter->Report(NewLine);
    i_reporter->Report(NewLine);
  }
  
  //enumerate all points
  for (size_t i = 0; i < points.size(); ++i)
  {
    double min_distance = std::numeric_limits<double>::max();

    const Vector3D& point = *points[i];
    if (i_reporter)
    {
      i_reporter->Report(L"Image ");
      i_reporter->Report(i+1);
      i_reporter->Report(NewLine);
      i_reporter->Report(Offset);
      i_reporter->Report(point);
      i_reporter->Report(NewLine);
    }
    
    TClasses::iterator it_cluster = classes.begin();
    //find nearest cluster
    for ( ; it_cluster != clusters_end; ++it_cluster)
    {
      double dist_to_cluster = (**it_cluster).GetDistanceToNearestPoint(point);
      if (i_reporter)
      {
        i_reporter->Report(Offset);
        i_reporter->Report(L"Distance to ");
        i_reporter->Report((**it_cluster).GetName());
        i_reporter->Report(Delimiter);
        i_reporter->Report(dist_to_cluster);
        i_reporter->Report(NewLine);
      }
      if (dist_to_cluster < min_distance)
      {
        nearest_cluster = it_cluster;
        min_distance = dist_to_cluster;
      }
    }
    if (i_reporter)
    {
      i_reporter->Report(Offset);
      i_reporter->Report(L"Nearest class");
      i_reporter->Report(Delimiter);
      i_reporter->Report((**nearest_cluster).GetName());
      i_reporter->Report(NewLine);
    }
    //link point to this cluster
    (**nearest_cluster).LinkPoint(points[i]);
  }
  if (i_reporter)
  {
    i_reporter->Report(Reporters::NewLine);
    Utilities::GenerateReport(i_reporter, classes);
  }
  return true;
}

