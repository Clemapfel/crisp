// 
// Copyright 2021 Clemens Cords
// Created on 01.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <array>

#include <Dense>

namespace crisp
{
    template<size_t... LayerN>
    class NeuralNetwork
    {
        static_assert(sizeof...(LayerN) >= 3, "need at least 3 layers: 1 input, 1 hidden, 1 output");

        template<size_t n>
        static constexpr size_t get_layer_n() {return std::get<n>(std::make_tuple(LayerN...));};

        public:
            using InputMatrix_t = Eigen::Matrix<float, get_layer_n<0>(), Eigen::Dynamic>;
            using OutputMatrix_t = Eigen::Matrix<float, get_layer_n<sizeof...(LayerN)-1>(), Eigen::Dynamic>;

            NeuralNetwork(float learning_rate = 0.01);

            /// @brief forward propagate through network
            OutputMatrix_t identify(const InputMatrix_t&);

            /// @brief back propagate to train network once
            void train(InputMatrix_t features, OutputMatrix_t desired_result);

        protected:
            void override_weight(size_t layer_i, size_t neuron_i, size_t weight_i, float weight);
            void override_bias(size_t layer_i, size_t neuron_i, float bias);

        private:
            float activation_function(float x);
            float activation_function_partial_derivative(float x);

            float _alpha;
            std::array<Eigen::MatrixXf, sizeof...(LayerN)> _weights;
            std::array<Eigen::MatrixXf, sizeof...(LayerN)> _bias;

            const static inline std::vector<size_t> _layer_to_n = {LayerN...};
            static inline size_t seed = 0;
    };
}

#include ".src/fully_connected_neural_network.inl"