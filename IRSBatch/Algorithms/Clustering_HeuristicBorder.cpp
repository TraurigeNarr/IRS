#include "stdafx.h"

#include "Clustering_HeuristicBorder.h"

#include "AlgorithmManager.h"
#include "AlgorithmsUtilities.h"
#include "Reporter.h"

#include <MathBase/Cluster.h>
#include <MathBase/math/Vector3D.h>

#include <algorithm>

namespace Algorithms
{

  Clustering_HeuristicBorder::Clustering_HeuristicBorder(const std::wstring i_analyzer_name, int i_available_int)
    : IAnalyzer<Clustering_Parameters>(i_analyzer_name)
    , m_mode (i_available_int)
  {

  }

  Clustering_HeuristicBorder::~Clustering_HeuristicBorder()
  {

  }

  bool Clustering_HeuristicBorder::IsAvailableInMode(int i_mode) const
  {
    return i_mode == m_mode;
  }

  bool Clustering_HeuristicBorder::Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter /*= nullptr*/)
  {
    using namespace MathBase;

    TPointCloud& points = GetParameters().m_points;
    TClusters* clusters = GetParameters().mp_clusters;
    //select first clusters
    Utilities::ClearClusters(*clusters);

    //complexity of algorithm is O(n)
    size_t iteration_count = points.size();
    //because we calculate distance in squared
    double border_distance = GetParameters().m_radius_to_append * GetParameters().m_radius_to_append;
    for (size_t i = 0; i < iteration_count; ++i)
    {
      if (points.size() == 0)
        break;
      //select point to process
      size_t point_index = Utilities::GetFirstClusterIndex(GetParameters());
      Vector3D* p_point = GetParameters().m_points[point_index];

      if (i_reporter && GetParameters().m_report_on_each_iteration)
        {
        i_reporter->Report(L"Image size");
        i_reporter->Report(Reporters::Delimiter);
        i_reporter->Report(GetParameters().m_points.size());
        i_reporter->Report(Reporters::NewLine);
        i_reporter->Report(L"Image index ");
        i_reporter->Report(point_index);
        i_reporter->Report(Reporters::NewLine);
        i_reporter->Report(Reporters::Offset);
        i_reporter->Report(*p_point);
        i_reporter->Report(Reporters::NewLine);
        }

      double min_distance = std::numeric_limits<double>::max();
      TClusters::const_iterator clusters_end = clusters->end();
      TClusters::iterator it_cluster = clusters->begin();
      TClusters::iterator it_candidate = clusters->end();
      //find candidate
      for ( ; it_cluster != clusters_end; ++it_cluster)
      {
      double distance_to_cluster = (*it_cluster)->GetDistanceToCenterSq(*p_point, false);
      if (i_reporter && GetParameters().m_report_on_each_iteration)
        {
        i_reporter->Report(Reporters::Offset);
        i_reporter->Report(L"Distance to ");
        i_reporter->Report((*it_cluster)->GetName());
        i_reporter->Report(Reporters::Delimiter);
        i_reporter->Report(distance_to_cluster);
        i_reporter->Report(Reporters::NewLine);
        }
      if (distance_to_cluster <= border_distance && distance_to_cluster < min_distance)
        {
        it_candidate = it_cluster;
        min_distance = distance_to_cluster;
        }
      }
      //if candidate is not found then create new cluster with center in p_point
      if (it_candidate == clusters_end)
      {
      clusters->push_back(new Cluster(AlgorithmsInstance->GenerateNextName()));
      clusters->back()->LinkPoint(p_point);
      //validate cluster
      clusters->back()->GetCenter();
      if (i_reporter && GetParameters().m_report_on_each_iteration)
        {
        i_reporter->Report(Reporters::Offset);
        i_reporter->Report(L"Create new cluster "); 
        i_reporter->Report(clusters->back()->GetName());
        i_reporter->Report(Reporters::NewLine);
        }
      }
      else
      {
      if (i_reporter && GetParameters().m_report_on_each_iteration)
        {
        i_reporter->Report(Reporters::Offset);
        i_reporter->Report(L"Link to");
        i_reporter->Report(Reporters::Delimiter);
        i_reporter->Report((*it_candidate)->GetName());
        i_reporter->Report(Reporters::NewLine);
        }
      (*it_candidate)->LinkPoint(p_point);
      }
    //update points
    Utilities::RemoveClassifiedPoints(*clusters, points);
    }
    
    if (i_reporter)
      {
      i_reporter->Report(Reporters::NewLine);
      i_reporter->Report(L"Final clusters");
      i_reporter->Report(Reporters::NewLine);
      Algorithms::Utilities::GenerateReport(i_reporter, *GetParameters().mp_clusters);
      i_reporter->Report(Reporters::NewLine);
      }

    return true;
  }
}