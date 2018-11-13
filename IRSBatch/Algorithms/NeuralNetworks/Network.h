#pragma once

#include "IAnalyzer.h"
#include "Recognition/RecognitionParameters.h"

namespace Algorithms
{
	namespace neural_impl
	{
		class NeuralNetwork;
	}

	class NeuralNetworkAnalyzer : public IAnalyzer<Recognition_Parameters>
	{
	private:
		int m_mode;
		std::unique_ptr<neural_impl::NeuralNetwork> mp_impl;


	public:
		NeuralNetworkAnalyzer(const std::wstring& i_analyzer_name, int i_available_int);
		virtual ~NeuralNetworkAnalyzer();

		virtual bool Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr) override;
		virtual void Train(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr) override;

		virtual bool IsAvailableInMode(int i_mode) const override;
	};
}