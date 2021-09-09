// 
// Copyright 2021 Clemens Cords
// Created on 09.09.21 by clem (mail@clemens-cords.com)
//

#include <spatial_filter.hpp>

namespace crisp
{
    template<typename Image_t>
    SpatialFilter<Image_t>::SpatialFilter()
    {
        set_kernel(identity(1));
        set_evaluation_function(convolution());
    }

    template<typename Image_t>
    void SpatialFilter<Image_t>::apply_to(Image_t& image)
    {
        Image_t result;
        result.create(image.get_size().x(), image.get_size().y());

        for (long x = 0; x < image.get_size().x(); ++x)
            for (long y = 0; y < image.get_size().y(); ++y)
                result(x, y) = _evaluation_function(image, x, y, _kernel);

        for (long x = 0; x < image.get_size().x(); ++x)
            for (long y = 0; y < image.get_size().y(); ++y)
                image(x, y) = result(x, y);
    }

    template<typename Image_t>
    float SpatialFilter<Image_t>::operator()(size_t x, size_t y) const
    {
        return _kernel(x, y);
    }

    template<typename Image_t>
    float& SpatialFilter<Image_t>::operator()(size_t x, size_t y)
    {
        return _kernel(x, y);
    }

    template<typename Image_t>
    void SpatialFilter<Image_t>::set_kernel(Kernel kernel)
    {
        _kernel = kernel;
    }

    template<typename Image_t>
    Kernel& SpatialFilter<Image_t>::get_kernel()
    {
        return _kernel;
    }

    template<typename Image_t>
    void SpatialFilter<Image_t>::set_evaluation_function(SpatialFilter::EvaluationFunction_t&& lambda)
    {
        _evaluation_function = lambda;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::identity(size_t dimensions)
    {
        assert(dimensions != 0);
        Kernel out;
        out.resize(dimensions, dimensions);
        out.setConstant(0);
        out((dimensions - 1) / 2, (dimensions - 1) / 2) = 1;
        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::zero(size_t dimensions)
    {
        assert(dimensions != 0);
        Kernel out;
        out.resize(dimensions, dimensions);
        out.setConstant(0);
        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::one(size_t dimensions)
    {
        assert(dimensions != 0);
        Kernel out;
        out.resize(dimensions, dimensions);
        out.setConstant(1);
        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::box(size_t dimensions, float value)
    {
        assert(dimensions != 0);
        Kernel out;
        out.resize(dimensions, dimensions);
        out.setConstant(value);
        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::normalized_box(size_t dimension)
    {
        assert(dimension != 0);
        return box(dimension, 1.f / float(dimension));
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::gaussian(size_t dimension)
    {
        assert(dimension != 0);
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> matrix;
        matrix.resize(dimension, dimension);

        auto square = [](double value) {return value * value;};

        double sum = 0;
        double sigma_sq = double(dimension);
        double gauss_factor = 1.f / sqrt(2 * M_PI + sigma_sq);
        double center = ceil(double(dimension) / 2.f);

        for (size_t x = 0; x < dimension; ++x)
        {
            for (size_t y = 0; y < dimension; ++y)
            {
                double length = sqrt(square(x - center) + square(y - center));
                matrix(x, y) = gauss_factor * exp(-1 * length / (2 * sigma_sq));
                sum += matrix(x, y);
            }
        }

        Kernel out;
        out.resize(dimension, dimension);

        // normalize and convert
        for (size_t x = 0; x < dimension; ++x)
            for (size_t y = 0; y < dimension; ++y)
                out(x, y) = matrix(x, y) / sum;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::laplacian_first_derivative()
    {
        Kernel out;
        out.resize(3, 3);

        out << -1, -1, -1,
               -1,  8, -1,
               -1  -1, -1;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::laplacian_second_derivative()
    {
        Kernel out;
        out.resize(3, 3);

        out << -9, -1, -9,
               -1, 72, -1,
               -9, -1, -9;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::simple_gradient_x()
    {
        Kernel out;
        out.resize(1, 2);
        out(0, 0) = -1;
        out(0, 1) = 1;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::simple_gradient_y()
    {
        Kernel out;
        out.resize(2, 1);
        out(0, 0) = -1;
        out(1, 0) = 1;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::roberts_gradient_x()
    {
        Kernel out;
        out.resize(2, 2);

        out << -1, 0,
                0, 1;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::roberts_gradient_y()
    {
        Kernel out;
        out.resize(2, 2);

        out <<  0, -1,
                1,  0;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::prewitt_gradient_x()
    {
        Kernel out;
        out.resize(3, 3);

        out << -1, -1, -1,
                0,  0,  0,
                1,  1,  1;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::prewitt_gradient_y()
    {
        Kernel out;
        out.resize(3, 3);

        out << -1,  0,  1,
               -1,  0,  1,
               -1,  0,  1;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::sobel_gradient_x()
    {
        Kernel out;
        out.resize(3, 3);

        out << -1, -2, -1,
                0,  0,  0,
                1,  2,  1;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::sobel_gradient_y()
    {
        Kernel out;
        out.resize(3, 3);

        out << -1,  0,  1,
               -2,  0,  2,
               -1,  0,  1 ;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::kirsch_compass_n()
    {
        Kernel out;
        out.resize(3, 3);

        out << -3, -3,  5,
               -3,  0,  5,
               -3, -3,  5;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::kirsch_compass_nw()
    {
        Kernel out;
        out.resize(3, 3);

        out << -3,  5,  5,
               -3,  0,  5,
               -3, -3, -3;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::kirsch_compass_w()
    {
        Kernel out;
        out.resize(3, 3);

        out <<  5,  5,  5,
               -3,  0, -3,
               -3, -3, -3;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::kirsch_compass_sw()
    {
        Kernel out;
        out.resize(3, 3);

        out <<  5,  5, -3,
                5,  0, -3,
               -3, -3, -3;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::kirsch_compass_s()
    {
        Kernel out;
        out.resize(3, 3);

        out <<  5, -3, -3,
                5,  0, -3,
                5, -3, -3;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::kirsch_compass_se()
    {
        Kernel out;
        out.resize(3, 3);

        out << -3, -3, -3,
                5,  0, -3,
                5,  5, -3;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::kirsch_compass_e()
    {
        Kernel out;
        out.resize(3, 3);

        out << -3, -3, -3,
               -3,  0, -3,
                5,  5,  5;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::kirsch_compass_ne()
    {
        Kernel out;
        out.resize(3, 3);

        out << -3, -3, -3,
               -3,  0,  5,
               -3,  5,  5;

        return out;
    }

    template<typename Image_t>
    Kernel SpatialFilter<Image_t>::laplacian_of_gaussian(size_t dimension)
    {
        assert(dimension != 0);
        Kernel matrix;
        matrix.resize(dimension, dimension);

        auto square = [](double value) {return value * value;};

        double sum = 0;
        double sigma_sq = dimension * 2;
        double center = ceil(double(dimension) / 2.f);

        float min = std::numeric_limits<float>::max(),
              max = std::numeric_limits<float>::min();

        for (long x = 0; x < dimension; ++x)
        {
            for (long y = 0; y < dimension; ++y)
            {
                float x_sq = (x - center)*(x - center);
                float y_sq = (y - center)*(y - center);

                matrix(x, y) = -1*((x_sq + y_sq - 2*sigma_sq) / (sigma_sq * sigma_sq)) * exp(-(x_sq + y_sq) / (2*sigma_sq));
                sum += matrix(x, y);

                min = std::min(min, matrix(x, y));
                max = std::max(max, matrix(x, y));
            }
        }

        return matrix;
    }

    template<typename Image_t>
    auto && SpatialFilter<Image_t>::convolution()
    {
        return std::move([](const Image_t& image, long x, long y, const Kernel& kernel) -> typename Image_t::Value_t
        {
            long a = floor(kernel.rows() / 2);
            long b = floor(kernel.cols() / 2);

            using Value_t = typename Image_t::Value_t;

            Value_t current_sum = Value_t(0.f);
            for (long s = -a; s <= a; ++s)
            {
                for (long t = -b; t <= b; ++t)
                {
                    if (abs(kernel(a + s, b + t) - 0) < 0.000001)
                        continue;

                    auto first = kernel(a + s, b + t);
                    auto two = image(x + s, y + t);
                    current_sum += first * two;
                }
            }

            return current_sum;
        });
    }

    template<typename Image_t>
    auto && SpatialFilter<Image_t>::min()
    {
        return std::move([](const Image_t& image, long x, long y, const Kernel& kernel) -> typename Image_t::Value_t
        {
            long a = (kernel.cols() - 1) / 2;
            long b = (kernel.rows() - 1) / 2;

            using ImageValue_t = typename Image_t::Value_t;
            using Inner_t = typename Image_t::Value_t::Value_t;

            ImageValue_t out;
            for (size_t i = 0; i < ImageValue_t::size(); ++i)
            {
                Inner_t current = std::numeric_limits<Inner_t>::max();

                for (long s = -a; s <= a; ++s)
                    for (long t = -b; t <= b; ++t)
                        current = std::min<float>(kernel(s + a, t + b) * image(x + a, y + b).at(i), float(current));

                out.at(i) = current;
            }

            return out;
        });
    }

    template<typename Image_t>
    auto && SpatialFilter<Image_t>::max()
    {
        return std::move([](const Image_t& image, long x, long y, const Kernel& kernel) -> typename Image_t::Value_t
        {
            long a = (kernel.cols() - 1) / 2;
            long b = (kernel.rows() - 1) / 2;

            using ImageValue_t = typename Image_t::Value_t;
            using Inner_t = typename Image_t::Value_t::Value_t;

            ImageValue_t out;
            for (size_t i = 0; i < ImageValue_t::size(); ++i)
            {
                Inner_t current = std::numeric_limits<Inner_t>::min();

                for (long s = -a; s <= a; ++s)
                    for (long t = -b; t <= b; ++t)
                        current = std::max<float>(kernel(s + a, t + b) * image(x + a, y + b).at(i), float(current));

                out.at(i) = current;
            }

            return out;
        });
    }

    template<typename Image_t>
    auto && SpatialFilter<Image_t>::mean()
    {
        return SpatialFilter<Image_t>::convolution();
    }

    template<typename Image_t>
    auto && SpatialFilter<Image_t>::median()
    {
        return std::move([](const Image_t& image, long x, long y, const Kernel& kernel) -> typename Image_t::Value_t
        {
            long a = (kernel.cols() - 1) / 2;
            long b = (kernel.rows() - 1) / 2;

            using ImageValue_t = typename Image_t::Value_t;
            using Inner_t = typename Image_t::Value_t::Value_t;

            ImageValue_t out;
            for (size_t i = 0; i < ImageValue_t::size(); ++i)
            {
                std::vector<float> values;
                values.reserve(kernel.rows()*kernel.cols());

                for (long s = -a; s <= a; ++s)
                    for (long t = -b; t <= b; ++t)
                        values.push_back(kernel(s + a, t + b) * image(x + a, y + b).at(i));

                if (values.size() % 2 == 0)
                    out.at(i) = (values.at(floor(values.size() / 2.f)) + values.at(ceil(values.size() / 2.f))) / 2;
                else
                    out.at(i) = values.at(values.size() / 2 + 1);
            }

            return out;
        });
    }

    template<typename Image_t>
    auto && SpatialFilter<Image_t>::n_ths_k_quantile(
            size_t n, size_t k)
    {
        return std::move([n, k](const Image_t& image, long x, long y, const Kernel& kernel) -> float
        {
            assert(kernel.rows() % 2 == 1 and kernel.cols() % 2 == 1 && "Kernel needs to have odd dimensions");

            long a = (kernel.cols() - 1) / 2;
            long b = (kernel.rows() - 1) / 2;

            std::vector<float> values;
            values.reserve(kernel.rows()*kernel.cols());

            for (long s = -a; s <= a; ++s)
                for (long t = -b; t <= b; ++t)
                    values.push_back(kernel(s + a, t + b) * image(x + a, y + b));

            if (n >= k)
                return values.back();
            else if (n <= 0)
                return values.front();
            else
                return values.at(size_t(values.size() * (float(n) / float(k))));
        });
    }
}