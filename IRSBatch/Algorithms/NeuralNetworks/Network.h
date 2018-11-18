#pragma once

#include "IAnalyzer.h"
#include "Recognition/RecognitionParameters.h"

namespace Algorithms
{
	namespace neural_impl
	{
		class NetworkBase
		{
		public:
			virtual ~NetworkBase() {}

			virtual std::vector<float> Evaluate(const std::vector<float>& input, std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter) const = 0;
			virtual const std::string& GetValue(const std::vector<float>& output) const = 0;
			virtual void Train(const Recognition_Parameters& params, std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter) = 0;
			virtual bool IsEmpty() const = 0;
			virtual void Create(const Recognition_Parameters& params) = 0;
		};
	}

	class NeuralNetworkAnalyzer : public IAnalyzer<Recognition_Parameters>
	{
	private:
		int m_mode;
		std::unique_ptr<neural_impl::NetworkBase> mp_impl;


	public:
		NeuralNetworkAnalyzer(const std::wstring& i_analyzer_name, int i_available_int);
		virtual ~NeuralNetworkAnalyzer();

		virtual bool Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr) override;
		virtual void Train(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter = nullptr) override;

		virtual bool IsAvailableInMode(int i_mode) const override;
	};
}