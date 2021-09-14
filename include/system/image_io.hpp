// 
// Copyright 2021 Clemens Cords
// Created on 08.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>
#include <image/color_image.hpp>
#include <image/binary_image.hpp>

namespace crisp
{
    /// @brief load image as binary
    /// @param path: absolute path
    /// @note a warning is displayed if the image file contains pixels that are not either black or white
    BinaryImage load_binary_image(std::string path);

    /// @brief load image as grayscale
    /// @param path: absolute path
    /// @returns image
    GrayScaleImage load_grayscale_image(std::string path);

    /// @brief load image as color
    /// @param path: absolute path
    /// @returns image
    ColorImage load_color_image(std::string path);

    /// @brief save binary image to disk
    /// @param image: binary image
    /// @param path: absolute path
    /// @returns true if  successful, false otherwise
    bool save_to_disk(const BinaryImage&, std::string path);

    /// @brief save grayscale image to disk
    /// @param image: grayscale image
    /// @param path: absolute path
    /// @returns true if  successful, false otherwise
    bool save_to_disk(const GrayScaleImage &, std::string path);

    /// @brief save color image to disk as 8-bit color
    /// @param image: color image
    /// @param path: absolute path
    /// @returns true if  successful, false otherwise
    bool save_to_disk(const ColorImage&, std::string path);
}

#include ".src/image_io.inl"