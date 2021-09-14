// 
// Copyright 2021 Clemens Cords
// Created on 10.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>

namespace crisp
{
    /// @brief apply histogram equalization
    /// @param image
    template<typename Image_t>
    void histogram_equalize(Image_t&);

    /// @brief map all values in the image into [0, 1]
    /// @param image
    template<typename Image_t>
    void normalize(Image_t&);

    /// @brief compute gradient magnitude using sobel gradient
    /// @param image
    /// @returns gradient as normalized grayscale image
    template<typename Image_t>
    GrayScaleImage compute_gradient_magnitude(const Image_t&);

    /// @brief quantize into 8-bit, then split into bitplanes
    /// @param image
    /// @returns bitplanes as vector of 1d binary images
    std::vector<Image<bool, 1>> bitplane_decompose(const Image<float, 1>&);

    /// @brief assemble from n bitplanes where n in {8, 16, 32, 64}
    /// @param bitplanes: vector of 1-d binary images
    /// @returns assembled image
    Image<float, 1> bitplane_assemble(const std::vector<Image<bool, 1>>&);

    /// @brief wavelet transform
    /// @param image
    /// @returns vector of wavelet decompositions as 1d grayscale images
    std::vector<Image<float, 1>> wavelet_decompose(const Image<float, 1>&);

    /// @brief wavelet transform
    /// @param vector of wavelet decompositions
    /// @returns assembled image
    Image<float, 1> wavelet_assemble(const std::vector<Image<bool, 1>>&);
}

#include ".src/whole_image_transform.inl"