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
            GrayScaleImage();
            GrayScaleImage(size_t , size_t);

            template<typename T>
            GrayScaleImage(const Image<T, 1>&);

            /// @brief convert to visually identical color image
            ColorImage convert_to_color() const;
    };
}

#include ".src/grayscale_image.inl"