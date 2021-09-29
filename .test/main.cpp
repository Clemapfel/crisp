// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <.classification/fully_connected_neural_network.hpp>

using namespace crisp;

int main()
{
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

    std::cout << nn.forward_pass({3, 0, 1}).second << std::endl;

}

