// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <.classification/fully_connected_neural_network.hpp>
#include <thread>

using namespace crisp;

int main()
{
    /*
    // A  B    XOR  NOT XOR
    // 0  1 -> 1    0
    // 1  0 -> 1    0
    // 1  1 -> 0    1
    // 0  0 -> 0    1

    std::vector<Vector2f> feature = {
            {-1, 1},
            {1, -1},
            {1, 1},
            {-1, -1}
    };

    std::vector<Vector2f> desired = {
            {1, 0},
            {1, 0},
            {0, 1},
            {0, 1}
    };

    auto nn = NeuralNetwork<2, 2, 2>(1);

    size_t n_epochs = 10000000;
    while (n_epochs > 0)
    {
        for (size_t i = 0; i < feature.size(); ++i)
            nn.back_propagate(feature.at(i), desired.at(i));

        //std::cout << "---------------------------------------------------------" << std::endl;
        Vector2f error = Vector2f(0);
        for (size_t i = 0; i < feature.size(); ++i)
        {
            auto des = desired.at(i);
            auto res = nn.forward_pass(feature.at(i));

            for (size_t i = 0; i < res.size(); ++i)
                error += (des.at(i) - res.at(i)) * (des.at(i) - res.at(i));

            if (false)
            {
                std::cout << "feature : " << feature.at(i).x() << " " << feature.at(i).y() << std::endl;
                std::cout << "desired : " << desired.at(i).x() << " " << desired.at(i).y() << std::endl;
                std::cout << "response: " << res.x() << " " << res.y() << std::endl << std::endl;
            }

        }

        error /= feature.size();
        std::cout << (error.x() + error.y()) / 2 << std::endl;

        //std::this_thread::sleep_for(std::chrono::seconds(2));
        n_epochs--;
    }
    */


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

    std::cout << nn.forward_pass({3, 0, 1}) << std::endl;


}

