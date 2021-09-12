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

    template<typename Inner_t>
    BinaryImage manual_threshold(const Image<Inner_t, 1>&, Inner_t threshold);

    template<typename Inner_t>
    BinaryImage basic_threshold(const Image<Inner_t>&);

    template<typename Inner_t>
    BinaryImage otsu_threshold(const Image<Inner_t>&);

    // @param : image
    // @param tail_length_factor: range [0, 1+)
    template<typename Inner_t>
    BinaryImage variable_threshold(const Image<Inner_t>&, float tail_length_factor = 0.05);

    template<typename Inner_t>
    BinaryImage neighborhood_threshold(const Image<Inner_t>&, size_t neighborhood_size = 5);
}

#include ".src/segmentation.inl"