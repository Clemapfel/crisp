// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <classification/fully_connected_neural_network.hpp>
#include <thread>
#include <iostream>
#include <fstream>

using namespace crisp;

int main()
{
    Eigen::MatrixXf c1;
    c1.resize(3, 4);
    c1 << 0, 0, 1, 0,
          0, 1, 1, 1,
          1, 1, 1, 0;

    auto c2 = c1;
    c2 << 1, 1, 1, 0,
          0, 0, 1, 0,
          1, 0, 0, 0;

    // estimate mean
    auto estimate_mean = [](const Eigen::MatrixXf& m)
    {
        Eigen::MatrixXf out;
        out.resize(m.rows(), 1);
        out.setConstant(0);

        for (size_t y = 0; y < m.cols(); y++)
            for (size_t x = 0; x < m.rows(); x++)
                out(x, 0) += m(x, y);

        out /= m.cols();
        return out;
    };

    // estimate covar
    auto estimate_covar = [&](const Eigen::MatrixXf& m)
    {
        auto mean = estimate_mean(m);

        auto covar = Eigen::MatrixXf(m.rows(), m.rows());
        covar.setConstant(0);

        for (size_t i = 0; i < m.cols(); ++i)
        {
            auto x = m.block(0, i, m.rows(), 1);
            covar = covar + ((x - mean) * (x - mean).transpose());
        }
        covar /= m.cols();
        return covar;
    };

    auto decision_function = [&](Eigen::MatrixXf pattern, const Eigen::MatrixXf& m)
    {
        auto mean = estimate_mean(m);
        auto covar = estimate_covar(m);

        return covar.determinant() + ((pattern - mean).transpose() * covar.inverse() * (pattern - mean))(0, 0);
    };

    Eigen::MatrixXf test;
    test.resize(3, 1);
    test << 0, 0.4, 0;
    decision_function(test, c1);
    std::cout << "A: " << decision_function(test, c1) << std::endl;
    std::cout << "B: " << decision_function(test, c2) << std::endl;


    /*
    // A  B    XOR  NOT XOR
    // 0  1 -> 1    0
    // 1  0 -> 1    0
    // 1  1 -> 0    1
    // 0  0 -> 0    1

    Eigen::MatrixXf feature;
    feature.resize(2, 4);
    feature << 1, -1, 0,  -0.5,
               -1, -1, 0, +0.5;

    Eigen::MatrixXf desired;
    desired.resize(2, 4);
    desired << 1, 0, 1, 0,
               0, 1, 0, 1;

    auto nn = NeuralNetwork<2, 2, 2, 2>();

    auto file = std::ifstream("/home/clem/Workspace/crisp/docs/feature_classification/xor_nn.bin");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string str = buffer.str();
    nn.from_string(str);

    //std::cout << nn.train_until(feature, desired, 0.0001) << std::endl;

    std::cout << nn.identify(feature) << std::endl;
    std::cout << std::endl;

    /*
    auto nn = NeuralNetwork<3, 2, 2>();

    // pg. 949
    nn.override_weight(1, 0, 0, 0.1);
    nn.override_weight(1, 0, 1, 0.2);
    nn.override_weight(1, 0, 2, 0.6);
    nn.override_bias(1, 0, 0.4);

    nn.override_weight(1, 1, 0, 0.4);
    nn.override_weight(1, 1, 1, 0.3);
    nn.override_weight(1, 1, 2, 0.1);
    nn.override_bias(1, 1, 0.2);

    nn.override_weight(2, 0, 0, 0.2);
    nn.override_weight(2, 0, 1, 0.1);
    nn.override_bias(2, 0, 0.6);

    nn.override_weight(2, 1, 0, 0.1);
    nn.override_weight(2, 1, 1, 0.4);
    nn.override_bias(2, 1, 0.3);

    //std::cout << nn.forward_pass({3, 0, 1}) << std::endl;

    Eigen::MatrixXf input;
    input.resize(3, 4);
    input << 3, 3, 3, 3,
             0, 0, 0, 0,
             1, 1, 1, 1;

    nn.back_propagate(input, input);
     */


}

