// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/binary_image.hpp>

namespace crisp::EdgeDetection
{
    constexpr float MEASURE_THRESHOLD = -1.f;

    BinaryImage threshold_gradient(const GrayScaleImage& original, float threshold = MEASURE_THRESHOLD);
    BinaryImage threshold_sobel(const GrayScaleImage& original, float threshold = MEASURE_THRESHOLD);
    BinaryImage canny(const GrayScaleImage& original, float lower_threshold = MEASURE_THRESHOLD, float upper_threshold = MEASURE_THRESHOLD);
}

#include ".src/edge_detection.inl"