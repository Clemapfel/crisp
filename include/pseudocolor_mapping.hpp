// 
// Copyright 2021 Clemens Cords
// Created on 10.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <color.hpp>
#include <image/color_image.hpp>
#include <image/grayscale_image.hpp>
#include <gpu_side/is_gpu_side.hpp>
#include <gpu_side/texture.hpp>
#include <gpu_side/native_handle.hpp>

#include <functional>

/// @brief transforms that map grayscale values to color values
namespace crisp
{
    template<typename side = CPU_SIDE>
    class PseudoColor;

    template<>
    struct PseudoColor<CPU_SIDE>
    {
        PseudoColor() = delete;

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

        /// @brief map multiple gray intensity ranges onto multiple hue ranges simultaneously
        /// @param mapping: mapping of type crisp::PseudoColor::RangeMapping
        static ColorImage value_ranges_to_hue_ranges(RangeMapping& mapping, const GrayScaleImage&);
    };

    /// @brief gpu-side version, requires instance
    template<>
    class PseudoColor<GPU_SIDE>
    {
        public:
            /// @brief ctor
            PseudoColor();

            /// @brief apply to texture, invokes copy ctor
            /// @param grayscale texture
            /// @returns new texture by value
            Texture<float, 3> apply_to(const Texture<float, 1>&) const;

            /// @brief apply to texture in place, does not invokes copy ctor
            /// @param color texture
            /// @returns reference to texture after modification
            Texture<float, 3>& apply_in_place(Texture<float, 3>&) const;

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

        protected:
            /// @brief serialize mapping to an array for use in glsl
            std::array<float, 256> range_mapping_to_array() const;

        private:
            std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> _gray_to_hue;
            std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> _gray_to_inverse_hue;

            GLNativeHandle _program;
    };
}

#include ".src/pseudocolor_mapping.inl"
#include ".src/pseudocolor_mapping_gpu.inl"