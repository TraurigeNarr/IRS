#include "stdafx.h"

#include "Recognition_KNN.h"

namespace Algorithms
{

	Recognition_KNN::Recognition_KNN(const std::wstring i_analyzer_name, int i_available_int)
		: IAnalyzer<Recognition_Parameters>(i_analyzer_name)
		, m_mode(i_available_int)
	{
	}

	Recognition_KNN::~Recognition_KNN()
	{

	}


	bool Recognition_KNN::IsAvailableInMode(int i_mode) const
	{
		return i_mode == m_mode;
	}

	bool Recognition_KNN::Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter /*= nullptr*/)
	{
		return true;
	}

} // Algorithms