// 
// Copyright 2021 Clemens Cords
// Created on 09.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>
#include <vector.hpp>
#include <gpu_side/texture.hpp>
#include <structuring_element.hpp>

namespace crisp
{
    template<typename, size_t>
    class Texture;

    /// @brief object representing a morphological transform using a flat structuring element
    class MorphologicalTransform
    {
        public:
            /// @brief default ctor
            MorphologicalTransform() = default;

            /// @brief set a structuring element, all "don't care" by default
            /// @param structuring_element
            void set_structuring_element(StructuringElement);

            /// @brief expose the structuring element
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

            /// @brief erode a texture with the current structuring element
            /// @param texture
            template<typename T, size_t N>
            void erode(Texture<T, N>& texture);

            /// @brief geodesically erode an image with the current structuring element
            /// @param image: image to be modified
            /// @param mask: mask image used to limit erosion
            template<typename Image_t>
            void erode(Image_t& image, const Image_t& mask);

            /// @brief dilate an image with the current structuring element
            /// @param image: image to be modified
            template<typename Image_t>
            void dilate(Image_t& image);

            /// @brief erode a texture with the current structuring element
            /// @param texture
            template<typename T, size_t N>
            void dilate(Texture<T, N>& texture);

            /// @brief geodesically dilate an image with the current structuring element
            /// @param image: image to be modified
            /// @param mask: mask image used to limit dilation
            template<typename Image_t>
            void dilate(Image_t& image, const Image_t& mask);

            /// @brief erode, then dilate an image
            /// @param image: image to be modified
            template<typename Image_t>
            void open(Image_t& image);

            /// @brief erode, then dilate a texture
            /// @param texture
            template<typename T, size_t N>
            void open(Texture<T, N>& texture);

            /// @brief dilate, then erode an image
            /// @param image: image to be modified
            template<typename Image_t>
            void close(Image_t& image);

            /// @brief dilate, then erode a texture
            /// @param texture
            template<typename T, size_t N>
            void close(Texture<T, N>& texture);

            /// @brief set all pixels where the structuring element occurs in the image to 1, zero otherwise
            /// @param image: image to be modified
            template<typename Image_t>
            void hit_or_miss_transform(Image_t& image);

            /// @brief replace all occurrences of structuring element in picture with another structuring element
            /// @param image: image to be modified
            /// @param replace: structuring element that will be the replacement, has to be of same size as pattern
            template<typename Image_t>
            void replace_pattern(Image_t& image, const StructuringElement& replacement);

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

            /// @brief non-flat structuring element in the shape of a pyramid with a square base
            /// @param dimensions: x- and y-dimensions
            /// @returns non-flat structuring element
            static NonFlatStructuringElement square_pyramid(long dimensions);

            /// @brief non-flat structuring element in the shape of a pyramid with a diamond base. All non-pyramid elements are "don't care"
            /// @param dimensions: x- and y-dimensions
            /// @returns non-flat structuring element
            static NonFlatStructuringElement diamond_pyramid(long dimensions);

            /// @brief non-flat structuring element in the shape of a pyramid with a circular base. All non-pyramid elements are "don't care"
            /// @param dimensions: x- and y-dimensions
            /// @returns non-flat structuring element
            static NonFlatStructuringElement cone(long dimensions);

            /// @brief non-flat structuring element in the shape of a hemisphere. All elements outside the sphere are "don't care"
            /// @param dimensions: x- and y-dimensions
            /// @returns non-flat structuring element
            static NonFlatStructuringElement hemisphere(long dimensions);

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