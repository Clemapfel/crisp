// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>
#include <color.hpp>

namespace crisp
{
    /// @brief color image whos pixel have crisp::RGB values
    class ColorImage : public Image<float, 3>
    {
        public:
            /// @brief access all red components
            /// @returns component as 1d image
            Image<float, 1> get_red_plane();
            
            /// @brief set all red components
            /// @param plane: plane as 1d image
            void set_red_plane(const Image<float, 1>&);

            /// @brief access all blue components
            /// @returns component as 1d image
            Image<float, 1> get_blue_plane();

            /// @brief set all blue components
            /// @param plane: plane as 1d image
            void set_blue_plane(const Image<float, 1>&);

            /// @brief set all green components
            /// @returns component as 1d image
            Image<float, 1> get_green_plane();

            /// @brief set all green components
            /// @param plane: plane as 1d image
            void set_green_plane(const Image<float, 1>&);

            /// @brief extract hue component as if the image was in HSV
            /// @returns component as 1d image
            Image<float, 1> get_hue_plane();

            /// @brief set all hue components
            /// @param plane: plane as 1d image
            void set_hue_plane(const Image<float, 1>&);

            /// @brief extract saturation component as if the image was in HSV
            /// @returns component as 1d image
            Image<float, 1> get_saturation_plane();

            /// @brief set all saturation components
            /// @param plane: plane as 1d image
            void set_saturation_plane(const Image<float, 1>&);

            /// @brief extract value component as if the image was in HSV
            /// @returns component as 1d image
            Image<float, 1> get_value_plane();

            /// @brief set all value components
            /// @param plane: plane as 1d image
            void set_value_plane(const Image<float, 1>&);

            /// @brief extract lightness component as if the image was in HSL
            /// @param plane: plane as 1d image
            Image<float, 1> get_lightness_plane();

            /// @brief set all lightness components
            /// @param plane: plane as 1d image
            void set_lightness_plane(const Image<float, 1>&);
    };
}

#include ".src/color_image.inl"