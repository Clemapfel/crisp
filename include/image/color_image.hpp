// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>
#include <color.hpp>

namespace crisp
{
    class ColorImage : public Image<float, 3>
    {
        public:
            // @brief access individual component of each color representation
            Image<float, 1> get_red_plane();
            Image<float, 1> get_blue_plane();
            Image<float, 1> get_green_plane();
            Image<float, 1> get_hue_plane();
            Image<float, 1> get_saturation_plane();
            Image<float, 1> get_value_plane();
            Image<float, 1> get_lightness_plane();
    };
}

#include ".src/color_image.inl"