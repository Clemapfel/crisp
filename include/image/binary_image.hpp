// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>
#include <image/grayscale_image.hpp>
#include <image/color_image.hpp>

namespace crisp
{
    // image that uses booleans as pixels
    class BinaryImage : public Image<bool, 1>
    {
        public:
            /// @brief apply unary not to all pixels
            void invert();

            /// @brief convert to visually identical grayscale image
            /// @returns new grayscale image
            GrayScaleImage convert_to_grayscale() const;

            /// @brief convert to visually identical color image
            /// @returns new color image
            ColorImage convert_to_color() const;
            
            /// @brief element-wise binary NOT operator
            /// @returns resulting image
            BinaryImage operator!() const;

            /// @brief element-wise binary AND operator
            /// @param image: another binary image
            /// @returns resulting image
            BinaryImage operator&(const BinaryImage&) const;

            /// @brief element-wise binary OR operator
            /// @param image: another binary image
            /// @returns resulting image
            BinaryImage operator|(const BinaryImage&) const;

            /// @brief element-wise binary XOR operator
            /// @param image: another binary image
            /// @returns resulting image
            BinaryImage operator^(const BinaryImage&) const;

            /// @brief element-wise binary AND operator assignment
            /// @param image: another binary image
            /// @returns reference to itself
            BinaryImage& operator&=(const BinaryImage&);

            /// @brief element-wise binary OR operator assignment
            /// @param image: another binary image
            /// @returns reference to itself
            BinaryImage& operator|=(const BinaryImage&);

            /// @brief element-wise binary XOR operator assignment
            /// @param image: another binary image
            /// @returns reference to itself
            BinaryImage& operator^=(const BinaryImage&);
    };
}

#include ".src/binary_image.inl"