#pragma once

#include "IAnalyzer.h"
#include "ClusteringParameters.h"

namespace Algorithms
{

  class Clustering_HeuristicBorder : public IAnalyzer<Clustering_Parameters>
  {
  private:
    int m_mode;

  public:
    Clustering_HeuristicBorder(const std::wstring i_analyzer_name, int i_available_int);
    virtual ~Clustering_HeuristicBorder();

    virtual bool Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr) override;
    virtual bool IsAvailableInMode(int i_mode) const override;
  };

}