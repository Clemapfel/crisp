// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <.classification/fully_connected_neural_network.hpp>
#include <thread>
#include <iostream>

using namespace crisp;

int main()
{
    // A  B    XOR  NOT XOR
    // 0  1 -> 1    0
    // 1  0 -> 1    0
    // 1  1 -> 0    1
    // 0  0 -> 0    1

    Eigen::MatrixXf feature;
    feature.resize(2, 4);
    feature << 1, -1, -1,  1,
               1, -1,  1, -1;

    Eigen::MatrixXf desired;
    desired.resize(2, 4);
    desired << 1, 0, 1, 0,
               0, 1, 0, 1;

    auto nn = NeuralNetwork<2, 2, 2, 2>();

    for (size_t n_epochs = 0; n_epochs < 1000; ++n_epochs)
    {
        nn.train(feature, desired);
    }

    std::cout << "before:" << std::endl;
    std::cout << nn.identify(feature) << std::endl;
    std::cout << std::endl;

    auto str = nn.as_string();
    nn.clear();
    nn.from_string(str);

    std::cout << "after:" << std::endl;
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

