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
    BinaryImage load_binary_image(std::string path);
    GrayScaleImage load_grayscale_image(std::string path);
    ColorImage load_color_image(std::string path);

    bool save_to_disk(const BinaryImage&, std::string path);
    bool save_to_disk(const GrayScaleImage &, std::string path);
    bool save_to_disk(const ColorImage&, std::string path);
}

#include ".src/image_io.inl"