// 
// Copyright 2021 Clemens Cords
// Created on 01.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <array>
#include <Dense>

namespace crisp
{
    /// @brief fully connected neural network
    /// @tparam NeuronsPerLayer: list {i, n_1, n_2, ..., n_m, o} where
    /// @tparam i: number of features in feature vector
    /// @tparam n_x: number of neurons in each hidden layer (min 1 layer)
    /// @tparam o: number of neurons in output layer (recommended to be equal to the number of desired classes)
    template<size_t... NeuronsPerLayer>
    class NeuralNetwork
    {
        static_assert(sizeof...(NeuronsPerLayer) >= 3, "crisp::NeuralNetwork needs to have at least 3 layers: 1 input, 1 hidden and 1 output");

        public:
            /// @brief access number of neurons in specified layer at compile-time
            /// @tparam N: layer index   (template argument)
            /// @returns number of neurons
            template<size_t n>
            static constexpr size_t get_n_neurons_in_layer();

            /// @brief matrix holding one or more feature vectors, each column is one sample, each row is one feature. Feature values should be normalized into [-1, 1]
            using InputMatrix_t = Eigen::Matrix<float, get_n_neurons_in_layer<0>(), Eigen::Dynamic>;

            /// @brief matrix holding the output layer activation results for one or more feature vector samples. Each column is one sample, each row is the neuron output. Outputs should be normalized into [0, 1]
            using OutputMatrix_t = Eigen::Matrix<float, get_n_neurons_in_layer<sizeof...(NeuronsPerLayer) - 1>(), Eigen::Dynamic>;

            /// @brief ctor
            /// @param learning_rate: constant that governs learning speed, often called alpha or mu in the literature. Recommended range: [0.001, 0.1]
            NeuralNetwork(float learning_rate = 0.01);

            /// @brief forward pass through network
            /// @param input: matrix holding one or more samples, each column is one sample, each row one feature
            /// @returns matrix holding activation values, each column is one sample, each row one output layer neuron activation result
            OutputMatrix_t identify(const InputMatrix_t&);

            /// @brief back propagate to train network once
            /// @param features: matrix holding one or more samples, each column is one sample, each row one feature
            /// @param desired_result: matrix holding one or more desired response vectors, each column is one sample, each row is one neuron in the output layer.
            void train(InputMatrix_t features, OutputMatrix_t desired_result);

            /// @brief set learning rate for all future train(...) calls
            /// @param alpha: learning rate, recommended range: [0.001, 0.1]
            /// @note for badly chosen alpha, divergence may be exhibited
            void set_learning_rate(float);

            /// @brief export the networks weights and biases as a string, useful for storing in permanent memory
            /// @returns string, can be read from using from_string
            /// @note the networks architecture (the number of layers and number of neurons per layer) must be kept externally
            std::string as_string() const;

            /// @brief imports network weights and biases from string generated using as_string()
            /// @param string
            /// @note parsing is only succesful if the networks architecture (the number of layers and number of neurons per layer) is the same as when as_string() was called
            void from_string(const std::string&);

            /// @brief reset all weights to 1+e and all biases to 0 where e in [-0.25, +0.25] uniformly distributed
            void clear();

        protected:
            /// @brief manually set a specific weight
            /// @param layer_i: index of layer
            /// @param neuron_i: index of neuron
            /// @param weight_i: index of weight
            void override_weight(size_t layer_i, size_t neuron_i, size_t weight_i, float weight);

            /// @brief manually set a specific bias
            /// @param layer_i: index of layer
            /// @param neuron_i: index of neuron
            void override_bias(size_t layer_i, size_t neuron_i, float bias);

        private:
            float activation_function(float x);
            float activation_function_partial_derivative(float x);

            float _alpha;
            std::array<Eigen::MatrixXf, sizeof...(NeuronsPerLayer)> _weights;
            std::array<Eigen::MatrixXf, sizeof...(NeuronsPerLayer)> _bias;

            const static inline std::vector<size_t> _layer_to_n = {NeuronsPerLayer...};
            static inline size_t seed = 0;
    };
}

#include ".src/fully_connected_neural_network.inl"