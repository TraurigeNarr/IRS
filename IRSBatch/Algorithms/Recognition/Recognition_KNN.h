#pragma once

#include "IAnalyzer.h"
#include "RecognitionParameters.h"

namespace Algorithms
{
	class Recognition_KNN : public IAnalyzer<Recognition_Parameters>
	{
	private:
		int m_mode;

	public:
		Recognition_KNN(const std::wstring i_analyzer_name, int i_available_int);
		virtual ~Recognition_KNN();

		virtual bool Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr) override;
		virtual bool IsAvailableInMode(int i_mode) const override;
	};
}