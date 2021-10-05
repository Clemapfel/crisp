// 
// Copyright 2021 Clemens Cords
// Created on 04.10.21 by clem (mail@clemens-cords.com)
//

#include <classification.hpp>
using namespace crisp;

int main()
{
    auto classes = Eigen::Matrix<float, 2, 12>();
    classes << 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // is chicken
               0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1; // is duck

    auto features = Eigen::Matrix<float, 2, 12>();
    features << .52, .58, .67, .62, .59, .65, .61, .63, .69, .60, .71, .75, // weight
                .31, .34, .32, .37, .32, .29, .36, .35, .39, .38, .37, .34; // yok ratio

    auto bayes = BayesClassifier<2, 2>(); // 2 features, 2 classes
    bayes.train(features, classes);

    auto to_identify = Eigen::MatrixXf(2, 3);
    //               0    1    2    // egg
    to_identify <<  .51, .60, .70,   // weight
                    .29, .28, .38;   // yolk ratio

    auto result = bayes.identify(to_identify);
    std::cout << result << "\n" << std::endl;

    auto nn = NeuralNetwork<2, 4, 4, 2>(0.0001);

    for (size_t x = 0; x < features.rows(); ++x)
        for (size_t y = 0; y < features.cols(); ++y)
            features(x, y) = features(x, y) * 2 - 1;

    nn.from_string("1 0 7.29966 -1.0088 1.97509 -5.70112 6.09532 1.90318 6.66415 4.52024 -0.362792 1.39296 1.78121 2.78627 4.87721 2.98852 4.78013 -1.24513 1.73004 1.00528 2.00333 0.658974 5.5919 -0.682239 3.20744 1.42496 2.60235 -0.782948 1.89215 7.16907 0.634797 1.12685 0.292728 -1.21311 -1.83605 3.14411 3.11112 -1.64273 -1.2936 -1.85931 1.39732 -1.0836 1.41304 0.890882 ");

    for (size_t x = 0; x < to_identify.rows(); ++x)
        for (size_t y = 0; y < to_identify.cols(); ++y)
            to_identify(x, y) = to_identify(x, y) * 2 - 1;

    std::cout << classes << "\n" << std::endl;
    std::cout << nn.identify(features) << "\n" << std::endl;
    std::cout << nn.identify(to_identify) << std::endl;
}

