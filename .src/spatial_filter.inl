// 
// Copyright 2021 Clemens Cords
// Created on 09.09.21 by clem (mail@clemens-cords.com)
//

#include <spatial_filter.hpp>

namespace crisp
{
    Kernel convolute(Kernel left, Kernel right)
    {
        auto size = Vector2f{float(left.rows()), float(left.cols())};

        assert(left.cols() >= right.cols() and left.rows() >= right.rows());

        Kernel result;
        result.resize(size.x(), size.y());

        long a = floor(right.rows() / 2.f);
        long b = floor(right.cols() / 2.f);

        for (long i = 0; i < size.x(); ++i)
        {
            for (long j = 0; j < size.y(); ++j)
            {
                float current_sum = 0;
                for (long s = -a; s <= a; ++s)
                {
                    for (long t = -b; t <= b; ++t)
                    {
                        if (i + s < 0 or j + t < 0 or i + s >= left.rows() or j + t >= left.cols())
                            continue;

                        auto test = right(a + s, b + t);
                        auto test2 = left(i + s, j + t);
                    }
                }
                result(i, j) = current_sum;
            }
        }

        return result;
    }

    bool seperate(const Kernel& original, Kernel* out_left, Kernel* out_right)
    {
        auto svd = Eigen::JacobiSVD<Kernel, Eigen::ColPivHouseholderQRPreconditioner>(original, Eigen::ComputeThinU | Eigen::ComputeThinV);

        auto s = svd.singularValues()(0);

        auto singular_sum = 0;
        for(size_t i = 0; i < svd.singularValues().size(); ++i)
            singular_sum += svd.singularValues()(i);

        if (abs(s - singular_sum) < 0.005)
        {
            out_left = nullptr;
            out_right = nullptr;
            return false;
        }

        auto U = svd.matrixU();
        auto u = Eigen::Vector3d();
        for (size_t i = 0; i < U.rows(); ++i)
            u(i) = U(i, 0) * s;

        auto V = svd.matrixV();
        auto v = Eigen::Vector3d();
        for (size_t i = 0; i < V.cols(); ++i)
            v(i) = V(i, 0);

        *out_left = u.cast<float>();
        *out_right = v.transpose().cast<float>();
        return true;
    }

    void normalize(Kernel& kernel)
    {
        float sum = 0;
        for (long i = 0; i < kernel.rows(); ++i)
            for (long j = 0; j < kernel.rows(); ++j)
                sum += kernel(i, j);

        for (long i = 0; i < kernel.rows(); ++i)
            for (long j = 0; j < kernel.rows(); ++j)
                kernel(i, j) /= sum;
    }

    void rotate(Kernel& kernel, size_t n)
    {
        n = n % 4;
        if (n == 0 or n == 4)
            return;

        long size = kernel.cols();

        while (n > 0)
        {
            for (long x = 0; x < size / 2; ++x)
            {
                for (long y = x; y < size - x - 1; ++y)
                {
                    float temp = kernel(x, y);
                    kernel(x, y) = kernel(y, size - 1 - x);
                    kernel(y, size - 1 - x) = kernel(size - 1 - x, size - 1 - y);
                    kernel(size - 1 - x, size - 1 - y) = kernel(size - 1 - y, x);
                    kernel(size - 1 - y, x) = temp;
                }
            }
            n -= 1;
        }
    }

    
    SpatialFilter::SpatialFilter()
    {
        set_kernel(identity(1));
        set_evaluation_function(CONVOLUTION);
    }

    template<typename Image_t>
    void SpatialFilter::apply_to(Image_t& image)
    {
        Image_t result;
        result.create(image.get_size().x(), image.get_size().y());

        switch (_evaluation_function)
        {
            case CONVOLUTION:
                apply_convolution_to(image, result);
                break;

            case NORMALIZED_CONVOLUTION:
                apply_normalized_convolution_to(image, result);
                break;

            case MINIMUM:
                apply_min_to(image, result);
                break;

            case MAXIMUM:
                apply_max_to(image, result);
                break;
        }

        for (long x = 0; x < image.get_size().x(); ++x)
            for (long y = 0; y < image.get_size().y(); ++y)
                image(x, y) = result(x, y);
    }

    
    float SpatialFilter::operator()(size_t x, size_t y) const
    {
        return _kernel(x, y);
    }

    
    float& SpatialFilter::operator()(size_t x, size_t y)
    {
        return _kernel(x, y);
    }

    
    void SpatialFilter::set_kernel(Kernel kernel)
    {
        _kernel = kernel;

        _kernel_sum = 0;
        for (size_t x = 0; x < _kernel.rows(); ++x)
            for (size_t y = 0; y < _kernel.cols(); ++y)
                _kernel_sum += _kernel(x, y);
    }

    
    Kernel& SpatialFilter::get_kernel()
    {
        return _kernel;
    }

    
    void SpatialFilter::set_evaluation_function(SpatialFilter::EvaluationFunction function)
    {
        _evaluation_function = function;
    }

    
    Kernel SpatialFilter::identity(size_t dimensions)
    {
        assert(dimensions != 0);
        Kernel out;
        out.resize(dimensions, dimensions);
        out.setConstant(0);
        out((dimensions - 1) / 2, (dimensions - 1) / 2) = 1;
        return out;
    }

    
    Kernel SpatialFilter::zero(size_t dimensions)
    {
        assert(dimensions != 0);
        Kernel out;
        out.resize(dimensions, dimensions);
        out.setConstant(0);
        return out;
    }

    
    Kernel SpatialFilter::one(size_t dimensions)
    {
        assert(dimensions != 0);
        Kernel out;
        out.resize(dimensions, dimensions);
        out.setConstant(1);
        return out;
    }

    
    Kernel SpatialFilter::box(size_t dimensions, float value)
    {
        assert(dimensions != 0);
        Kernel out;
        out.resize(dimensions, dimensions);
        out.setConstant(value);
        return out;
    }

    
    Kernel SpatialFilter::normalized_box(size_t dimension)
    {
        assert(dimension != 0);
        return box(dimension, 1.f / float(dimension));
    }

    Kernel SpatialFilter::gaussian(size_t dimension)
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

    
    Kernel SpatialFilter::laplacian_first_derivative()
    {
        Kernel out;
        out.resize(3, 3);

        out << -1, -1, -1,
               -1,  8, -1,
               -1  -1, -1;

        return out;
    }

    
    Kernel SpatialFilter::laplacian_second_derivative()
    {
        Kernel out;
        out.resize(3, 3);

        out << -9, -1, -9,
               -1, 72, -1,
               -9, -1, -9;

        return out;
    }

    
    Kernel SpatialFilter::simple_gradient_x()
    {
        Kernel out;
        out.resize(1, 2);
        out(0, 0) = -1;
        out(0, 1) = 1;

        return out;
    }

    
    Kernel SpatialFilter::simple_gradient_y()
    {
        Kernel out;
        out.resize(2, 1);
        out(0, 0) = -1;
        out(1, 0) = 1;

        return out;
    }

    
    Kernel SpatialFilter::roberts_gradient_x()
    {
        Kernel out;
        out.resize(2, 2);

        out << -1, 0,
                0, 1;

        return out;
    }

    
    Kernel SpatialFilter::roberts_gradient_y()
    {
        Kernel out;
        out.resize(2, 2);

        out <<  0, -1,
                1,  0;

        return out;
    }

    
    Kernel SpatialFilter::prewitt_gradient_x()
    {
        Kernel out;
        out.resize(3, 3);

        out << -1, -1, -1,
                0,  0,  0,
                1,  1,  1;

        return out;
    }

    
    Kernel SpatialFilter::prewitt_gradient_y()
    {
        Kernel out;
        out.resize(3, 3);

        out << -1,  0,  1,
               -1,  0,  1,
               -1,  0,  1;

        return out;
    }

    
    Kernel SpatialFilter::sobel_gradient_x()
    {
        Kernel out;
        out.resize(3, 3);

        out << -1, -2, -1,
                0,  0,  0,
                1,  2,  1;

        return out;
    }

    
    Kernel SpatialFilter::sobel_gradient_y()
    {
        Kernel out;
        out.resize(3, 3);

        out << -1,  0,  1,
               -2,  0,  2,
               -1,  0,  1 ;

        return out;
    }

    
    Kernel SpatialFilter::kirsch_compass_n()
    {
        Kernel out;
        out.resize(3, 3);

        out << -3, -3,  5,
               -3,  0,  5,
               -3, -3,  5;

        return out;
    }

    
    Kernel SpatialFilter::kirsch_compass_nw()
    {
        Kernel out;
        out.resize(3, 3);

        out << -3,  5,  5,
               -3,  0,  5,
               -3, -3, -3;

        return out;
    }

    
    Kernel SpatialFilter::kirsch_compass_w()
    {
        Kernel out;
        out.resize(3, 3);

        out <<  5,  5,  5,
               -3,  0, -3,
               -3, -3, -3;

        return out;
    }

    
    Kernel SpatialFilter::kirsch_compass_sw()
    {
        Kernel out;
        out.resize(3, 3);

        out <<  5,  5, -3,
                5,  0, -3,
               -3, -3, -3;

        return out;
    }

    
    Kernel SpatialFilter::kirsch_compass_s()
    {
        Kernel out;
        out.resize(3, 3);

        out <<  5, -3, -3,
                5,  0, -3,
                5, -3, -3;

        return out;
    }

    
    Kernel SpatialFilter::kirsch_compass_se()
    {
        Kernel out;
        out.resize(3, 3);

        out << -3, -3, -3,
                5,  0, -3,
                5,  5, -3;

        return out;
    }

    
    Kernel SpatialFilter::kirsch_compass_e()
    {
        Kernel out;
        out.resize(3, 3);

        out << -3, -3, -3,
               -3,  0, -3,
                5,  5,  5;

        return out;
    }

    
    Kernel SpatialFilter::kirsch_compass_ne()
    {
        Kernel out;
        out.resize(3, 3);

        out << -3, -3, -3,
               -3,  0,  5,
               -3,  5,  5;

        return out;
    }

    
    Kernel SpatialFilter::laplacian_of_gaussian(size_t dimension)
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
    void SpatialFilter::apply_convolution_to(Image_t& in, Image_t& out)
    {
        long a = floor(_kernel.rows() / 2);
        long b = floor(_kernel.cols() / 2);

        bool rows_even = _kernel.rows() % 2 == 0,
             cols_even = _kernel.cols() % 2 == 0;

        using Value_t = typename Image_t::Value_t;

        for (size_t y = 0; y < in.get_size().y(); ++y)
        {
            for (size_t x = 0; x < in.get_size().x(); ++x)
            {
                Value_t current_sum = Value_t(0.f);
                for (long s = -a; rows_even ? s < a : s <= a; ++s)
                {
                    for (long t = -b; cols_even ? t < b : t <= b; ++t)
                    {
                        if (abs(_kernel(a + s, b + t)) < std::numeric_limits<float>::epsilon())
                            continue;

                        current_sum += _kernel(a + s, b + t) * in(x + s, y + t);
                    }
                }

                out(x, y) = current_sum;
            }
        }
    }

    template<typename Image_t>
    void SpatialFilter::apply_normalized_convolution_to(Image_t& in, Image_t& out)
    {
        long a = floor(_kernel.rows() / 2);
        long b = floor(_kernel.cols() / 2);

        bool rows_even = _kernel.rows() % 2 == 0,
             cols_even = _kernel.cols() % 2 == 0;

        using Value_t = typename Image_t::Value_t;

        for (size_t y = 0; y < in.get_size().y(); ++y)
        {
            for (size_t x = 0; x < in.get_size().x(); ++x)
            {
                Value_t current_sum = Value_t(0.f);
                for (long s = -a; rows_even ? s < a : s <= a; ++s)
                {
                    for (long t = -b; cols_even ? t < b : t <= b; ++t)
                    {
                        if (abs(_kernel(a + s, b + t) - 0) < 0.000001)
                            continue;

                        current_sum += _kernel(a + s, b + t) * in(x + s, y + t);
                    }
                }

                out(x, y) = current_sum / _kernel_sum;
            }
        }
    }

    template<typename Image_t>
    void SpatialFilter::apply_min_to(Image_t& in, Image_t& out)
    {
        long a = floor(_kernel.rows() / 2);
        long b = floor(_kernel.cols() / 2);

        bool rows_even = _kernel.rows() % 2 == 0,
             cols_even = _kernel.cols() % 2 == 0;

        using ImageValue_t = typename Image_t::Value_t;
        using Inner_t = typename Image_t::Value_t::Value_t;

        for (size_t y = 0; y < in.get_size().y(); ++y)
        {
            for (size_t x = 0; x < in.get_size().x(); ++x)
            {
                ImageValue_t min_vec;
                for (size_t i = 0; i < ImageValue_t::size(); ++i)
                {
                    Inner_t current = std::numeric_limits<Inner_t>::max();

                    for (long s = -a; s <= a; ++s)
                        for (long t = -b; t <= b; ++t)
                            current = std::min<float>(_kernel(s + a, t + b) * in(x + a, y + b).at(i), float(current));

                    min_vec.at(i) = current;
                }

                out(x, y) = min_vec;
            }
        }
    }

    template<typename Image_t>
    void SpatialFilter::apply_max_to(Image_t& in, Image_t& out)
    {
        long a = floor(_kernel.rows() / 2);
        long b = floor(_kernel.cols() / 2);

        bool rows_even = _kernel.rows() % 2 == 0,
             cols_even = _kernel.cols() % 2 == 0;

        using ImageValue_t = typename Image_t::Value_t;
        using Inner_t = typename Image_t::Value_t::Value_t;

        for (size_t y = 0; y < in.get_size().y(); ++y)
        {
            for (size_t x = 0; x < in.get_size().x(); ++x)
            {
                ImageValue_t max_vec;
                for (size_t i = 0; i < ImageValue_t::size(); ++i)
                {
                    Inner_t current = std::numeric_limits<Inner_t>::min();

                    for (long s = -a; s <= a; ++s)
                        for (long t = -b; t <= b; ++t)
                            current = std::max<float>(_kernel(s + a, t + b) * in(x + a, y + b).at(i), float(current));

                    max_vec.at(i) = current;
                }

                out(x, y) = max_vec;
            }
        }
    }
}