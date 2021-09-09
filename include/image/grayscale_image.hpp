// 
// Copyright 2021 Clemens Cords
// Created on 08.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/color_image.hpp>

class ColorImage;

namespace crisp
{
    class ColorImage;

    class GrayScaleImage : public Image<float, 1>
    {
        public:
            // @brief convert to visually identical color image
            ColorImage convert_to_color() const;
    };
}

#include ".src/grayscale_image.inl"