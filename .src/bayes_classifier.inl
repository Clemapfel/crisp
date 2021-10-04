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
    void BayesClassifier<FeatureN, ClassN>::train(size_t class_i, const BayesClassifier<FeatureN, ClassN>::FeatureMatrix_t & input)
    {
        assert(class_i < ClassN && "Class Index out of Range");

        auto& c = _classes.at(class_i);
        auto m = Eigen::MatrixXf(FeatureN, c.samples.cols() + input.cols());
        m << c.samples, input;

        auto mean = Eigen::MatrixXf(FeatureN, 1);
        mean.setConstant(0);

        for (size_t col_i = 0; col_i < m.cols(); ++col_i)
        {
            for (size_t row_i = 0; row_i < FeatureN; ++row_i)
                mean(row_i, 0) += m(row_i, col_i);
        }

        mean /= float(m.cols());

        auto variance = mean;

        for (size_t col_i = 0; col_i < m.cols(); ++col_i)
        {
            for (size_t row_i = 0; row_i < FeatureN; ++row_i)
                variance(row_i, 0) += powf((m(row_i, col_i) - mean(row_i, 0)), 2);
        }

        variance /= float(m.cols());

        // add pseudocount
        for (size_t row_i = 0; row_i < FeatureN; ++row_i)
            if (variance(row_i, 0) == 0)
                variance(row_i, 0) = 1 / float(m.cols());

        c.mean = mean;
        c.variance = variance;
        c.samples = m;
        c.trained = true;

        return;
    }
}