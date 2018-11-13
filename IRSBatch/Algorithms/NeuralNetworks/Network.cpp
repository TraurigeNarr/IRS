#include "stdafx.h"

#include "Network.h"

#include "AlgorithmsUtilities.h"

#include <Reporter.h>

namespace Algorithms
{

	namespace neural_impl
	{

		float sigmoid(float z)
		{
			return 1.0f / (1.0f + std::exp(-z));
		}

		class NeuralNetwork
		{
		public:

			void Clear()
			{
				_biases.clear();
				_weights.clear();
				_sizes.clear();
			}

			void Create(const std::vector<size_t>& layers)
			{
				if (layers.size() < 2u) {
					return;
				}

				Clear();

				_sizes = layers;

				_biases.resize(layers.size() - 1);
				_weights.resize(layers.size() - 1);
				for (size_t i = 0; i < layers.size() - 1; ++i)
				{
					_biases[i].resize(layers[i + 1]);
					for (size_t j = 0; j < layers[i + 1]; ++j) {
						_biases[i][j] = Utilities::Random(-1.0f, 1.0f);
					}
				}

				for (size_t i = 0; i < layers.size() - 1; ++i)
				{
					const size_t first_size = layers[i + 1];
					const size_t second_size = layers[i];
					_weights[i].resize(first_size);
					auto& layer_to_layer = _weights[i];
					for (size_t j = 0; j < first_size; ++j) {
						layer_to_layer[j].resize(second_size);
						for (size_t k = 0; k < second_size; ++k) {
							layer_to_layer[j][k] = Utilities::Random(-1.0f, 1.0f);
						}
					}
				}
			}

			std::vector<float> Evaluate(const std::vector<float>& input)
			{
				if (input.size() != _sizes[0]) {
					return std::vector<float>{};
				}

				std::vector<float> result = input;

				for (size_t layer = 0; layer < _weights.size(); ++layer) {
					auto& layer_weights = _weights[layer];

					if (result.size() != layer_weights[0].size()) {
						// error
						return result;
					}

					std::vector<float> layer_result;
					layer_result.resize(layer_weights.size());
					for (size_t i = 0; i < layer_result.size(); ++i) {
						layer_result[i] = 0.0f;
						auto& ith_neuron_w = layer_weights[i];
						for (size_t j = 0; j < result.size(); ++j) {
							layer_result[i] += result[j] * ith_neuron_w[j];
						}
					}

					result = layer_result;
				}

				return result;
			}

		private:
			using Layer = std::vector<float>;
			using Biases = std::vector<Layer>;
			struct Link
			{
				size_t to;
				float weight;
			};

			using Links = std::vector<Link>;

			using Floats = std::vector<float>;
			using LayerToLayerWeights = std::vector<Floats>;
			using Weights = std::vector<LayerToLayerWeights>;
			Biases _biases;
			Weights _weights;

			std::vector<size_t> _sizes;
		};

	} // neural_impl

	NeuralNetworkAnalyzer::NeuralNetworkAnalyzer(const std::wstring& i_analyzer_name, int i_available_int)
		: IAnalyzer<Recognition_Parameters>(i_analyzer_name)
		, m_mode(i_available_int)
		, mp_impl(new neural_impl::NeuralNetwork{})
	{
	}

	NeuralNetworkAnalyzer::~NeuralNetworkAnalyzer()
	{
	}

	bool NeuralNetworkAnalyzer::IsAvailableInMode(int i_mode) const
	{
		return i_mode == m_mode;
	}

	bool NeuralNetworkAnalyzer::Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter)
	{
		mp_impl->Create({ 2, 3, 5, 10 });
		auto vec = mp_impl->Evaluate({ 0.2f, 1.0f });
		i_reporter->Report(Reporters::NewLine);
		int max_ind = 0;
		float max_val = -100000000000.0f;
		for (int i = 0; i < vec.size(); ++i) {
			i_reporter->ReportMultiple(vec[i], " ");
			if (vec[i] > max_val) {
				max_val = vec[i];
				max_ind = i;
			}
		}
		i_reporter->ReportMultiple(Reporters::NewLine, L"=========================", Reporters::NewLine);
		i_reporter->ReportMultiple(L"Max ind: ", max_ind);

		return true;
	}

	void NeuralNetworkAnalyzer::Train(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter)
	{

	}

} // Algorithms