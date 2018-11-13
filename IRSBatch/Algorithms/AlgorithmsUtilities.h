#pragma once

#include "ClusteringParameters.h"

namespace Algorithms
{
  namespace Reporters
  {
    class Reporter;
  }

  namespace Utilities
  {

    // random generator function:
    int RandomNumber (int i);
	float Random(float a, float b);

    size_t GetFirstClusterIndex(Clustering_Parameters& io_parameters);

    void SelectFirstClusters_FromBegin(Clustering_Parameters& io_parameters);

    void SelectFirstClusters_FromEnd(Clustering_Parameters& io_parameters);

    void SelectFirstClusters_Random(Clustering_Parameters& io_parameters);

    void PrepareParameters(Clustering_Parameters& io_parameters);

    void RememberCenterPoints(const MathBase::TClusters& i_clusters, MathBase::TPointCloud& o_centers);

    //returns true if equal
    bool CompareCenterPoints(const MathBase::TClusters& i_clusters, const MathBase::TPointCloud& i_centers, double i_border_distance = 0.0);

    void ClearClusters(MathBase::TClusters& io_clusters);

    void RemoveClassifiedPoints(const MathBase::TClusters& i_clusters, MathBase::TPointCloud& i_points);

    void GenerateReport(std::shared_ptr<Reporters::Reporter> oh_reporter, MathBase::TClasses& i_classes);
    void GenerateReport(std::shared_ptr<Reporters::Reporter> oh_reporter, MathBase::TClusters& i_clusters);

  } // Utilities
 
} // Algorithms