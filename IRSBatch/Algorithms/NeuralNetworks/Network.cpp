#include "stdafx.h"

#include "Network.h"

#include "AlgorithmsUtilities.h"

#include <Reporter.h>
#include <MathBase/math/Vector3D.h>

#include <cv_utils/cv_utils.h>

#include <functional>

namespace Algorithms
{

	namespace neural_impl
	{
		static constexpr int RESIZED_IMAGE_WIDTH = 20;
		static constexpr int RESIZED_IMAGE_HEIGHT = 30;

		namespace neural_from_python
		{

			float sigmoid(float z)
			{
				return 1.0f / (1.0f + std::exp(-z));
			}

			std::vector<float> sigmoid(const std::vector<float>& z)
			{
				std::vector<float> sig;
				sig.resize(z.size());
				for (size_t i = 0; i < sig.size(); ++i) {
					sig[i] = sigmoid(z[i]);
				}
				return sig;
			}

			float sigmoid_prime(float z)
			{
				const float sig = sigmoid(z);
				return sig * (1 - sig);
			}

			std::vector<float> sigmoid_prime(const std::vector<float>& z)
			{
				std::vector<float> sig;
				sig.resize(z.size());
				for (size_t i = 0; i < z.size(); ++i)
				{
					sig[i] = sigmoid_prime(z[i]);
				}
				return sig;
			}

			void zeros(std::vector<float>& out) {
				for (size_t i = 0; i < out.size(); ++i) {
					out[i] = 0.0f;
				}
			}

			std::vector<float> operator + (const std::vector<float>& l, const std::vector<float>& r)
			{
				if (l.size() != r.size())
					throw std::exception();
				std::vector<float> result = l;
				for (size_t i = 0; i < r.size(); ++i) {
					result[i] += r[i];
				}
				return result;
			}

			std::vector<float> operator * (const std::vector<float>& l, const std::vector<float>& r)
			{
				if (l.size() != r.size())
					throw std::exception();
				std::vector<float> result = l;
				for (size_t i = 0; i < r.size(); ++i) {
					result[i] *= r[i];
				}
				return result;
			}

			std::vector<float> dot(const std::vector<std::vector<float>>& w, const std::vector<float>& v)
			{
				std::vector<float> result;
				result.resize(w.size());
				for (size_t i = 0; i < w.size(); ++i) {
					if (v.size() != w[i].size()) {
						throw std::exception();
					}
					result[i] = 0.0f;
					for (size_t j = 0; j < w[i].size(); ++j) {
						result[i] += w[i][j] * v[j];
					}
				}

				return result;
			}

			struct TrainingData
			{
				std::vector<float> input;
				std::vector<float> output;
			};

			struct TrainingBatch
			{
				const std::vector<TrainingData>& data;
				size_t begin;
				size_t end;

				TrainingBatch(const std::vector<TrainingData>& data_, size_t begin_, size_t end_)
					: data(data_)
					, begin(begin_)
					, end(end_)
				{}

				size_t len() const {
					return end - begin;
				}
			};

			class NeuralNetwork : public NetworkBase
			{
			public:
				using Layer = std::vector<float>;
				using Biases = std::vector<Layer>;

				using Floats = std::vector<float>;
				using Ints = std::vector<size_t>;
				using LayerToLayerWeights = std::vector<Floats>;
				using Weights = std::vector<LayerToLayerWeights>;

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

				virtual void Train(const Recognition_Parameters& params, std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter) override
				{
					if (IsEmpty()) {
						return;
					}

					TrainingData data;
					data.input.resize(_sizes[0]);
					for (size_t i = 0; i < data.input.size(); ++i) {
						data.input[i] = Utilities::Random(0, 1);
					}
					data.output.resize(_sizes[_sizes.size() - 1]);
					for (size_t i = 0; i < data.output.size(); ++i) {
						data.output[i] = 0.0f;
					}
					data.output[1] = 1.0f;
					SGD({ data }, 1, 1, 3.0f, false);
				}

				virtual std::vector<float> Evaluate(const std::vector<float>& input, std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter) const override
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

						std::vector<float> layer_result = dot(layer_weights, result);
						result = layer_result;
					}

					return result;
				}

				virtual const std::string& GetValue(const std::vector<float>& evaluated) const override
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

				virtual bool IsEmpty() const override {
					return _biases.empty();
				}

				virtual void Create(const Recognition_Parameters& params) override
				{
					const size_t input_size = neural_impl::RESIZED_IMAGE_WIDTH * neural_impl::RESIZED_IMAGE_HEIGHT;
					const size_t output_size = 'Z' - 'A' + 1;
					std::vector<std::string> output;
					output.resize(output_size);
					for (size_t i = 0; i < output_size; ++i) {
						output[i] = char('A' + i);
					}
					Create({ input_size, 15, output.size() }, output);
				}

				void SGD(const std::vector<TrainingData>& training_data,
						 int epochs,
						 int mini_batch_size,
						 float eta,
						 bool test_data = false)
				{
					TrainingBatch batch{ training_data, 0u, training_data.size() };
					update_batch(batch, eta);
				}

				int feedforward(const std::vector<float>& input) const
				{
					auto result = Evaluate(input, nullptr);
					int index = -1;
					float max = -10000000000000.0f;
					for (size_t i = 0; i < result.size(); ++i) {
						if (result[i] > max) {
							max = result[i];
							index = i;
						}
					}
					return index;
				}

				void update_batch(const TrainingBatch& batch, float eta)
				{
					Biases nabla_b = zeros_bias(_biases);
					Weights nabla_w = zeros_weights(_weights);

					for (size_t i = batch.begin; i < batch.end; ++i)
					{
						const TrainingData& data = batch.data[i];
						auto delta = backprop(data.input, data.output, eta);

						// apply delta to nabla biases
						for (size_t nb = 0; nb < nabla_b.size(); ++nb) {
							for (size_t j = 0; j < nabla_b[nb].size(); ++j) {
								nabla_b[nb][j] += delta.first[nb][j];
							}
						}
						// apply delta to nabla weights
						for (size_t nwl = 0; nwl < nabla_w.size(); ++nwl) {
							for (size_t nw = 0; nw < nabla_w[nwl].size(); ++nw) {
								for (size_t j = 0; j < nabla_w[nwl][nw].size(); ++j) {
									nabla_w[nwl][nw][j] += delta.second[nwl][nw][j];
								}
							}
						}
					}

					for (size_t nwl = 0; nwl < nabla_w.size(); ++nwl) {
						for (size_t nw = 0; nw < nabla_w[nwl].size(); ++nw) {
							for (size_t j = 0; j < nabla_w[nwl][nw].size(); ++j) {
								float w = _weights[nwl][nw][j];
								float nw = nabla_w[nwl][nw][j];
								_weights[nwl][nw][j] = w - (eta / batch.len()) * nw;
							}
						}
					}

					for (size_t nb = 0; nb < nabla_b.size(); ++nb) {
						for (size_t j = 0; j < nabla_b[nb].size(); ++j) {
							float b = _biases[nb][j];
							float nb = nabla_b[nb][j];
							_biases[nb][j] = b - (eta / batch.len()) * nb;
						}
					}
				}

				Biases zeros_bias(const Biases& b) {
					Biases nabla_b;
					nabla_b.resize(b.size());
					for (size_t i = 0; i < nabla_b.size(); ++i) {
						nabla_b[i].resize(b[i].size());
						zeros(nabla_b[i]);
					}
					return nabla_b;
				}

				NeuralNetwork::Weights zeros_weights(const NeuralNetwork::Weights& w)
				{
					Weights nabla_w;
					nabla_w.resize(w.size());
					for (size_t i = 0; i < nabla_w.size(); ++i) {
						nabla_w[i].resize(w[i].size());
						for (size_t j = 0; j < nabla_w[i].size(); ++j) {
							nabla_w[i][j].resize(w[i][j].size());
							zeros(nabla_w[i][j]);
						}
					}
					return nabla_w;
				}

				std::pair<Biases, Weights> backprop(const Floats& input, const std::vector<float>& output, float eta)
				{
					Biases nabla_b = zeros_bias(_biases);
					Weights nabla_w = zeros_weights(_weights);

					// feedforward
					Floats activation = input;
					// list to store all the activations, layer by layer
					std::vector<Floats> activations{ activation };
					// list to store all the z vectors, layer by layer
					std::vector<Floats> zs;
					for (size_t layer = 0; layer < _biases.size(); ++layer)
					{
						auto& b = _biases[layer];
						auto& w = _weights[layer];
						Floats z = dot(w, activation) + b;
						zs.push_back(z);
						activation = sigmoid(z);
						activations.push_back(activation);
					}

					// backward
					Floats expected = output;
					const int layers_size = static_cast<int>(nabla_b.size());

					Floats delta3 = cost_derivative(activations[layers_size - 1], output) * sigmoid_prime(zs[layers_size]);
					nabla_b[layers_size - 1] = delta3;


					
					for (int layer = layers_size - 1; layer >= 0; --layer)
					{

						nabla_b[layer] = delta;
						auto& weights = nabla_w[layer];
						for (size_t w = 0; w < weights.size(); ++w)
						{
							for (size_t b = 0; b < delta.size(); ++b)
							{

								weights[w][b] *= delta[b];
							}
						}
						expected = activations[layer + 1];
					}

					return std::make_pair(nabla_b, nabla_w);
				}

				std::vector<float> cost_derivative(const Floats& output_activations, const Floats& y) const {
					Floats res;
					res.resize(output_activations.size());
					for (size_t i = 0; i < res.size(); ++i) {
						res[i] = output_activations[i] - y[i];
					}
					return res;
				}

			private:
				Biases _biases;
				Weights _weights;

				std::vector<size_t> _sizes;
				std::vector<std::string> _values;
			};
		}

		
		std::string RecognizeData(const NetworkBase& network,
								  const Recognition_Parameters& params,
								  std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter, bool show_debug_image)
		{
			auto& points = params.m_points;
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
				std::vector<float> res = network.Evaluate(input, i_reporter);
				resulted_string += network.GetValue(res);
			}

			return resulted_string;
		}

	} // neural_impl

	NeuralNetworkAnalyzer::NeuralNetworkAnalyzer(const std::wstring& i_analyzer_name, int i_available_int)
		: IAnalyzer<Recognition_Parameters>(i_analyzer_name)
		, m_mode(i_available_int)
		, mp_impl(new neural_impl::neural_from_python::NeuralNetwork{})
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
		if (mp_impl->IsEmpty()) {
			i_reporter->ReportMultiple(L"Network not trained", Reporters::NewLine);
			Train(i_reporter);
		}

		Recognition_Parameters& params = GetParameters();
		
		const std::string result = neural_impl::RecognizeData(*mp_impl, params, i_reporter, false);

		if (i_reporter)
		{
			i_reporter->ReportMultiple(L"Result: ", result.c_str(), Reporters::NewLine);
			i_reporter->ReportMultiple(L"End recognition", Reporters::NewLine, L"==========================", Reporters::NewLine);
		}

		return true;
	}

	void NeuralNetworkAnalyzer::Train(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter)
	{
		i_reporter->ReportMultiple("Training...", Reporters::NewLine);
		
		Recognition_Parameters p;
		mp_impl->Create(p);
		mp_impl->Train(p, i_reporter);

		i_reporter->ReportMultiple("Training done", Reporters::NewLine, L"===============================", Reporters::NewLine);
	}

} // Algorithms