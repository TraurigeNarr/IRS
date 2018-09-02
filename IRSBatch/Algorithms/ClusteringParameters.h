#pragma once

namespace Algorithms
{
  
  struct Clustering_Parameters
  {
    enum AlgorithmFirstCLusters
    {
      AT_NONE = -1,
      AT_FROM_BEGIN,
      AT_FROM_END,
      AT_RANDOM,
    };

    enum MaxMinDistanceCriteria
      {
      MMDC_AVERAGE_DISTANCE,
      MMDC_DISTANCE_BETWEEN_LAST_TWO_CLUSTERS
      };

    // points contain points itself
    MathBase::TPointCloud m_points;
    // after operation here will be points, linked to cluster
    MathBase::TClusters* mp_clusters;
    AlgorithmFirstCLusters m_select_first_algorithm;
    //border for points that can be assign to one cluster
    double m_radius_to_append;

    size_t m_number_of_clusters;
    size_t m_iterations;
    //in K means centers should deviate less then this value
    double m_center_shift_allowed;

    bool m_report_on_each_iteration;
    MaxMinDistanceCriteria m_maxmin_distance_criteria;
    double m_maxmin_separating_value;
  };

}