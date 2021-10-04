// 
// Copyright 2021 Clemens Cords
// Created on 24.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>

namespace crisp
{
    /// @brief naive bayes optimum statistical classifier, assumes distribution to be gaussian
    template<size_t FeatureN, size_t ClassN>
    class BayesClassifier
    {
        using FeatureMatrix_t = Eigen::Matrix<float, FeatureN, Eigen::Dynamic>;
        using ClassificationMatrix_t = Eigen::Matrix<float, ClassN, Eigen::Dynamic>;

        public:
            /// @brief add population fo samples to class with given index
            /// @param class_i: index of class in {0, 1, .., ClassN}
            /// @param samples: matrix of size FeatureN*x. Each row is one feature, each column is one sample
            void train(const FeatureMatrix_t&, const ClassificationMatrix_t&);

            /// @brief classify a feature vector
            /// @param samples: matrix of size Feature*x, where each row i one feature, each column one sample
            /// @returns matrix of size ClassN*x, each column is one sample. The row with the *highest* value is the predicted classification
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