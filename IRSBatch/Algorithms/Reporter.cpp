#include "stdafx.h"
#include "Reporter.h"

#include <MathBase/ClassHandler.h>
#include <MathBase/Cluster.h>
#include <MathBase/IDivisionFunction.h>
#include <MathBase/math/Vector3D.h>

#include <algorithm>

#include <iomanip>      
#include <sstream>

namespace
{

  void _Calculate(std::wstring& o_state_string, MathBase::TClasses& i_clusters)
  {
    std::wstringstream stream;
    size_t number = 0;
  }

} // namespace

namespace Algorithms
{

  namespace Reporters
  { 
    Reporter::Reporter(size_t i_precision)
      : m_state(L"")
      , m_precision(i_precision)
    {
    }

    Reporter::~Reporter()
    {

    }

    std::wstring Reporter::GetState(const std::wstring& i_new_line_pattern /*= L"\n"*/, const std::wstring& i_offset_pattern /*= L"  "*/, const std::wstring i_delimiter /*= L":"*/) const
    {
      std::wstring state = L"";
      for (size_t i = 0; i < m_state.size(); ++i)
      {
        if (m_state[i] == Delimiter)
          state += i_delimiter;
        else if (m_state[i] == Offset)
          state += i_offset_pattern;
        else if (m_state[i] == NewLine)
          state += i_new_line_pattern;
        else
          state += m_state[i];
      }
      return std::move(state);
    }

    bool Reporter::IsEmpty() const
    {
      return m_state.empty();
    }

    void Reporter::Flush()
    {
      m_state.clear();
    }

    size_t Reporter::Precision() const
    {
      return m_precision;
    }

    void Reporter::Precision(size_t i_new_precision)
    {
      m_precision = i_new_precision;
    }

  } // Reporters
} // Algorithms