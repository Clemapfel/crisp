// 
// Copyright 2021 Clemens Cords
// Created on 09.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>
#include <image/color_image.hpp>
#include <image/grayscale_image.hpp>

#include <Dense>

namespace crisp
{
    using Kernel = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;

    // @brief convolute two kernels
    Kernel convolute(Kernel left, Kernel right);

    // @brief numerically seperate kernel
    bool seperate(const Kernel& original, Kernel* out_left, Kernel* out_right);

    // @brief normalize a kernel so it's elements sum to 1
    void normalize(Kernel&);

    // @brief rotate a kernel by 90° n times
    void rotate(Kernel&, size_t n_90_degrees);

    class SpatialFilter
    {
        public:
            enum EvaluationFunction
            {
                CONVOLUTION,
                NORMALIZED_CONVOLUTION,
                MINIMUM,
                MAXIMUM,
            };

            SpatialFilter();

            // @brief apply filter to image
            template<typename Image_t>
            void apply_to(Image_t&);

            void set_evaluation_function(EvaluationFunction);

            // @brief set the filters kernel, identity by default
            void set_kernel(Kernel);

            // @brief expose the filters kernel
            Kernel& get_kernel();

            // @brief access kernel elements
            float& operator()(size_t x, size_t y);
            float operator()(size_t x, size_t y) const;

            // kernels
            static Kernel identity(size_t dimensions);
            static Kernel one(size_t dimensions);
            static Kernel zero(size_t dimensions);

            static Kernel box(size_t dimensions, float value);
            static Kernel normalized_box(size_t dimensions);
            static Kernel gaussian(size_t dimensions);

            static Kernel laplacian_first_derivative();
            static Kernel laplacian_second_derivative();

            static Kernel laplacian_of_gaussian(size_t dimensions);

            static Kernel simple_gradient_x();
            static Kernel simple_gradient_y();

            static Kernel roberts_gradient_x();
            static Kernel roberts_gradient_y();

            static Kernel prewitt_gradient_x();
            static Kernel prewitt_gradient_y();

            static Kernel sobel_gradient_x();
            static Kernel sobel_gradient_y();

            static Kernel kirsch_compass_n();
            static Kernel kirsch_compass_nw();
            static Kernel kirsch_compass_w();
            static Kernel kirsch_compass_sw();
            static Kernel kirsch_compass_s();
            static Kernel kirsch_compass_se();
            static Kernel kirsch_compass_e();
            static Kernel kirsch_compass_ne();

        private:
            Kernel _kernel;
            float _kernel_sum;

            EvaluationFunction _evaluation_function;

            template<typename Image_t>
            void apply_convolution_to(Image_t& in, Image_t& out);

            template<typename Image_t>
            void apply_normalized_convolution_to(Image_t& in, Image_t& out);

            template<typename Image_t>
            void apply_min_to(Image_t& in, Image_t& out);

            template<typename Image_t>
            void apply_max_to(Image_t& in, Image_t& out);
    };
}

#include ".src/spatial_filter.inl"