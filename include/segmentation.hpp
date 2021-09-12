// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image_segment.hpp>
#include <image/multi_plane_image.hpp>

#include <vector>

namespace crisp
{
    // @brief extract all pixels of identical value results in number of segments equal to the number of pairwise different pixel values
    template<typename Image_t>
    std::vector<ImageSegment> decompose_into_segments(const Image_t&, size_t min_segment_size = 2);

    // @brief extract all 4-connected regions of identical value
    template<typename Image_t>
    std::vector<ImageSegment> decompose_into_connected_segments(const Image_t&, size_t min_segment_size = 2);
}

#include ".src/segmentation.inl"