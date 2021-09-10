// 
// Copyright 2021 Clemens Cords
// Created on 10.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <color.hpp>
#include <image/color_image.hpp>
#include <image/grayscale_image.hpp>

#include <functional>

namespace crisp
{
    // maps grayscale values to color values
    class PseudoColorTransform
    {
        public:
            PseudoColorTransform() = default;

            // @brief transform a grayscale image into a color image my mapping certain gray values to the specified hues
            [[nodiscard]] ColorImage transform(const GrayScaleImage&);

            // @brief set the transform function
            void set_function(std::function<RGB(float)>&& mapping);

            // @brief the identity function, maps gray value x to RBG(x, x, x, 1)
            static auto&& identity();

            // @brief map single gray value to single hue
            //
            // @note equivalent to value_range_to_hue_range(gray, gray, hue, hue)
            static auto&& value_to_hue(float gray, float hue);

            // @brief map range of gray values onto single hue
            //
            // @note equivalent to value_range_to_hue_range(min_gray, max_gray, hue, hue)
            static auto&& value_range_to_hue(float min_gray, float max_gray, float hue);

            // @brief maps grayvalues in [min_gray, max_gray] onto HSV values with hue in [min_hue, max_hue]
            static auto&& value_range_to_hue_range(float min_gray, float max_gray, float min_hue = 0, float max_hue = 1);

            // @brief maps grayvalues in [min_gray, max_gray] onto HSV values with hue in [max_hue, min_hue]
            static auto&& value_range_to_inverse_hue_range(float min_gray, float max_gray, float min_hue = 0, float max_hue = 1);

            // container class that allows for specifying multiple ranges at the same time
            class RangeMapping
            {
                friend class PseudoColorTransform;

                public:
                    RangeMapping() = default;

                    // @brief add exact value to single hue mapping
                    void add_value_to_hue(float gray, float hue);

                    // @brief add value range to single hue mapping
                    void add_value_range_to_hue(float min_gray, float max_gray, float hue);

                    // @brief add value range to hue range mapping
                    void add_value_range_to_hue_range(float min_gray, float max_gray, float min_hue, float max_hue);

                    // @brief add value range to inverse hue range mapping
                    void add_value_range_to_inverse_hue_range(float min_gray, float max_gray, float min_hue, float max_hue);

                private:
                    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> _gray_to_hue;
                    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> _gray_to_inverse_hue;
            };

            // @brief map multiple gray intensity ranges onto multiple hue ranges simultaenously
            static auto&& value_ranges_to_hue_ranges(RangeMapping& mapping);

        private:
            std::function<RGB(float)> _function = [](float x) -> RGB {return RGB(x);};
    };
}

#include ".src/pseudocolor_mapping.inl"