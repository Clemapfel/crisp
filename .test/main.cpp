// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <.classification/fully_connected_neural_network.hpp>

using namespace crisp;

int main()
{
    auto nn = NeuralNetwork<1, 2, 3>();

    std::cout << nn.forward_pass({2}).second << std::endl;

}

