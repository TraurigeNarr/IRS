#include "stdafx.h"

#include "ClusteringK_Means.h"

#include "AlgorithmsUtilities.h"
#include "Reporter.h"

#include <MathBase/Cluster.h>
#include <MathBase/math/Vector3D.h>

#include <algorithm>

//////////////////////////////////////////////////////////////////////////
using namespace Algorithms;

//////////////////////////////////////////////////////////////////////////

ClusteringK_Means::ClusteringK_Means(const std::wstring i_analyzer_name, int i_available_int)
  : IAnalyzer<Clustering_Parameters>(i_analyzer_name)
  , m_mode (i_available_int)
{

}

ClusteringK_Means::~ClusteringK_Means()
{

}


bool ClusteringK_Means::IsAvailableInMode(int i_mode) const
{
  return i_mode == m_mode;
}

void ClusteringK_Means::_Iterate()
{
  using namespace MathBase;

  TPointCloud& points = GetParameters().m_points;
  TClusters* clusters = GetParameters().mp_clusters;

  TClusters::const_iterator clusters_end = clusters->end();
  TClusters::iterator nearest_cluster = clusters->begin();

  for (size_t i = 0; i < points.size(); ++i)
  {
    double min_distance = std::numeric_limits<double>::max();
    TClusters::iterator it_cluster = clusters->begin();
    //find nearest cluster
    for ( ; it_cluster != clusters_end; ++it_cluster)
    {
      //do not update center point of cluster
      double dist_to_cluster = (**it_cluster).GetDistanceToCenterSq(*points[i], false);

      if (dist_to_cluster < min_distance)
      {
        nearest_cluster = it_cluster;
        min_distance = dist_to_cluster;
      }
    }
    //link to center points
    (**nearest_cluster).LinkPoint(points[i]);
  }

}

bool ClusteringK_Means::Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter /*= nullptr*/)
{
  using namespace MathBase;
  using namespace Algorithms;
  
  //select first clusters
  Utilities::PrepareParameters(GetParameters());

  if (GetParameters().mp_clusters->size() == 0)
    return false;

  size_t it_number = GetParameters().m_iterations == -1 ? 10000 : GetParameters().m_iterations;
  size_t i = 0;
  for (i = 0; i < it_number; ++i)
  {
    //remember centers
   TPointCloud centers_before; 
   Utilities::RememberCenterPoints(*GetParameters().mp_clusters, centers_before);
   //iterate
   _Iterate();
   //check for exit
   if (Utilities::CompareCenterPoints(*GetParameters().mp_clusters, centers_before, GetParameters().m_center_shift_allowed))
     {
     if (i_reporter && GetParameters().m_report_on_each_iteration)
       {
       i_reporter->Report(Reporters::NewLine);
       }
     break;
     }
   if (i_reporter && GetParameters().m_report_on_each_iteration)
     {
     if (i != 0)
       i_reporter->Report(Reporters::NewLine);
     i_reporter->Report(L"Iteration");
     i_reporter->Report(Reporters::Delimiter);
     i_reporter->Report(i);
     i_reporter->Report(Reporters::NewLine);
     Algorithms::Utilities::GenerateReport(i_reporter, *GetParameters().mp_clusters);
     i_reporter->Report(Reporters::NewLine);
     }
  }
  
  if (i_reporter)
    {
    i_reporter->Report(L"Iteration number");
    i_reporter->Report(Reporters::Delimiter);
    i_reporter->Report(i);
    i_reporter->Report(Reporters::NewLine);
    Algorithms::Utilities::GenerateReport(i_reporter, *GetParameters().mp_clusters);
    i_reporter->Report(Reporters::NewLine);
    }

  return true;
}
