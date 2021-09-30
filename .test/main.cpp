// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <.classification/fully_connected_neural_network.hpp>

using namespace crisp;

int main()
{
    // A  B    XOR  NOT XOR
    // 0  1 -> 1    0
    // 1  0 -> 1    0
    // 1  1 -> 0    1
    // 0  0 -> 0    1

    std::vector<Vector2f> feature = {
            {0, 1},
            {1, 0},
            {1, 1},
            {0, 0}
    };

    std::vector<Vector2f> response = {
            {1, 0},
            {1, 0},
            {0, 1},
            {0, 1}
    };

    auto nn = NeuralNetwork<2, 2, 2>(1);

    size_t n_epochs = 100000;
    while (n_epochs > 0)
    {
        for (size_t i = 0; i < feature.size(); ++i)
            nn.back_propagate(feature.at(i), response.at(i));
        n_epochs--;
    }

    for (size_t i = 0; i < feature.size(); ++i)
    {
        std::cout << "feature: " << feature.at(i).x() << " " << feature.at(i).y() << std::endl;
        auto result = nn.forward_pass(feature.at(i));
        std::cout << "response: " << "XOR: " << result.x() << " | NOT XOR: " << result.y() << std::endl << std::endl;
    }

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

    std::cout << nn.forward_pass({3, 0, 1}).second << std::endl;
     */

}

