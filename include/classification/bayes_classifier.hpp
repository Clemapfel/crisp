// 
// Copyright 2021 Clemens Cords
// Created on 24.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>

namespace crisp
{
    /// @brief naive bayes optimum statistical classifier, assumes distribution to be gaussian
    /// @tparam FeatureN: number of features in feature vector
    /// @tparam ClassN: number of possible classes
    template<size_t FeatureN, size_t ClassN>
    class BayesClassifier
    {
        using FeatureMatrix_t = Eigen::Matrix<float, FeatureN, Eigen::Dynamic>;
        using ClassificationMatrix_t = Eigen::Matrix<float, ClassN, Eigen::Dynamic>;

        public:
            /// @brief estimate variance and mean from population
            /// @param features: feature matrix where each row is one feature, each column one sample
            /// @param desired: classification matrix where each row is one class, each column one sample
            void train(const FeatureMatrix_t&, const ClassificationMatrix_t&);

            /// @brief classify a feature vector
            /// @param samples: matrix of size Feature*x where each row is one feature, each column one sample
            /// @returns matrix of size ClassN*x where each row is one class, each column one sample. The row with the highest value is the predicted classification
            ClassificationMatrix_t identify(const FeatureMatrix_t&);

        private:
            struct ClassInfo
            {
                float probability = 1.f / ClassN;
                Eigen::Matrix<float, FeatureN, 1> variance;
                Eigen::Matrix<float, FeatureN, 1> mean;
                size_t n = 0;
                bool trained = false;
            };

            std::array<ClassInfo, ClassN> _classes;
    };
}

#include ".src/bayes_classifier.inl"