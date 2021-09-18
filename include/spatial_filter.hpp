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
    /// @brief filter kernel, float matrix of uneven size
    using Kernel = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;

    /// @brief convolute two kernels
    /// @param left: kernel as left hand of expression
    /// @param right: kernel as right hand of expression
    /// @returns resulting kernel
    Kernel convolute(Kernel left, Kernel right);

    /// @brief numerically seperate kernel
    /// @param original: [in] kernel to be seperated
    /// @param out_left: [out] resulting left-hand kernel
    /// @param out_right: [out] result right-hand kernel
    /// @returns bool if seperation was possible, false otherwise
    /// @note if no seperation took place, out_left and out_right will be set to nullptr
    bool seperate(const Kernel& original, Kernel* out_left, Kernel* out_right);

    /// @brief normalize a kernel so it's elements sum to 1
    /// @param kernel: kernel to be normalized
    void normalize(Kernel&);

    /// @brief rotate a kernel by 90° n times counter-clockwise
    /// @param kernel: kernel to be rotated
    /// @param n_90_degrees: number of 90° rotations
    void rotate(Kernel&, size_t n_90_degrees);

    /// @brief object representing a filter operating in the spatial domain
    class SpatialFilter
    {
        public:
            /// @brief evalution function used for filtering
            enum EvaluationFunction
            {
                /// @brief compute sum weighted by kernel
                CONVOLUTION = 0,

                /// @brief compute sum weighted by kernel and divide by sum of kernel elements
                NORMALIZED_CONVOLUTION = 1,

                /// @brief compute minimum of elements weighted by kernel in kernel-sized neighborhood
                MINIMUM = 2,

                /// @brief compute maximum of elements weighted by kernel in kernel-sized neighborhood
                MAXIMUM = 3,

                /// @brief compute mean of elements weighted by kernel in kernel-sized neighborhood
                MEAN = 4,

                /// @brief compute median of elements weighted by kernel in kernel-sized neighborhood
                MEDIAN = 5
            };

            SpatialFilter();

            /// @brief apply filter to image
            /// @param image
            template<typename Image_t>
            void apply_to(Image_t&);

            /// @brief specify evaluation function
            /// @param evaluation_function
            void set_evaluation_function(EvaluationFunction);

            /// @brief set the filters kernel, identity by default
            /// @param kernel
            void set_kernel(Kernel);

            /// @brief expose the filters kernel
            /// @returns reference to kernel
            Kernel& get_kernel();

            /// @brief access kernel elements
            /// @param x: row index
            /// @param y: col index
            /// @return reference to kernel element
            float& operator()(size_t x, size_t y);

            /// @brief access kernel elements
            /// @param x: row index
            /// @param y: col index
            /// @return copy of kernel element
            float operator()(size_t x, size_t y) const;

            /// @brief identiy kernel
            /// @param dimensions: size of kernel
            /// @returns dimension*dimensions sized kernel with center element 1, 0 otherwise
            static Kernel identity(size_t dimensions);

            /// @brief kernel with all components as 1
            /// @param dimensions: size of kernel
            /// @returns dimension*dimensions sized kernel
            static Kernel one(size_t dimensions);

            /// @brief kernel with all components as 0
            /// @param dimensions: size of kernel
            /// @returns dimension*dimensions sized kernel
            static Kernel zero(size_t dimensions);

            /// @brief kernel with all components as value
            /// @param dimensions: size of kernel
            /// @param value: value of components
            /// @returns dimension*dimensions sized kernel
            static Kernel box(size_t dimensions, float value);

            /// @brief kernel with all components as 1/dimensions
            /// @param dimensions: size of kernel
            /// @returns dimension*dimensions sized kernel
            static Kernel normalized_box(size_t dimensions);

            /// @brief kernel that samples normalized gaussian function
            /// @param dimensions: size of kernel
            /// @returns dimension*dimensions sized kernel
            static Kernel gaussian(size_t dimensions);

            /// @brief kernel that computes the laplacian first derivative in all directions
            /// @returns 3x3 kernel
            static Kernel laplacian_first_derivative();

            /// @brief kernel that computes the laplacian second derivative in all directions
            /// @returns 3x3 kernel
            static Kernel laplacian_second_derivative();

            /// @brief kernel that samples the derivate of a gaussian function
            /// @param dimensions: size of kernel
            /// @returns dimension*dimensions sized kernel
            static Kernel laplacian_of_gaussian(size_t dimensions);

            /// @brief simple gradient in x direction
            /// @returns 2x1 kernel
            static Kernel simple_gradient_x();

            /// @brief simple gradient in x direction
            /// @returns 1x2 kernel
            static Kernel simple_gradient_y();

            /// @brief roberts gradient in x direction
            /// @returns 2x2 kernel
            static Kernel roberts_gradient_x();

            /// @brief roberts gradient in y direction
            /// @returns 2x2 kernel
            static Kernel roberts_gradient_y();

            /// @brief prewitt gradient in x direction
            /// @returns 3x3 kernel
            static Kernel prewitt_gradient_x();

            /// @brief prewitt gradient in y direction
            /// @returns 3x3 kernel
            static Kernel prewitt_gradient_y();

            /// @brief sobel gradient in x direction
            /// @returns 3x3 kernel
            static Kernel sobel_gradient_x();

            /// @brief sobel gradient in y direction
            /// @returns 3x3 kernel
            static Kernel sobel_gradient_y();

            /// @brief kirsch compass gradient north
            /// @returns 3x3 kernel
            static Kernel kirsch_compass_n();

            /// @brief kirsch compass gradient north west
            /// @returns 3x3 kernel
            static Kernel kirsch_compass_nw();

            /// @brief kirsch compass gradient west
            /// @returns 3x3 kernel
            static Kernel kirsch_compass_w();

            /// @brief kirsch compass gradient south west
            /// @returns 3x3 kernel
            static Kernel kirsch_compass_sw();

            /// @brief kirsch compass gradient south
            /// @returns 3x3 kernel
            static Kernel kirsch_compass_s();

            /// @brief kirsch compass gradient south east
            /// @returns 3x3 kernel
            static Kernel kirsch_compass_se();

            /// @brief kirsch compass gradient east
            /// @returns 3x3 kernel
            static Kernel kirsch_compass_e();

            /// @brief kirsch compass gradient north east
            /// @returns 3x3 kernel
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

            template<typename Image_t>
            void apply_mean_to(Image_t& in, Image_t& out);

            template<typename Image_t>
            void apply_median_to(Image_t& in, Image_t& out);
    };
}

#include ".src/spatial_filter.inl"