#include "stdafx.h"

#include "Clustering_Maxmin.h"

#include "AlgorithmManager.h"
#include "AlgorithmsUtilities.h"
#include "Reporter.h"

#include <MathBase/Cluster.h>
#include <MathBase/math/Vector3D.h>

#include <algorithm>
#include <cassert>


namespace
  {

  size_t _GetImageNumber(Vector3D* ip_image, const Algorithms::CLustering_Maxmin::TImageInformations& i_informations)
    {
    Algorithms::CLustering_Maxmin::TImageInformations::const_iterator it = std::find_if(i_informations.begin(), i_informations.end(), [ip_image](const Algorithms::CLustering_Maxmin::ImageInformation& i_information) -> bool 
      { return ip_image == i_information.mp_image; });

    return it != i_informations.end() ? it->m_number : 0;
    }

  }

namespace Algorithms
  {

  CLustering_Maxmin::ImageInformation::ImageInformation(Vector3D* ip_image, size_t i_number)
    : mp_image(ip_image)
    , m_number(i_number)
    {}

  CLustering_Maxmin::CLustering_Maxmin(const std::wstring i_analyzer_name, int i_available_int)
    : IAnalyzer<Clustering_Parameters>(i_analyzer_name)
    , m_mode (i_available_int)
  {

  }

  CLustering_Maxmin::~CLustering_Maxmin()
  {

  }

  bool CLustering_Maxmin::IsAvailableInMode(int i_mode) const
  {
    return i_mode == m_mode;
  }

  bool CLustering_Maxmin::_Iterate(const TImageInformations& i_informations, std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter /*= nullptr*/)
  {
    using namespace MathBase;

    TPointCloud& points = GetParameters().m_points;
    TClusters& clusters = *GetParameters().mp_clusters;

    assert (clusters.size() >= 2);

    TPointCloud::const_iterator points_end = points.end();
    TPointCloud::iterator it_point = points.begin();

    double border_value = 0.0;
    if (GetParameters().m_maxmin_distance_criteria == Clustering_Parameters::MMDC_AVERAGE_DISTANCE)
      {
      double total_distance = 0.0;
      size_t count = 0;
      for ( size_t i = 0; i < clusters.size(); ++i)
        {
        for (size_t j = i + 1; j < clusters.size(); ++j)
          {
          total_distance += clusters[i]->GetDistanceToCenterSq(clusters[j]->GetCenter());
          ++count;
          }
        }

      border_value = total_distance / count;
      }
    else if (GetParameters().m_maxmin_distance_criteria == Clustering_Parameters::MMDC_DISTANCE_BETWEEN_LAST_TWO_CLUSTERS)
      {
      Cluster* p_cluster_last = clusters[clusters.size() - 1];
      Cluster* p_cluster_pre_last = clusters[clusters.size() - 2];
      border_value = p_cluster_last->GetDistanceToCenterSq(p_cluster_pre_last->GetCenter());
      }

    border_value *= GetParameters().m_maxmin_separating_value;

    if (i_reporter && GetParameters().m_report_on_each_iteration)
      {
      i_reporter->Report(L"Border distance");
      i_reporter->Report(Reporters::Delimiter);
      i_reporter->Report(border_value);
      i_reporter->Report(Reporters::NewLine);
      }

    double current_max_distance = 0.0;
    TClusters::const_iterator clusters_end = clusters.end();
    TPointCloud::const_iterator it_candidate = points_end;
    for ( ; it_point != points_end; ++it_point)
      {

      if (i_reporter && GetParameters().m_report_on_each_iteration)
        {
        i_reporter->Report(Reporters::Offset);
        i_reporter->Report(L"Image ");
        i_reporter->Report(_GetImageNumber(*it_point, i_informations));
        i_reporter->Report(Reporters::NewLine);
        i_reporter->Report(Reporters::Offset);
        i_reporter->Report(Reporters::Offset);
        i_reporter->Report(**it_point);
        i_reporter->Report(Reporters::NewLine);
        }

      double min_distance = std::numeric_limits<double>::max();

      for (TClusters::iterator it_cluster = clusters.begin(); it_cluster != clusters_end; ++it_cluster)
        {
        double distance_to_cluster = (*it_cluster)->GetDistanceToCenterSq(**it_point);
        if (i_reporter && GetParameters().m_report_on_each_iteration)
          {
          i_reporter->Report(Reporters::Offset);
          i_reporter->Report(L"Distance to ");
          i_reporter->Report((*it_cluster)->GetName());
          i_reporter->Report(Reporters::Delimiter);
          i_reporter->Report(distance_to_cluster);
          i_reporter->Report(Reporters::NewLine);
          }

        min_distance = std::min(distance_to_cluster, min_distance);
        }

      if (min_distance >= border_value && current_max_distance < min_distance)
        {
        it_candidate = it_point;
        if (i_reporter && GetParameters().m_report_on_each_iteration)
          {
          i_reporter->Report(Reporters::Offset);
          i_reporter->Report(L"Create new cluster from image");
          i_reporter->Report(Reporters::NewLine);
          }
        }
      }

    //create new cluster if we have a candidate
    if (it_candidate != points.end())
    {
    if (i_reporter && GetParameters().m_report_on_each_iteration)
      {
      i_reporter->Report(Reporters::Offset);
      i_reporter->Report(L"Create new cluster from image");
      i_reporter->Report(Reporters::Delimiter);
      i_reporter->Report(points_end - it_candidate);
      }
    clusters.push_back(new Cluster(AlgorithmsInstance->GenerateNextName()));
    clusters.back()->LinkPoint(*it_candidate);
    }

    return it_candidate != points.end();
  }

  void CLustering_Maxmin::_GetFarestCluster(const MathBase::TPointCloud& i_points, const Cluster& i_cluster, Cluster* op_farest_cluster)
  {
  Vector3D* p_farest = nullptr;
  double max_distance = 0.0;
  std::for_each(i_points.begin(), i_points.end(), [&i_cluster, &p_farest, &max_distance](Vector3D* ip_point)
  {
    double dist_to_cluster = i_cluster.GetDistanceToCenterSq(*ip_point);
    if (max_distance < dist_to_cluster)
    {
      max_distance = dist_to_cluster;
      p_farest = ip_point;
    }
  });

  op_farest_cluster->LinkPoint(p_farest);
  }

  void CLustering_Maxmin::_ClassifyPoints(MathBase::TClusters& i_clusters, MathBase::TPointCloud& i_points)
  {
    using namespace MathBase;

    TClusters::iterator clusters_end = i_clusters.end();
    TClusters::iterator nearest_cluster = i_clusters.begin();

    //enumerate all points
    for (size_t i = 0; i < i_points.size(); ++i)
    {
      double min_distance = std::numeric_limits<double>::max();

      const Vector3D& point = *i_points[i];
      TClusters::iterator it_cluster = i_clusters.begin();
      //find nearest cluster
      for ( ; it_cluster != clusters_end; ++it_cluster)
      {
        double dist_to_cluster = (**it_cluster).GetDistanceToNearestPoint(point);

        if (dist_to_cluster < min_distance)
        {
          nearest_cluster = it_cluster;
          min_distance = dist_to_cluster;
        }
      }
      //link point to this cluster
      (**nearest_cluster).LinkPoint(i_points[i]);
    }

  }

  bool CLustering_Maxmin::Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter /*= nullptr*/)
  {
    using namespace MathBase;

    TPointCloud& points = GetParameters().m_points;
    TClusters* clusters = GetParameters().mp_clusters;
    //select first clusters
    Utilities::ClearClusters(*clusters);

    TImageInformations informations;
    for (size_t i = 0; i < points.size(); ++i)
      informations.push_back(ImageInformation(points[i], i+1));

    size_t first_cluster_index = Utilities::GetFirstClusterIndex(GetParameters());

    clusters->push_back(new Cluster(AlgorithmsInstance->GenerateNextName()));
    clusters->back()->LinkPoint(GetParameters().m_points[first_cluster_index]);

    clusters->push_back(new Cluster(AlgorithmsInstance->GenerateNextName()));
    //initialize first and second clusters  
    _GetFarestCluster(points, *clusters->front(), clusters->back());

    size_t it_number = GetParameters().m_iterations == 0 ? 10000 : GetParameters().m_iterations;
    for (size_t i = 0; i < it_number; ++i)
    {
      Utilities::RemoveClassifiedPoints(*clusters, points);
      if (i_reporter && GetParameters().m_report_on_each_iteration)
        {
        if (i != 0)
          {
          i_reporter->Report(Reporters::NewLine);
          i_reporter->Report(Reporters::NewLine);
          }
        i_reporter->Report(L"Iteration");
        i_reporter->Report(Reporters::Delimiter);
        i_reporter->Report(i);
        i_reporter->Report(Reporters::NewLine);
        }
      if (!_Iterate(informations, i_reporter))
        break;
    }
    
    Utilities::RemoveClassifiedPoints(*clusters, points);
    //classify points to created clusters
    _ClassifyPoints(*clusters, points);

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