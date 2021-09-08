// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>
#include <color.hpp>

namespace crisp
{
    // color image whos pixel have RGB values
    class ColorImage : public Image<float, 3>
    {
        public:
            // @brief access individual component of each color representation as 1-d image
            GrayScaleImage get_red_plane();

            // @brief access individual component of each color representation as 1-d image
            GrayScaleImage get_blue_plane();

            // @brief access individual component of each color representation as 1-d image
            GrayScaleImage get_green_plane();

            // @brief access individual component of each color representation as 1-d image
            GrayScaleImage get_hue_plane();

            // @brief access individual component of each color representation as 1-d image
            GrayScaleImage get_saturation_plane();

            // @brief access individual component of each color representation as 1-d image
            GrayScaleImage get_value_plane();

            // @brief access individual component of each color representation as 1-d image
            GrayScaleImage get_lightness_plane();
    };
}

#include ".src/color_image.inl"