// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/binary_image.hpp>

namespace crisp::EdgeDetection
{
    /// @brief if handed as an argument to edge detection algorithms, will employ a heuristic to measure the corresponding threshold
    constexpr float MEASURE_THRESHOLD = -1.f;

    /// @brief compute sobel gradient of image and threshold it
    /// @param origin: image
    /// @param threshold: threshold, a heuristic will be employed if not specified
    /// @returns binary image containing the edges
    BinaryImage threshold_gradient(const GrayScaleImage& original, float threshold = MEASURE_THRESHOLD);

    /// @brief employ canny algorithm to comput high-quality 1-pixel thick edges
    /// @param origin: image
    /// @param lower_threshold: threshold for edge candidates
    /// @param upper_threshold: threshold for guruanteed edges
    /// @returns binary image containing the edges
    BinaryImage canny(const GrayScaleImage& original, float lower_threshold = MEASURE_THRESHOLD, float upper_threshold = MEASURE_THRESHOLD);
}

#include ".src/edge_detection.inl"