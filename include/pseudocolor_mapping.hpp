// 
// Copyright 2021 Clemens Cords
// Created on 10.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <color.hpp>
#include <image/color_image.hpp>
#include <image/grayscale_image.hpp>

#include <functional>

/// @brief transforms that map grayscale values to color values
namespace crisp
{
    struct PseudoColor
    {
        /// @brief the identity function, maps gray value x to RBG(x, x, x, 1)
        /// @returns function settable with set_function
        static ColorImage identity(const GrayScaleImage&);

        /// @brief map single gray value to single hue
        /// @param gray: origin gray value
        /// @param hue: destination hue value
        static ColorImage value_to_hue(float gray, float hue, const GrayScaleImage&);

        /// @brief map range of gray values onto single hue
        /// @param min_gray: lower inclusive boundary of gray values
        /// @param max_gray: upper inclusive boundary of gray values
        /// @param hue: destination hue value
        static ColorImage value_range_to_hue(float min_gray, float max_gray, float hue, const GrayScaleImage&);

        /// @brief map range of gray values [g_a, g_b] onto range of hues [h_a, h_b] such that g_a -> h_a, ..., g_b -> h_b
        /// @param min_gray: lower inclusive boundary of gray values
        /// @param max_gray: upper inclusive boundary of gray values
        /// @param min_hue: lower inclusive boundary of hue values
        /// @param max_hue: upper inclusive boundary of hue values
        static ColorImage value_range_to_hue_range(float min_gray, float max_gray, float min_hue, float max_hue, const GrayScaleImage&);

        /// @brief map range of gray values [g_a, g_b] onto range of hues [h_a, h_b] such that g_a -> h_b, ..., g_b -> h_a
        /// @param min_gray: lower inclusive boundary of gray values
        /// @param max_gray: upper inclusive boundary of gray values
        /// @param min_hue: lower inclusive boundary of hue values
        /// @param max_hue: upper inclusive boundary of hue values
        static ColorImage
        value_range_to_inverse_hue_range(float min_gray, float max_gray, float min_hue, float max_hue, const GrayScaleImage&);

        // @brief container class that allows for specifying multiple ranges at the same time
        class RangeMapping
        {
            friend class PseudoColor;

            public:
                /// @brief default ctor
                RangeMapping() = default;

                /// @brief add exact value to single hue mapping
                /// @param gray: origin gray value
                /// @param hue: destination hue value
                void add_value_to_hue(float gray, float hue);

                /// @brief add value range to single hue mapping
                /// @param min_gray: lower inclusive boundary of gray values
                /// @param max_gray: upper inclusive boundary of gray values
                /// @param hue: destination hue value
                void add_value_range_to_hue(float min_gray, float max_gray, float hue);

                /// @brief add value range to hue range mapping
                /// @param min_gray: lower inclusive boundary of gray values
                /// @param max_gray: upper inclusive boundary of gray values
                /// @param min_hue: lower inclusive boundary of hue values
                /// @param max_hue: upper inclusive boundary of hue values
                void add_value_range_to_hue_range(float min_gray, float max_gray, float min_hue, float max_hue);

                /// @brief add value range to inverse hue range mapping
                /// @param min_gray: lower inclusive boundary of gray values
                /// @param max_gray: upper inclusive boundary of gray values
                /// @param min_hue: lower inclusive boundary of hue values
                /// @param max_hue: upper inclusive boundary of hue values
                void add_value_range_to_inverse_hue_range(float min_gray, float max_gray, float min_hue, float max_hue);

            private:
                std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> _gray_to_hue;
                std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> _gray_to_inverse_hue;
        };

        /// @brief map multiple gray intensity ranges onto multiple hue ranges simultaenously
        /// @param mapping: mapping of type crisp::PseudoColor::RangeMapping
        static ColorImage value_ranges_to_hue_ranges(RangeMapping& mapping, const GrayScaleImage&);

        private:
            PseudoColor() = default;
    };
}

#include ".src/pseudocolor_mapping.inl"