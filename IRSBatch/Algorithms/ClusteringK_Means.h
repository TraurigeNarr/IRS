#pragma once

#include "IAnalyzer.h"
#include "ClusteringParameters.h"

namespace Algorithms
{
  class ClusteringK_Means : public IAnalyzer<Clustering_Parameters>
  {
  private:
    int m_mode;

  private:
    void _Iterate();

  public:
    ClusteringK_Means(const std::wstring i_analyzer_name, int i_available_int);
    virtual ~ClusteringK_Means();

    virtual bool Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr) override;
    virtual bool IsAvailableInMode(int i_mode) const override;
  };
}