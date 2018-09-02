#include "stdafx.h"

#include "AlgorithmManager.h"
#include "IAnalyzer.h"

AlgorithmManager::AlgorithmManager()
  : mp_current_analyzer(nullptr)
{
}

AlgorithmManager::~AlgorithmManager()
{
}

AlgorithmManager* AlgorithmManager::GetInstance()
{
  static AlgorithmManager instance;
  return &instance;
}

void AlgorithmManager::AddAnalyzer(TAnalyzerImpl ip_analyzer)
{
  TAnalyzerMap::iterator it = m_analyzers.find(ip_analyzer->GetName());

   if (it == m_analyzers.end())
     m_analyzers[ip_analyzer->GetName()] = ip_analyzer;
}

bool AlgorithmManager::SetCurrentAnalyzer(const std::wstring& i_analyzer_name, int i_mode)
{
  TAnalyzerMap::iterator it = m_analyzers.find(i_analyzer_name);

  if (it != m_analyzers.end() 
    && it->second->IsAvailableInMode(i_mode))
  {
    mp_current_analyzer = it->second;
    return true;
  }

  return false;
}

std::vector<std::wstring> AlgorithmManager::GetAnalyzerNames(int i_mode) const
{
  std::vector<std::wstring> names;

  TAnalyzerMap::const_iterator it = m_analyzers.begin();
  TAnalyzerMap::const_iterator it_end = m_analyzers.end();

  for ( ; it != it_end; ++it)
  {
    if (it->second->IsAvailableInMode(i_mode))
      names.push_back(it->first);
  }

  return std::move(names);
}

void AlgorithmManager::SetNameGenerator(TNameGenerator i_generator)
  {
  m_name_generator = i_generator;
  }

std::wstring AlgorithmManager::GenerateNextName() const
  {
  if (m_name_generator)
    return m_name_generator();
  return L"";
  }