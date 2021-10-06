// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <classification/fully_connected_neural_network.hpp>
#include <classification/bayes_classifier.hpp>
#include <thread>
#include <iostream>
#include <fstream>

using namespace crisp;

int main()
{
    Eigen::MatrixXf features;
    features.resize(3, 12);
    features << 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4,
                0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0, 0,
                1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0;

    auto classes = features;
    classes <<  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
                0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
                1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0;


    auto bayes = BayesClassifier<3, 3>();
    bayes.train(features, classes);

    auto ctest = Eigen::MatrixXf(3, 3);
    ctest << 0, 1, 0,
             1, 0, 0,
             0, 0, 1;

    std::cout << bayes.identify(ctest) << std::endl;
    return 0;

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

