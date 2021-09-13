// 
// Copyright 2021 Clemens Cords
// Created on 10.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>

namespace crisp
{
    // @brief only keep maxima in specified neighborhod
    template<typename Image_t>
    void non_maxima_surpress(Image_t&, size_t neighborhood_size);

    // @brief apply histogram equalization
    template<typename Image_t>
    void histogram_equalize(Image_t&);

    // @brief compute gradient magnitude
    template<typename Image_t>
    GrayScaleImage compute_gradient_magnitude(const Image_t&);

    // @brief compute the

    // @brief quantize into 8-bit, then split into bitplanes
    std::vector<Image<bool, 1>> bitplane_decompose(const Image<float, 1>&);
    Image<float, 1> bitplane_assemble(const std::vector<Image<bool, 1>>&);

    // @brief wavelet transform
    std::vector<Image<float, 1>> wavelet_decompose(const Image<float, 1>&);
    Image<float, 1> wavelet_assemble(const std::vector<Image<bool, 1>>&);
}

#include ".src/whole_image_transform.inl"