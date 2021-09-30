// 
// Copyright 2021 Clemens Cords
// Created on 24.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector.hpp>
#include <iostream>
#include <random>

namespace crisp
{
    template<size_t... LayerN>
    struct NeuralNetwork
    {
        const static inline std::vector<size_t> _layer_to_n = {LayerN...};

        template<size_t n>
        static constexpr size_t get_layer_n() {return std::get<n>(std::make_tuple(LayerN...));};

        using Matrix = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;

        std::array<Matrix, sizeof...(LayerN)> _weights;  // x = neuron index, y = weight index
        std::array<Matrix, sizeof...(LayerN)> _bias;     // x = neuron index, y = 0

        NeuralNetwork()
        {
            _weights.at(0).resize(get_layer_n<0>(), 1);
            _weights.at(0).setConstant(1);
            _bias.at(0).resize(get_layer_n<0>(), 1);
            _bias.at(0).setConstant(0);

            for (size_t layer_i = 1; layer_i < _layer_to_n.size(); ++layer_i)
            {
                _weights.at(layer_i).resize(_layer_to_n.at(layer_i), _layer_to_n.at(layer_i - 1));
                _bias.at(layer_i).resize(_layer_to_n.at(layer_i), 1);

                for (size_t node_i = 0; node_i < _layer_to_n.at(layer_i); ++node_i)
                {
                    for (size_t weight_i = 0; weight_i < _layer_to_n.at(layer_i); weight_i++)
                        _weights.at(layer_i)(node_i, weight_i) = 0;

                    _bias.at(layer_i)(node_i, 0) = 0;
                }
            }
        }

        using OutputVector_t = Vector<float, get_layer_n<sizeof...(LayerN)-1>()>;
        using InputVector_t = Vector<float, get_layer_n<0>()>;

        Matrix sigmoid(const Matrix& in)
        {
            auto out = in;
            for (size_t i = 0; i < out.rows(); ++i)
                for (size_t j = 0; j < out.cols(); ++j)
                    out(i, j) = 1 / (1 + expf(-1 * out(i, j)));

            return std::move(out);
        }

        void override_weight(size_t layer_i, size_t neuron_i, size_t weight_i, float new_weight)
        {
            _weights.at(layer_i)(neuron_i, weight_i) = new_weight;
        }

        void override_bias(size_t layer_i, size_t neuron_i, float new_bias)
        {
            _bias.at(layer_i)(neuron_i, 0) = new_bias;
        }

        OutputVector_t forward_pass(const InputVector_t& input)
        {
            Matrix in;
            in.resize(InputVector_t::size(), 1);

            for (size_t i = 0; i < input.size(); ++i)
                in(i, 0) = input.at(i);

            std::vector<Matrix> responses;
            responses.push_back(in);

            for (size_t i = 1; i < _layer_to_n.size(); ++i)
            {
                std::cout << "weights: " << std::endl << _weights.at(i) << std::endl;
                responses.push_back(sigmoid(_weights.at(i) * responses.back() + _bias.at(i)));
                std::cout << "respons: " << std::endl << responses.back() << std::endl;
            }

            OutputVector_t out;
            for (size_t i = 0; i < OutputVector_t::size(); ++i)
                out.at(i) = responses.back()(0, i);

            return out;
        }
    };
    /*
    // first layer: number of elements in feature vector
    // hidden layers: arbitrary
    // last layer: number of observed classes
    template<size_t... LayerN>
    struct NeuralNetwork
    {
        static_assert(sizeof...(LayerN) >= 3, "Specify at least 3 layers");

        private:
            const static inline std::vector<size_t> _layer_to_n = {LayerN...};

            struct Neuron
            {
                Neuron(size_t layer_i, size_t previous_layer_n)
                    : _layer_i(layer_i)
                {
                    _weights.clear();

                    if (layer_i == 0)
                    {
                        _weights.resize(1, 0);
                        _bias = 0;
                    }
                    else
                    {
                        _weights.resize(previous_layer_n, 1);
                        _bias = 1;
                    }
                }

                float activate(const std::vector<float>& previous_layer)
                {
                    float sum = 0;
                    for (size_t i = 0; i < previous_layer.size(); ++i)
                        sum += _weights.at(i) * previous_layer.at(i);

                    sum += _bias;
                    return (1 / (1 + expf(-1 * sum)));
                }

                //private:
                    std::vector<float> _weights;
                    float _bias;
                    size_t _layer_i;
            };

            std::vector<std::vector<Neuron>> _layers;

        public:
            NeuralNetwork(float learning_rate)
            {
                alpha = learning_rate;
                for (size_t layer_i = 0; layer_i < _layer_to_n.size(); ++layer_i)
                {
                    _layers.emplace_back();
                    _layers.back().resize(_layer_to_n.at(layer_i), Neuron(layer_i, (layer_i == 0 ? 0 : _layer_to_n.at(layer_i - 1))));
                }

                // randomize weights
                auto dist = std::normal_distribution<float>(0, 0.02);
                auto engine = std::mt19937();

                for (size_t layer_i = 1; layer_i < _layers.size(); ++layer_i)
                    for (size_t node_i = 0; node_i < _layers.at(layer_i).size(); ++node_i)
                        for (size_t weight_i = 0; weight_i < _layers.at(layer_i-1).size(); ++weight_i)
                            override_weight(layer_i, node_i, weight_i, dist(engine));
            }

            void override_weight(size_t layer_i, size_t neuron_i, size_t weight_i, float new_weight)
            {
                _layers.at(layer_i).at(neuron_i)._weights.at(weight_i) = new_weight;
            }

            void override_bias(size_t layer_i, size_t neuron_i, float new_bias)
            {
                _layers.at(layer_i).at(neuron_i)._bias = new_bias;
            }

            using FeatureVector_t = Vector<float, std::get<0>(std::make_tuple(LayerN...))>;
            using OutputVector_t = Vector<float, std::get<sizeof...(LayerN)-1>(std::make_tuple(LayerN...))>;


            /// @returns pair where .first is the class, .second the response value
            OutputVector_t forward_pass(const FeatureVector_t& feature_vector)
            {
                std::vector<std::vector<float>> responses;
                responses.reserve(_layer_to_n.size());

                responses.emplace_back();
                responses.back().reserve(FeatureVector_t::size());

                for (size_t i = 0; i < FeatureVector_t::size(); ++i)
                    responses.back().push_back(feature_vector.at(i));

                for (size_t layer_i = 1; layer_i < _layers.size(); ++layer_i)
                {
                    responses.emplace_back();
                    responses.back().reserve(_layer_to_n.at(layer_i));

                    for (size_t neuron_i = 0; neuron_i < _layers.at(layer_i).size(); ++neuron_i)
                        responses.back().push_back(
                                _layers.at(layer_i).at(neuron_i).activate(responses.at(layer_i - 1)));
                }

                OutputVector_t output;
                for (size_t i = 0; i < _layers.back().size(); ++i)
                    output.at(i) = responses.back().at(i);

                return output;
            }

            static inline float alpha;

            void back_propagate(const FeatureVector_t& input, const OutputVector_t& desired_output)
            {
                // forward pass first
                std::vector<std::vector<float>> observed;
                observed.reserve(_layer_to_n.size());

                observed.emplace_back();
                observed.back().reserve(FeatureVector_t::size());

                for (size_t i = 0; i < FeatureVector_t::size(); ++i)
                    observed.back().push_back(input.at(i));

                for (size_t layer_i = 1; layer_i < _layers.size(); ++layer_i)
                {
                    observed.emplace_back();
                    observed.back().reserve(_layer_to_n.at(layer_i));

                    for (size_t neuron_i = 0; neuron_i < _layers.at(layer_i).size(); ++neuron_i)
                        observed.back().push_back(_layers.at(layer_i).at(neuron_i).activate(observed.at(layer_i - 1)));
                }

                // init error
                std::vector<std::vector<float>> node_delta;
                for (size_t i = 0; i < _layers.size(); ++i)
                {
                    node_delta.emplace_back();
                    for (size_t j = 0; j < _layers.at(i).size(); ++j)
                        node_delta.back().push_back(0);
                }

                // last layer
                for (size_t i = 0; i < _layers.back().size(); ++i)
                {
                    const size_t l = _layers.size() - 1;

                    float hzj = 0;
                    for (size_t k = 0; k < _layers.at(l-1).size(); ++k)
                        hzj += observed.at(l-1).at(k);

                    float error = observed.at(l).at(i) - desired_output.at(i);

                    node_delta.at(l).at(i) = hzj * (1 - hzj) * error;
                }

                // deltas
                for (size_t l = _layers.size() - 2; l > 0; l--)
                {
                    for (size_t j = 0; j < _layers.at(l).size(); ++j)
                    {
                        float hzj = 0;
                        for (size_t k = 0; k < _layers.at(l-1).size(); ++k)
                            hzj += observed.at(l-1).at(k);

                        float sum = 0;
                        for (size_t i = 0; i < _layers.at(l+1).size(); ++i)
                            sum += _layers.at(l+1).at(j)._weights.at(i) * node_delta.at(l+1).at(i);

                        node_delta.at(l).at(j) = hzj * (1 - hzj) * sum;
                    }
                }

                // update
                for (size_t l = _layers.size() - 1; l > 0; l--)
                {
                    for (size_t i = 0; i < _layers.at(l).size(); ++i)
                    {
                        for (size_t j = 0; j < _layers.at(l-1).size(); ++j)
                        {
                            _layers.at(l).at(i)._weights.at(j) -= alpha * node_delta.at(l).at(i) * observed.at(l-1).at(j);
                        }

                        _layers.at(l).at(i)._bias -= alpha * node_delta.at(l).at(i);
                    }
                }
            }
    };
     */
}