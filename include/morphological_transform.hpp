// 
// Copyright 2021 Clemens Cords
// Created on 09.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>
#include <vector.hpp>

namespace crisp
{
    /// @brief matrix of std::optional<bool> used as flat structuring element, the optional element not having a value represents "don't care" elements
    using StructuringElement = Eigen::Matrix<std::optional<bool>, Eigen::Dynamic, Eigen::Dynamic>;

    /// @brief object representing a morphological transform using a flat structuring element
    class MorphologicalTransform
    {
        public:
            /// @brief default ctor
            MorphologicalTransform() = default;

            /// @brief set a structuring element, all "don't care" by default
            /// @param structuring_element
            void set_structuring_element(StructuringElement);

            /// @brief expose the strucuring element
            /// @returns reference to current structuring element
            StructuringElement& get_structuring_element();

            /// @brief access component of structuring element
            /// @param x: row index
            /// @param y: col index
            /// @returns reference to component
            std::optional<bool>& operator()(size_t x, size_t y);

            /// @brief const-access component of structuring element
            /// @param x: row index
            /// @param y: col index
            /// @returns copy of component
            std::optional<bool> operator()(size_t x, size_t y) const;

            /// @brief specify the structuring elements origin
            /// @param x: row index
            /// @param y: col index
            void set_structuring_element_origin(size_t x, size_t y);

            /// @brief get the structuring elements origin
            /// @returns origin
            Vector2ui get_structuring_element_origin() const;

            /// @brief erode an image with the current structuring element
            /// @param image: image to be modified
            template<typename Image_t>
            void erode(Image_t& image);

            /// @brief geodesically erode an image with the current structuring element
            /// @param image: image to be modified
            /// @param mask: mask image used to limit erosion
            template<typename Image_t>
            void erode(Image_t& image, const Image_t& mask);

            /// @brief dilate an image with the current structuring element
            /// @param image: image to be modified
            template<typename Image_t>
            void dilate(Image_t& image);

            /// @brief geodesically dilate an image with the current structuring element
            /// @param image: image to be modified
            /// @param mask: mask image used to limit dilation
            template<typename Image_t>
            void dilate(Image_t& image, const Image_t& mask);

            /// @brief erode, then dilate an image
            /// @param image: image to be modified
            template<typename Image_t>
            void open(Image_t& image);

            /// @brief dilate, then erode an image
            /// @param image: image to be modified
            template<typename Image_t>
            void close(Image_t& image);

            /// @brief set all pixels where the structuring element occurs in the image to 1, zero otherwise
            /// @param image: image to be modified
            template<typename Image_t>
            void hit_or_miss_transform(Image_t& image);

            /// @brief structuring element of specified size where all elements are "don't care"
            /// @param nrows: x-dimension
            /// @param ncols: y-dimension
            /// @returns structuring element bindable with set_structuring_element
            static StructuringElement all_dont_care(long nrows, long ncols);

            /// @brief structuring element of specified size where all elements are 1
            /// @param nrows: x-dimension
            /// @param ncols: y-dimension
            /// @returns structuring element bindable with set_structuring_element
            static StructuringElement all_foreground(long nrows, long ncols);

            /// @brief structuring element of specified size where all elements are 0
            /// @param nrows: x-dimension
            /// @param ncols: y-dimension
            /// @returns structuring element bindable with set_structuring_element
            static StructuringElement all_background(long nrows, long ncols);

            /// @brief structuring element of specified size where 1/3 * size wide horizontal and vertical line that intersect in the center are foreground, rest "don't care"
            /// @param dimensions: x- and y-dimensions, has to be uneven
            /// @returns structuring element bindable with set_structuring_element
            static StructuringElement cross(long dimensions);

            /// @brief quadratic structuring element of all foreground (1)
            /// @param dimensions: x- and y-dimensions
            /// @returns structuring element bindable with set_structuring_element
            static StructuringElement square(long dimensions);

            /// @brief quadratic structuring element where a diamond (square rotated 45°) is foreground (1), rest "don't care"
            /// @param dimensions: x- and y-dimensions
            /// @returns structuring element bindable with set_structuring_element
            static StructuringElement diamond(long dimensions);

            /// @brief quadratic structuring element a center circle is foreground (1), rest "don't care"
            /// @param dimensions: x- and y-dimensions
            /// @returns structuring element bindable with set_structuring_element
            static StructuringElement circle(long dimensions);

        private:
            Vector2ui _origin;
            StructuringElement _structuring_element;

            template<typename Image_t>
            void erode_aux(Image_t&, Image_t&);

            template<typename Image_t>
            void dilate_aux(Image_t&, Image_t&);
    };
}

#include ".src/morphological_transform.inl"