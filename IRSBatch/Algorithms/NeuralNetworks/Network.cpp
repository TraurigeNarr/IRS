#include "stdafx.h"

#include "Network.h"

#include "AlgorithmsUtilities.h"

#include <Reporter.h>
#include <MathBase/math/Vector3D.h>

#include <cv_utils/cv_utils.h>

namespace Algorithms
{

	namespace neural_impl
	{
		static constexpr int RESIZED_IMAGE_WIDTH = 20;
		static constexpr int RESIZED_IMAGE_HEIGHT = 30;


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

			void Create(const std::vector<size_t>& layers, const std::vector<std::string>& values)
			{
				if (layers.size() < 2u) {
					return;
				}

				if (layers[layers.size() - 1] != values.size()) {
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

				_values = values;
			}

			std::vector<float> Evaluate(const std::vector<float>& input) const
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

			const std::string& GetValue(const std::vector<float>& evaluated) const
			{
				int max_ind = 0;
				float max_val = -100000000000.0f;
				for (int i = 0; i < evaluated.size(); ++i) {
					if (evaluated[i] > max_val) {
						max_val = evaluated[i];
						max_ind = i;
					}
				}

				return _values[max_ind];
			}

		private:
			using Layer = std::vector<float>;
			using Biases = std::vector<Layer>;

			using Floats = std::vector<float>;
			using LayerToLayerWeights = std::vector<Floats>;
			using Weights = std::vector<LayerToLayerWeights>;
			Biases _biases;
			Weights _weights;

			std::vector<size_t> _sizes;
			std::vector<std::string> _values;
		};

		std::string RecognizeData(const NeuralNetwork& network,
								  const std::vector<Vector3D>& points,
								  std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter, bool show_debug_image)
		{
			cv::Mat source_image = cv_utils::create_image(points);
			bool success = cv::imwrite("test.png", source_image);
			if (!success) {
				return std::string{};
			}
			source_image = cv::imread("test.png");
			cv::Mat prepared = cv_utils::prepare_image(source_image);

			std::vector<cv_utils::ContourWithData> validContoursWithData = cv_utils::find_contours(prepared);

			cv::Scalar colors[7] = {
				cv::Scalar(255, 0, 0),
				cv::Scalar(0, 255, 0),
				cv::Scalar(0, 0, 255),
				cv::Scalar(255, 255, 0),
				cv::Scalar(255, 0, 255),
				cv::Scalar(0, 255, 255),
				cv::Scalar(255, 255, 255),
			};

			std::string resulted_string = "";

			for (int i = 0; i < validContoursWithData.size(); i++) {
				cv::rectangle(source_image, validContoursWithData[i].boundingRect, colors[i % 6], 2);
				cv::Mat matROI = prepared(validContoursWithData[i].boundingRect);

				cv::Mat matROIResized;
				cv::resize(matROI, matROIResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));

				cv::Mat matROIFloat;
				matROIResized.convertTo(matROIFloat, CV_32FC1);

				cv::Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);

				cv::Mat matCurrentChar(0, 0, CV_32F);

				std::vector<float> input;
				input.resize(RESIZED_IMAGE_WIDTH * RESIZED_IMAGE_HEIGHT);
				input = matROIFlattenedFloat;
				auto result = network.Evaluate(input);
				resulted_string += network.GetValue(result);
			}

			return resulted_string;
		}

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
		const size_t input_size = neural_impl::RESIZED_IMAGE_WIDTH * neural_impl::RESIZED_IMAGE_HEIGHT;
		const size_t output_size = 'Z' - 'A' + 1;
		std::vector<std::string> output;
		output.resize(output_size);
		for (size_t i = 0; i < output_size; ++i) {
			output[i] = char('A' + i);
		}

		mp_impl->Create({ input_size, 15, output_size }, output);

		Recognition_Parameters& params = GetParameters();
		
		const std::string result = neural_impl::RecognizeData(*mp_impl, params.m_points, i_reporter, false);

		if (i_reporter)
		{
			i_reporter->ReportMultiple(L"Result: ", result.c_str(), Reporters::NewLine);
			i_reporter->ReportMultiple(L"End recognition", Reporters::NewLine, L"==========================", Reporters::NewLine);
		}

		return true;
	}

	void NeuralNetworkAnalyzer::Train(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter)
	{

	}

} // Algorithms