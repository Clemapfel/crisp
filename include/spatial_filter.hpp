// 
// Copyright 2021 Clemens Cords
// Created on 09.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>
#include <image/color_image.hpp>

#include <Dense>

namespace crisp
{
    using Kernel = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;
    
    template<typename Image_t>
    class SpatialFilter 
    {
        public:
            using EvaluationFunction_t = std::function<typename Image_t::Value_t(const Image_t&, size_t, size_t, const Kernel&)>;

            SpatialFilter();

            // @brief apply filter to image
            void apply_to(Image_t&);

            // @brief set the filters evaluation function, convolution by default
            void set_evaluation_function(EvaluationFunction_t&&);

            // @brief set the filters kernel, identity by default
            void set_kernel(Kernel);

            // @brief expose the filters kernel
            Kernel& get_kernel();

            // @brief access kernel elements
            float& operator()(size_t x, size_t y);
            const float operator()(size_t x, size_t y) const;

            // kernels
            static Kernel identity(size_t dimensions);
            static Kernel one(size_t dimensions);
            static Kernel zero(size_t dimensions);

            static Kernel box(size_t dimensions, float value);
            static Kernel normalized_box(size_t dimensions);
            static Kernel gaussian(size_t dimensions);

            static Kernel laplacian_first_derivative();
            static Kernel laplacian_second_derivative();

            static Kernel laplacian_of_gaussian(long dimensions);
            
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

            // evaluation functions

            // @brief applies the kernel by convolving it with the image such that pixel = kernel(s, t) * image(x + s, y + t)
            // @note no normalization will take place. To normalize the result, normalize the kernel via crisp::normalize
            static auto&& convolution();

            // @brief applies the kernel by returning the mean/median/min/max/ of it's weighted sum
            static auto&& mean();
            static auto&& median();
            static auto&& min();
            static auto&& max();
            static auto&& n_ths_k_quantile(size_t n, size_t k);

        private:
            Kernel _kernel;
            EvaluationFunction_t _function;
    };

    using GrayScaleFilter = SpatialFilter<GrayScaleImage>;
    using ColorFilter = SpatialFilter<ColorImage>
}