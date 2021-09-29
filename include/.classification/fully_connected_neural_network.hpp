// 
// Copyright 2021 Clemens Cords
// Created on 24.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector.hpp>
#include <iostream>

namespace crisp
{
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
                        _weights.resize(1, 1);
                        _bias = 0;
                    }
                    else
                    {
                        _weights.resize(previous_layer_n, 1);
                        _bias = 1;
                    }
                }

                // updates response
                float activate(const std::vector<float>& previous_layer)
                {
                    std::cout << "(" << _layer_i << ")" << std::endl;
                    for (auto& w : _weights)
                        std::cout << " " << w << " |";
                    std::cout << "| " << _bias << std::endl;

                    float sum = 0;
                    for (size_t i = 0; i < previous_layer.size(); ++i)
                        sum += _weights.at(i) * previous_layer.at(i);

                    sum += _bias;
                    float out = (1 / (1 + expf(-1 * sum)));

                    std::cout << "-> " << out << "\n" << std::endl;
                    return out;
                }

                //private:
                    std::vector<float> _weights;
                    float _bias;
                    size_t _layer_i;
            };

            std::vector<std::vector<Neuron>> _layers;

        public:
            NeuralNetwork()
            {
                for (size_t layer_i = 0; layer_i < _layer_to_n.size(); ++layer_i)
                {
                    _layers.emplace_back();
                    _layers.back().resize(_layer_to_n.at(layer_i), Neuron(layer_i, (layer_i == 0 ? 0 : _layer_to_n.at(layer_i - 1))));
                }
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
            std::pair<size_t, float> forward_pass(const FeatureVector_t& feature_vector)
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

                size_t max_i = -1;
                float max_value = std::numeric_limits<float>::min();
                for (size_t i = 0; i < _layer_to_n.back(); ++i)
                {
                    float value = responses.back().at(i);
                    if (value > max_value)
                    {
                        max_i = i;
                        max_value = value;
                    }
                }

                return std::make_pair(max_i, max_value);
            }

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
                        observed.back().push_back(
                                _layers.at(layer_i).at(neuron_i).activate(observed.at(layer_i - 1)));
                }

                // init error
                std::vector<std::vector<float>> error;
                for (size_t i = 0; i < _layers.size(); ++i)
                {
                    error.emplace_back();
                    for (size_t j = 0; j < _layers.at(i).size(); ++j)
                        error.back().push_back(-1);
                }

                // last layer
                for (size_t i = 0; i < error.back().size(); ++i)
                {
                    float hzj = observed.at(observed.size() - 2).at(i);
                    error.at(i) = hzj * (1 - hzj) * observed.back().at(i) - desired_output.at(i);
                }

                // back propagate
                for (size_t layer_i = _layers.size() - 2; layer_i >= 0; layer_i--)
                {
                    for (size_t node_i = 0; node_i < error.at(layer_i).size(); node_i++)
                    {
                        float hzj = observed.at(layer_i).at(node_i);

                        float delta = 0;
                        for (size_t weight_i = 0; weight_i < _layers.at(layer_i-1).size(); ++weight_i)
                        {
                            delta +=
                        }
                    }
                }






            }
    };
}