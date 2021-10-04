// 
// Copyright 2021 Clemens Cords
// Created on 03.10.21 by clem (mail@clemens-cords.com)
//

#include <classification/bayes_classifier.hpp>

namespace crisp
{
    template<size_t FeatureN, size_t ClassN>
    typename BayesClassifier<FeatureN, ClassN>::ClassificationMatrix_t BayesClassifier<FeatureN, ClassN>::identify(const FeatureMatrix_t& input)
    {
        ClassificationMatrix_t out;
        out.resize(ClassN, input.cols());

        float min = std::numeric_limits<float>::max();
        for (size_t col_i = 0; col_i < input.cols(); ++col_i)
        {
            const auto& pattern = input.block(0, col_i, 3, 1);

            for (size_t class_i = 0; class_i < ClassN; ++class_i)
            {
                out(class_i, col_i) = 1;

                const auto& c = _classes.at(class_i);
                for (size_t row_i = 0; row_i < FeatureN; row_i++)
                {
                    auto variance = c.variance(row_i, 0);
                    auto mean = c.mean(row_i, 0);
                    out(class_i, col_i) *= 1 / sqrt(variance) * exp(-1 * pow(pattern(row_i, 0) - mean, 2) / (2 * variance));
                }
            }
        }

        return out;
    }

    template<size_t FeatureN, size_t ClassN>
    void BayesClassifier<FeatureN, ClassN>::train(const BayesClassifier<FeatureN, ClassN>::FeatureMatrix_t & features, const ClassificationMatrix_t& classes)
    {
        assert(features.cols() == classes.cols());

        for (auto& c : _classes)
        {
            c.variance.setConstant(0);
            c.mean.setConstant(0);
            c.n = 1;
        }

        // estimate mean
        for (size_t col_i = 0; col_i < features.cols(); ++col_i)
        {
            auto& pattern = features.block(0, col_i, features.rows(), 1);

            for (size_t row_i = 0; row_i < classes.rows(); ++row_i)
            {
                if (bool(classes(row_i, col_i)))
                {
                    auto& c = _classes.at(row_i);
                    c.mean += pattern;
                    c.n += 1;
                }
            }
        }

        for (auto& c : _classes)
        {
            c.mean /= c.n;
        }

        // estimate var
        for (size_t col_i = 0; col_i < features.cols(); ++col_i)
        {
            auto& pattern = features.block(0, col_i, features.rows(), 1);

            for (size_t row_i = 0; row_i < classes.rows(); ++row_i)
            {
                if (bool(classes(row_i, col_i)))
                {
                    auto& c = _classes.at(row_i);
                    c.variance += (pattern - c.mean).cwiseProduct(pattern - c.mean);
                }
            }
        }

        auto mean_variance = Eigen::MatrixXf(FeatureN, 1);
        mean_variance.setConstant(0);

        for (auto& c : _classes)
        {
            c.variance /= c.n;
            mean_variance += c.variance;
        }

        mean_variance /= _classes.size();

        // estimate if variance is 0
        for (auto& c : _classes)
        {
            for (size_t row_i = 0; row_i < c.variance.size(); ++row_i)
                if (c.variance(row_i, 0) == 0)
                    c.variance(row_i, 0) = mean_variance(row_i, 0);
        }
    }
}