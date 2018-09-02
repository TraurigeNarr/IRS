#pragma once

#include "IAnalyzer.h"
#include "ClusteringParameters.h"

namespace Algorithms
{

  class CLustering_Maxmin : public IAnalyzer<Clustering_Parameters>
  {
  public:
    struct ImageInformation
      {
      Vector3D* mp_image;
      size_t m_number;

      ImageInformation(Vector3D* ip_image, size_t i_number);
      };
    
    typedef std::vector<ImageInformation> TImageInformations;

  private:
    int m_mode;

  private:
    bool _Iterate(const TImageInformations& i_informations, std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr);
    void _GetFarestCluster(const MathBase::TPointCloud& i_points, const Cluster& i_cluster, Cluster* op_farest_cluster);
    void _ClassifyPoints(MathBase::TClusters& i_clusters, MathBase::TPointCloud& i_points);
  public:
    CLustering_Maxmin(const std::wstring i_analyzer_name, int i_available_int);
    virtual ~CLustering_Maxmin();

    virtual bool Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr) override;
    virtual bool IsAvailableInMode(int i_mode) const override;
  };

}