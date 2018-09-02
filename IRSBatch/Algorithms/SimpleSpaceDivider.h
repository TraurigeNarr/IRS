#pragma once

#include "IAnalyzer.h"

namespace Algorithms
{
  struct SimpleSpaceDividerParameters
  {
    // points contain points itself
    MathBase::TPointCloud m_points;
    MathBase::TClasses m_clusters;
  };

  class SimpleSpaceDivider : public IAnalyzer<SimpleSpaceDividerParameters>
  {
    int m_mode;
  public:
    SimpleSpaceDivider(const std::wstring i_analyzer_name, int i_available_mode);
    virtual ~SimpleSpaceDivider();

    virtual bool Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr) override;
    virtual bool IsAvailableInMode(int i_mode) const override;
  };

}