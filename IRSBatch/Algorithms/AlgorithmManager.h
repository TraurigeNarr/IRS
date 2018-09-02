#pragma once

#include "IAnalyzer.h"

#include <functional>

class AlgorithmManager
{
public:
  typedef std::shared_ptr<IAnalyzerBase> TAnalyzerImpl;
  typedef std::map<std::wstring, TAnalyzerImpl> TAnalyzerMap;
private:
  TAnalyzerImpl mp_current_analyzer;

  TAnalyzerMap m_analyzers;

  typedef std::function<std::wstring()> TNameGenerator;
  TNameGenerator m_name_generator;

private:
  AlgorithmManager();
  ~AlgorithmManager();

  AlgorithmManager(const AlgorithmManager& i_other);
  AlgorithmManager& operator=(const AlgorithmManager& i_other);
public:
  static AlgorithmManager* GetInstance();

  template <typename Params>
  bool Analyze(Params& i_parameters, std::shared_ptr<Algorithms::Reporters::Reporter> ih_reporter = nullptr);

  bool SetCurrentAnalyzer(const std::wstring& i_analyzer_name, int i_mode);
  void AddAnalyzer(TAnalyzerImpl ip_analyzer);

  std::vector<std::wstring> GetAnalyzerNames(int i_mode) const;

  void SetNameGenerator(TNameGenerator i_generator);
  std::wstring GenerateNextName() const;
};

template <typename Params>
bool AlgorithmManager::Analyze(Params& i_parameters, std::shared_ptr<Algorithms::Reporters::Reporter> ih_reporter)
{
  if (mp_current_analyzer.get() == nullptr)
    return false;

  IAnalyzer<Params>* p_analyzer_impl = dynamic_cast<IAnalyzer<Params>*>(mp_current_analyzer.get());
  if (nullptr == p_analyzer_impl)
    return false;

  p_analyzer_impl->SetParameters(i_parameters);

  return p_analyzer_impl->Analyze(ih_reporter);
}

#define AlgorithmsInstance AlgorithmManager::GetInstance()