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
    /// @tparam NeuronsPerLayer: list of size_t {i, n_1, n_2, ..., n_m, o} where
    /// @tparam i: number of features in feature vector
    /// @tparam n_x: number of neurons in each hidden layer
    /// @tparam o: number of neurons in output layer (recommended to be equal to the number of desired classes)
    template<size_t... NeuronsPerLayer>
    class NeuralNetwork
    {
        static_assert(sizeof...(NeuronsPerLayer) >= 3, "crisp::NeuralNetwork needs to have at least 3 layers: 1 input, 1 hidden and 1 output");

        public:
            /// @brief access number of neurons in specified layer at compile-time
            /// @tparam N: layer index
            /// @returns number of neurons
            template<size_t N>
            static constexpr size_t get_n_neurons_in_layer();

            /// @brief matrix holding one or more feature vectors, each column is one sample, each row is one feature. Feature values should be normalized into [-1, 1]
            using InputMatrix_t = Eigen::Matrix<float, get_n_neurons_in_layer<0>(), Eigen::Dynamic>;

            /// @brief matrix holding the output layer activation results for one or more feature vector samples. Each column is one sample, each row is output of one neuron.
            using OutputMatrix_t = Eigen::Matrix<float, get_n_neurons_in_layer<sizeof...(NeuronsPerLayer) - 1>(), Eigen::Dynamic>;

            /// @brief ctor
            /// @param learning_rate: constant that governs learning speed, often called alpha or mu in the literature. Recommended range: [0.001, 0.1]
            NeuralNetwork(float learning_rate = 0.01);

            /// @brief forward pass through network
            /// @param input: matrix holding one or more samples, each column is one sample, each row one feature
            /// @returns matrix holding activation values, each column is one sample, each row one output layer neuron activation result
            [[nodiscard]] OutputMatrix_t identify(const InputMatrix_t&);

            /// @brief back propagate to train network a single time
            /// @param features: matrix holding one or more samples, each column is one sample, each row one feature. Features should be normalized into [-1, 1]
            /// @param desired_result: matrix holding one or more desired response vectors, each column is one sample, each row is the desired output for that neuron. All values should be either 0 or 1
            void train(InputMatrix_t features, OutputMatrix_t desired_result);

            /// @brief back propagate until mean squared error is below threshold for more than 3 runs in a row
            /// @param features: matrix holding one or more samples, each column is one sample, each row one feature. Features should be normalized into [-1, 1]
            /// @param desired_result: matrix holding one or more desired response vectors, each column is one sample, each row is the desired output for that neuron. All values should be either 0 or 1
            /// @param mse_threshold: threshold for mean squared error, recommended range: [0.000001, 0.01]
            /// @returns number of epochs elapsed
            size_t train_until(InputMatrix_t features, OutputMatrix_t desired_result, float mse_threshold);

            /// @brief forward pass for given feature vectors, then compute maximum mean squared error in result
            /// @param features: matrix holding one or more samples, each column is one sample, each row one feature. Features should be normalized into [-1, 1]
            /// @param desired_result: matrix holding one or more desired response vectors, each column is one sample, each row is the desired output for that neuron. All values should be either 0 or 1
            /// @returns mean squared error
            float compute_mean_squared_error(InputMatrix_t features, OutputMatrix_t desired_result);

            /// @brief compute ratio of misidentifications using current weights
            /// @param features: matrix holding one or more samples, each column is one sample, each row one feature. Features should be normalized into [-1, 1]
            /// @param desired_result: matrix holding one or more desired response vectors, each column is one sample, each row is the desired output for that neuron. All values should be either 0 or 1
            /// @returns float in [0, 1] where 0 if all correct, 1 if none correct
            float compute_classification_error(InputMatrix_t features, OutputMatrix_t desired_result);

            /// @brief set learning rate for all future back propagations
            /// @param alpha: learning rate, recommended range: [0.001, 0.1]
            /// @note for badly chosen alpha divergence may occur
            void set_learning_rate(float alpha);

            /// @brief export the networks weights and biases as a string, useful for storing in permanent memory
            /// @returns string, can be read from using NeuralNetwork::from_string()
            std::string as_string() const;

            /// @brief imports network weights and biases from string generated using NeuralNetwork::as_string()
            /// @param string generated by a neural network with the same number of layers and number of nodes per layer
            void from_string(const std::string&);

            /// @brief reset all weights to 1 + eps and all biases to 0 where eps in [-0.25, +0.25] randomly (uniformly distributed)
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
            static float mse_from(const OutputMatrix_t& actual, const OutputMatrix_t& desired);

            OutputMatrix_t back_propagate(const InputMatrix_t&, const OutputMatrix_t&);

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