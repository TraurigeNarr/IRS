#include "stdafx.h"

#include "AlgorithmManager.h"
#include "AlgorithmsUtilities.h"
#include "Reporter.h"

#include <MathBase/Cluster.h>
#include <MathBase/ClassHandler.h>

#include <algorithm>
#include <sstream>

namespace Algorithms
{
  
  namespace Utilities
  {

    void ClearClusters(MathBase::TClusters& io_clusters)
    {
      std::for_each (io_clusters.begin(), io_clusters.end(), [](Cluster* ip_cluster)
      {
        ip_cluster->ClearLinkedPoints();
      });
    }

    int RandomNumber (int i) 
    { 
      return std::rand()%i;
    }

	float Random(float a, float b)
	{
		return a + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (b - a)));
	}

    size_t GetFirstClusterIndex(Clustering_Parameters& io_parameters)
    {
      using namespace Algorithms;

      size_t first_cluster_index =  0;
      switch (io_parameters.m_select_first_algorithm)
      {
      case Clustering_Parameters::AT_FROM_BEGIN:
        first_cluster_index = 0;
        break;
      case Clustering_Parameters::AT_FROM_END:
        first_cluster_index = io_parameters.m_points.size() - 1;
        break;
      case Clustering_Parameters::AT_RANDOM:
        first_cluster_index = io_parameters.m_points.size() != 1 ? Utilities::RandomNumber(io_parameters.m_points.size() - 1) : 0;
        break;
      }

      return first_cluster_index;
    }

    void SelectFirstClusters_FromBegin(Clustering_Parameters& io_parameters)
    {
      io_parameters.mp_clusters->clear();

      size_t number = io_parameters.m_number_of_clusters > io_parameters.m_points.size() ? io_parameters.m_points.size() : io_parameters.m_number_of_clusters;

      for (size_t i = 0; i < number; ++i)
      {
        io_parameters.mp_clusters->push_back(new Cluster(AlgorithmsInstance->GenerateNextName()));
        io_parameters.mp_clusters->back()->LinkPoint(io_parameters.m_points[i]);
      }
    }

    void SelectFirstClusters_FromEnd(Clustering_Parameters& io_parameters)
    {
      io_parameters.mp_clusters->clear();

      size_t point_number = io_parameters.m_points.size();
      size_t cluster_number = io_parameters.m_number_of_clusters;

      size_t number = cluster_number > point_number ? point_number : cluster_number;

      for (size_t i = point_number - 1; i >= point_number - cluster_number; --i)
      {
        io_parameters.mp_clusters->push_back(new Cluster(AlgorithmsInstance->GenerateNextName()));
        io_parameters.mp_clusters->back()->LinkPoint(io_parameters.m_points[i]);
      }
    }

    void SelectFirstClusters_Random(Clustering_Parameters& io_parameters)
    {
      io_parameters.mp_clusters->clear();

      size_t number = io_parameters.m_number_of_clusters > io_parameters.m_points.size() ? io_parameters.m_points.size() : io_parameters.m_number_of_clusters;

      //generate random numbers
      size_t *random_numbers = new size_t[io_parameters.m_points.size()];
      for (size_t i = 0; i < io_parameters.m_points.size(); ++i)
        random_numbers[i] = i;
      std::random_shuffle(random_numbers, random_numbers+io_parameters.m_points.size());
      //get random points
      for (size_t i = 0; i < number; ++i)
      {
        io_parameters.mp_clusters->push_back(new Cluster(AlgorithmsInstance->GenerateNextName()));
        size_t random = random_numbers[i];
        io_parameters.mp_clusters->back()->LinkPoint(io_parameters.m_points[random]);
      }

      delete[] random_numbers;
    }

    void PrepareParameters(Clustering_Parameters& io_parameters)
    {
      switch (io_parameters.m_select_first_algorithm)
      {
      case Clustering_Parameters::AT_FROM_BEGIN:
        SelectFirstClusters_FromBegin(io_parameters);
        break;
      case Clustering_Parameters::AT_FROM_END:
        SelectFirstClusters_FromEnd(io_parameters);
        break;
      case Clustering_Parameters::AT_RANDOM:
        SelectFirstClusters_Random(io_parameters);
        break;
      }
    }

    void RememberCenterPoints(const MathBase::TClusters& i_clusters, MathBase::TPointCloud& o_centers)
    {
      for (size_t i = 0; i < i_clusters.size(); ++i)
        o_centers.push_back(new Vector3D(i_clusters[i]->GetCenter()));
      for (size_t i = 0; i < o_centers.size(); ++i)
        delete o_centers[i];
      o_centers.clear();

      for (size_t i = 0; i < i_clusters.size(); ++i)
      {
        o_centers.push_back(new Vector3D(i_clusters[i]->GetCenter()));
        i_clusters[i]->ClearLinkedPoints();
      }
    }

    //returns true if equal
    bool CompareCenterPoints(const MathBase::TClusters& i_clusters, const MathBase::TPointCloud& i_centers, double i_border_distance /* = 0.0 */)
    {
      if (i_centers.size() != i_clusters.size())
        return false;
      i_border_distance *= i_border_distance;
      for (size_t i = 0; i < i_clusters.size(); ++i)
      {
        Vector3D new_center = i_clusters[i]->GetCenter();
        if (new_center.DistanceSq(*i_centers[i]) > i_border_distance)
          return false;
      }
      return true;
    }

    void RemoveClassifiedPoints(const MathBase::TClusters& i_clusters, MathBase::TPointCloud& i_points)
    {
      using namespace MathBase;
      for (size_t i = 0; i < i_clusters.size(); ++i)
      {
        TPointCloud::iterator points_end = i_points.end();
        TPointCloud::iterator it_point = i_points.begin();

        for ( ; it_point != points_end; ++it_point)
        {
          if (i_clusters[i]->Contains(*it_point))
          {
            i_points.erase(it_point);
            it_point = i_points.begin();
            points_end = i_points.end();
            if (i_points.size() == 0)
              return;
          }
        }
      }
    }

    void GenerateReport(std::shared_ptr<Reporters::Reporter> oh_reporter, MathBase::TClasses& i_classes)
    {
      MathBase::TClasses::iterator cur_cluster = i_classes.begin();
      MathBase::TClasses::iterator it_end = i_classes.end();
      std::wstringstream stream;
      stream.precision(oh_reporter->Precision());
      for ( ; cur_cluster != it_end; ++cur_cluster)
      {
        ClassHandler* ip_class = *cur_cluster;

        oh_reporter->Report(L"Class ");
        oh_reporter->Report(ip_class->GetName());
        oh_reporter->Report(Reporters::NewLine);

        oh_reporter->Report(L"Decisive function");
        oh_reporter->Report(Reporters::Delimiter);
        stream.str(L"");
        ip_class->GetDivisionFunction()->Write(stream);
        oh_reporter->Report(stream.str());
        oh_reporter->Report(Reporters::NewLine);

        oh_reporter->Report(L"Etalons");
        oh_reporter->Report(Reporters::NewLine);
        for (size_t i = 0; i < ip_class->GetCorePoints().size(); ++i)
        {
          oh_reporter->Report(Reporters::Offset);
          oh_reporter->Report(*ip_class->GetCorePoints()[i]);
          oh_reporter->Report(Reporters::NewLine);
        }
        
        if (ip_class->GetCorePoints().size() > 1)
        {
          oh_reporter->Report(L"Average etalon");
          oh_reporter->Report(Reporters::Delimiter);
          oh_reporter->Report(ip_class->GetCenter());
          oh_reporter->Report(Reporters::NewLine);
        }

        oh_reporter->Report(L"Linked image number");
        oh_reporter->Report(Reporters::Delimiter);
        oh_reporter->Report(ip_class->GetLinkedPoints().size());
        oh_reporter->Report(Reporters::NewLine);

        oh_reporter->Report(L"Average distance between center and images");
        oh_reporter->Report(Reporters::Delimiter);
        oh_reporter->Report(ip_class->AverageDistanceBetweenPointsAndCenter());
        oh_reporter->Report(Reporters::NewLine);

        double max_dist = 0.0,
               min_dist = 0.0;
        ip_class->MaxMinLinkedPointDistance(min_dist, max_dist);

        oh_reporter->Report(L"Max distance to linked point");
        oh_reporter->Report(Reporters::Delimiter);
        oh_reporter->Report(max_dist);
        oh_reporter->Report(Reporters::NewLine);
        
        oh_reporter->Report(L"Min distance to linked point");
        oh_reporter->Report(Reporters::Delimiter);
        oh_reporter->Report(min_dist);
        oh_reporter->Report(Reporters::NewLine);

        oh_reporter->Report(L"Distance to classes");
        oh_reporter->Report(Reporters::NewLine);

        Vector3D center = ip_class->GetCenter();
        for (MathBase::TClasses::iterator it = i_classes.begin(); it != it_end; ++it)
        {
          if (it == cur_cluster)
            continue;
          oh_reporter->Report(Reporters::Offset);
          oh_reporter->Report(L"Distance to ");
          oh_reporter->Report((**it).GetName());
          oh_reporter->Report(Reporters::Delimiter);
          oh_reporter->Report(center.DistanceSq((**it).GetCenter()));
          oh_reporter->Report(Reporters::NewLine);
          oh_reporter->Report(L"Division function with ");
          oh_reporter->Report((**it).GetName());
          oh_reporter->Report(Reporters::Delimiter);
          stream.str(L"");
          (ip_class->GetDivisionFunction()->Minus((**it).GetDivisionFunction()))->Write(stream);
          oh_reporter->Report(stream.str());
          oh_reporter->Report(Reporters::NewLine);
        }
        oh_reporter->Report(Reporters::NewLine);
      }
    }
    
    void GenerateReport(std::shared_ptr<Reporters::Reporter> oh_reporter, MathBase::TClusters& i_clusters)
    {
    MathBase::TClusters::iterator cur_cluster = i_clusters.begin();
    MathBase::TClusters::iterator it_end = i_clusters.end();
    std::wstringstream stream;
    stream.precision(oh_reporter->Precision());
    for ( ; cur_cluster != it_end; ++cur_cluster)
      {
      Cluster* ip_cluster = *cur_cluster;

      oh_reporter->Report(L"Cluster ");
      oh_reporter->Report(ip_cluster->GetName());
      oh_reporter->Report(Reporters::NewLine);

      oh_reporter->Report(L"Cluster center");
      oh_reporter->Report(Reporters::NewLine);
      oh_reporter->Report(Reporters::Offset);
      oh_reporter->Report(ip_cluster->GetCenter());
      oh_reporter->Report(Reporters::NewLine);

      oh_reporter->Report(L"Deviation");
      oh_reporter->Report(Reporters::Delimiter);
      oh_reporter->Report(ip_cluster->GetDeviation());
      oh_reporter->Report(Reporters::NewLine);

      oh_reporter->Report(L"Dispersion");
      oh_reporter->Report(Reporters::Delimiter);
      oh_reporter->Report(ip_cluster->GetDispersion());
      oh_reporter->Report(Reporters::NewLine);

      oh_reporter->Report(L"Linked image number");
      oh_reporter->Report(Reporters::Delimiter);
      oh_reporter->Report(ip_cluster->GetLinkedPoints().size());
      oh_reporter->Report(Reporters::NewLine);

      oh_reporter->Report(L"Images in cluster");
      oh_reporter->Report(Reporters::NewLine);
      for (size_t i = 0; i < ip_cluster->GetLinkedPoints().size(); ++i)
        {
        oh_reporter->Report(Reporters::Offset);
        oh_reporter->Report(*ip_cluster->GetLinkedPoints()[i]);
        oh_reporter->Report(Reporters::NewLine);
        }

      oh_reporter->Report(L"Average distance between center and images");
      oh_reporter->Report(Reporters::Delimiter);
      oh_reporter->Report(ip_cluster->AverageDistanceBetweenPointsAndCenter());
      oh_reporter->Report(Reporters::NewLine);

      double max_dist = 0.0,
        min_dist = 0.0;
      ip_cluster->MaxMinLinkedPointDistance(min_dist, max_dist);

      oh_reporter->Report(L"Max distance to linked point");
      oh_reporter->Report(Reporters::Delimiter);
      oh_reporter->Report(max_dist);
      oh_reporter->Report(Reporters::NewLine);

      oh_reporter->Report(L"Min distance to linked point");
      oh_reporter->Report(Reporters::Delimiter);
      oh_reporter->Report(min_dist);
      oh_reporter->Report(Reporters::NewLine);

      oh_reporter->Report(L"Distance to classes");
      oh_reporter->Report(Reporters::NewLine);

      Vector3D center = ip_cluster->GetCenter();
      for (MathBase::TClusters::iterator it = i_clusters.begin(); it != it_end; ++it)
        {
        if (it == cur_cluster)
          continue;
        oh_reporter->Report(Reporters::Offset);
        oh_reporter->Report(L"Distance to ");
        oh_reporter->Report((**it).GetName());
        oh_reporter->Report(Reporters::Delimiter);
        oh_reporter->Report(center.DistanceSq((**it).GetCenter()));
        oh_reporter->Report(Reporters::NewLine);
        }

      oh_reporter->Report(Reporters::NewLine);
      }
    }

  } // Utilities

} // Algorithms