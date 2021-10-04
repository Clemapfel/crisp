// 
// Copyright 2021 Clemens Cords
// Created on 01.10.21 by clem (mail@clemens-cords.com)
//

#include <classification/fully_connected_neural_network.hpp>
#include <random>
#include <iostream>

namespace crisp
{
    template<size_t... Ns>
    template<size_t n>
    constexpr size_t NeuralNetwork<Ns...>::get_n_neurons_in_layer()
    {
        return std::get<n>(std::make_tuple(Ns...));
    };

    template<size_t... Ns>
    float NeuralNetwork<Ns...>::activation_function(float x)
    {
        float x2 = x * x;
        float a = x * (135135.0f + x2 * (17325.0f + x2 * (378.0f + x2)));
        float b = 135135.0f + x2 * (62370.0f + x2 * (3150.0f + x2 * 28.0f));
        return a / b;

        // source: https://math.stackexchange.com/questions/107292/rapid-approximation-of-tanhx
    }

    template<size_t... Ns>
    float NeuralNetwork<Ns...>::activation_function_partial_derivative(float x)
    {
        return 1 - x*x;
    }
    
    template<size_t... Ns>
    NeuralNetwork<Ns...>::NeuralNetwork(float learning_rate)
        : _alpha(learning_rate)
    {
        _weights.at(0).resize(1, 1);
        _weights.at(0).setConstant(1);
        _bias.at(0).resize(1, 1);
        _bias.at(0).setConstant(0);

        auto dist = std::normal_distribution(-0.25, +0.25);
        auto engine = std::mt19937(seed++);

        for (size_t layer_i = 1; layer_i < _layer_to_n.size(); ++layer_i)
        {
            _weights.at(layer_i).resize(_layer_to_n.at(layer_i), _layer_to_n.at(layer_i - 1));
            _bias.at(layer_i).resize(_layer_to_n.at(layer_i), 1);

            for (size_t node_i = 0; node_i < _layer_to_n.at(layer_i); ++node_i)
            {
                for (size_t weight_i = 0; weight_i < _layer_to_n.at(layer_i-1); weight_i++)
                    _weights.at(layer_i)(node_i, weight_i) = 1 + dist(engine);

                _bias.at(layer_i)(node_i, 0) = dist(engine);
            }
        }
    }
    
    template<size_t... Ns>
    typename NeuralNetwork<Ns...>::ClassificationMatrix_t NeuralNetwork<Ns...>::identify(const FeatureMatrix_t& input)
    {
        const size_t n_layers = _layer_to_n.size();

        std::vector<Eigen::MatrixXf> bias;

        for (size_t l = 0; l < _bias.size(); ++l)
        {
            bias.emplace_back(_bias.at(l).rows(), input.cols());
            for (size_t col_i = 0; col_i < input.cols(); ++col_i)
                for (size_t row_i = 0; row_i < _bias.at(l).rows(); ++row_i)
                    bias.back()(row_i, col_i) = _bias.at(l)(row_i, 0);
        }

        std::vector<Eigen::MatrixXf> zl;
        zl.push_back(input);

        std::vector<Eigen::MatrixXf> hzl;

        for (size_t l = 0; l < n_layers; ++l)
        {
            if (l > 0)
                zl.push_back(_weights.at(l) * hzl.at(l-1) + bias.at(l));

            hzl.emplace_back(zl.back().rows(), zl.back().cols());
            for (size_t i = 0; i < hzl.back().rows(); ++i)
                for (size_t j = 0; j < hzl.back().cols(); ++j)
                    hzl.back()(i, j) = activation_function(zl.back()(i, j));
        }

        return hzl.back();
    }

    template<size_t... Ns>
    typename NeuralNetwork<Ns...>::ClassificationMatrix_t NeuralNetwork<Ns...>::back_propagate(const FeatureMatrix_t& input, const ClassificationMatrix_t& desired)
    {
         const size_t n_layers = _layer_to_n.size();

        // create bias by concatenating
        std::vector<Eigen::MatrixXf> bias;

        for (size_t l = 0; l < _bias.size(); ++l)
        {
            bias.emplace_back(_bias.at(l).rows(), input.cols());
            for (size_t col_i = 0; col_i < input.cols(); ++col_i)
                for (size_t row_i = 0; row_i < _bias.at(l).rows(); ++row_i)
                    bias.back()(row_i, col_i) = _bias.at(l)(row_i, 0);
        }

        // feed forward
        std::vector<Eigen::MatrixXf> zl;     // raw
        zl.push_back(input);

        std::vector<Eigen::MatrixXf> hzl;    // sigmoid of raw
        hzl.push_back(input);

        std::vector<Eigen::MatrixXf> hdzl;   // sigmoid derivative of raw

        for (size_t l = 0; l < n_layers; ++l)
        {
            if (l > 0)
            {
                zl.push_back(_weights.at(l) * hzl.at(l - 1) + bias.at(l));

                hzl.emplace_back(zl.back().rows(), zl.back().cols());
                for (size_t i = 0; i < hzl.back().rows(); ++i)
                    for (size_t j = 0; j < hzl.back().cols(); ++j)
                        hzl.back()(i, j) = activation_function(zl.back()(i, j));
            }

            hdzl.emplace_back(zl.back().rows(), zl.back().cols());
            for (size_t i = 0; i < hzl.back().rows(); ++i)
                for (size_t j = 0; j < hzl.back().cols(); ++j)
                    hdzl.back()(i, j) = activation_function_partial_derivative(hzl.back()(i, j));
        }

        std::vector<Eigen::MatrixXf> deltas;
        deltas.resize(n_layers);

        deltas.at(n_layers-1) = (hzl.back() - desired).cwiseProduct(hdzl.at(n_layers-1));

        for (size_t l = n_layers -2; l >= 1; l--)
            deltas.at(l) = (_weights.at(l+1).transpose() * deltas.at(l+1)).cwiseProduct(hdzl.at(l));

        const float alpha = 0.01;
        for (size_t l = 1; l < n_layers; l++)
        {
            _weights.at(l) -= alpha * deltas.at(l) * hzl.at(l-1).transpose();

            for (size_t row_i = 0; row_i < _bias.at(l).rows(); ++row_i)
                for (size_t col_i = 0; col_i < deltas.at(l).cols(); ++col_i)
                    _bias.at(l)(row_i, 0) -= alpha * deltas.at(l)(row_i, col_i);
        }

        return hzl.back();
    }
    
    template<size_t... Ns>
    void NeuralNetwork<Ns...>::train(typename NeuralNetwork<Ns...>::FeatureMatrix_t input, ClassificationMatrix_t desired)
    {
        assert(input.cols() == desired.cols() && input.rows() == desired.rows());
        back_propagate(input, desired);
    }

    template<size_t... Ns>
    size_t NeuralNetwork<Ns...>::train_until(typename NeuralNetwork<Ns...>::FeatureMatrix_t input, ClassificationMatrix_t desired, float mse_threshold)
    {
        assert(input.cols() == desired.cols() && input.rows() == desired.rows());

        size_t n_epochs = 0,
               n_below_mse = 0;

        while (n_below_mse < 3)
        {
            if (mse_from(back_propagate(input, desired), desired) <= mse_threshold)
                n_below_mse++;
            else
                n_below_mse = 0;

            n_epochs++;
        }

        return n_epochs;
    }

    template<size_t... Ns>
    float NeuralNetwork<Ns...>::compute_mean_squared_error(typename NeuralNetwork<Ns...>::FeatureMatrix_t input, ClassificationMatrix_t desired)
    {
        assert(input.cols() == desired.cols());

        float error = 0;

        auto result = identify(input);
        return mse_from(result, desired);
    }

    template<size_t... Ns>
    float NeuralNetwork<Ns...>::mse_from(const typename NeuralNetwork<Ns...>::ClassificationMatrix_t& result, const ClassificationMatrix_t& desired)
    {
        float max_error = -1;
        for (size_t y = 0; y < result.cols(); ++y)
        {
            float error = 0;
            for (size_t x = 0; x < result.rows(); ++x)
                error += (desired(x, y) - result(x, y)) * (desired(x, y) - result(x, y));

            if (error > max_error)
                max_error = error;
        }

        return max_error / desired.rows();
    }

    template<size_t... Ns>
    float NeuralNetwork<Ns...>::compute_classification_error(typename NeuralNetwork<Ns...>::FeatureMatrix_t input, ClassificationMatrix_t desired)
    {
        assert(input.cols() == desired.cols());

        size_t n_correct = 0,
               n_wrong = 0;

        auto result = identify(input);

        for (size_t y = 0; y < result.cols(); ++y)
        {
            float max_result = 0;
            size_t max_result_i = -1;

            float max_desired = 0;
            size_t max_desired_i = -1;

            for (size_t x = 0; x < result.rows(); ++x)
            {
                if (result(x, y) > max_result)
                {
                    max_result = result(x, y);
                    max_result_i = x;
                }

                if (desired(x, y) > max_desired)
                {
                    max_desired = desired(x, y);
                    max_desired_i = x;
                }

                if (max_desired_i == max_result_i)
                    n_correct++;
                else
                    n_wrong++;
            }
        }

        return float(n_wrong) / float(n_correct + n_wrong);
    }


    template<size_t... Ns>
    void NeuralNetwork<Ns...>::set_learning_rate(float alpha)
    {
        _alpha = alpha;
    }

    template<size_t... Ns>
    void NeuralNetwork<Ns...>::override_weight(size_t layer_i, size_t neuron_i, size_t weight_i, float weight)
    {
        _weights.at(layer_i)(neuron_i, weight_i) = weight;
    }

    template<size_t... Ns>
    void NeuralNetwork<Ns...>::override_bias(size_t layer_i, size_t neuron_i, float bias)
    {
        _weights.at(layer_i)(neuron_i, 0) = bias;
    }

    template<size_t... Ns>
    std::string NeuralNetwork<Ns...>::as_string() const
    {
        std::stringstream str;
        for (size_t i = 0; i < _weights.size(); ++i)
        {
            for (size_t y = 0; y < _weights.at(i).cols(); ++y)
                for (size_t x = 0; x < _weights.at(i).rows(); ++x)
                    str << _weights.at(i)(x, y) << " ";

            for (size_t y = 0; y < _bias.at(i).cols(); ++y)
                for (size_t x = 0; x < _bias.at(i).rows(); ++x)
                    str << _bias.at(i)(x, y) << " ";
        }

        str << std::endl;
        return str.str();
    }

    template<size_t... Ns>
    void NeuralNetwork<Ns...>::from_string(const std::string& in)
    {
        std::vector<float> values;

        std::string buffer;
        std::stringstream ss(in);

        while (ss >> buffer)
            values.push_back(std::stof(buffer));

        for (size_t l = 0, i = 0; l < _weights.size(); ++l)
        {
            for (size_t y = 0; y < _weights.at(l).cols(); ++y)
                for (size_t x = 0; x < _weights.at(l).rows(); ++x, ++i)
                    _weights.at(l)(x, y) = values.at(i);

            for (size_t y = 0; y < _bias.at(l).cols(); ++y)
                for (size_t x = 0; x < _bias.at(l).rows(); ++x, ++i)
                    _bias.at(l)(x, y) = values.at(i);
        }
    }

    template<size_t... Ns>
    void NeuralNetwork<Ns...>::clear()
    {
        for (size_t l = 0, i = 0; l < _weights.size(); ++l)
        {
            _weights.at(l).setConstant(1);
            _bias.at(l).setConstant(0);
        }
    }
}