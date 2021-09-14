// 
// Copyright 2021 Clemens Cords
// Created on 08.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/color_image.hpp>

namespace crisp
{
    class GrayScaleImage : public Image<float, 1>
    {
        public:
            /// @brief default ctor
            GrayScaleImage();

            /// @brief create as specified size
            /// @param x: x-dimension
            /// @param y: y-dimension
            GrayScaleImage(size_t x, size_t y);

            /// @brief create from another 1d image
            /// @param image
            template<typename T>
            GrayScaleImage(const Image<T, 1>&);

            /// @brief convert to visually identical color image
            /// @returns new color image
            ColorImage convert_to_color() const;
    };
}

#include ".src/grayscale_image.inl"