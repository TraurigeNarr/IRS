#pragma once

namespace Algorithms
{
  namespace Reporters
  {
    class Reporter;
  }
}

class IAnalyzerBase
{
private:
  std::wstring m_mode_name;
public:
  IAnalyzerBase(const std::wstring i_mode_name)
    : m_mode_name(i_mode_name)
  {  }
  virtual ~IAnalyzerBase(){}

  void AddMode(int i_mode);

  //void SetParameters(ParametersBase& i_parameters);

  std::wstring GetName() const { return m_mode_name; }

  virtual bool Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr) = 0;
  virtual void Train(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr) {}
  virtual bool IsAvailableInMode(int i_mode) const = 0;
};

template <typename ParametersType>
class IAnalyzer : public IAnalyzerBase
{
protected:
  ParametersType* mp_parameters;
public:
  IAnalyzer(const std::wstring i_mode_name)
    : IAnalyzerBase(i_mode_name)
  {  }

  ParametersType& GetParameters();
  void SetParameters(ParametersType& i_parameters);

  virtual ~IAnalyzer(){}
};

template <typename ParametersType>
void IAnalyzer<ParametersType>::SetParameters(ParametersType& i_parameters)
{
  mp_parameters = &i_parameters;
}

template <typename ParametersType>
ParametersType& IAnalyzer<ParametersType>::GetParameters()
{
  return *mp_parameters;
}