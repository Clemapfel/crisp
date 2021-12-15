// 
// Copyright 2021 Clemens Cords
// Created on 15.11.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gpu_side/texture.hpp>

namespace crisp
{
    /// @brief compute gradient magnitude based on sobel kernel
    /// @param texture: texture to be modified
    template<size_t N>
    void compute_gradient_magnitude(Texture<float, N>&);

    /// @brief clamp all pixel values into [0, 1]
    /// @param texture: texture to be modified
    template<size_t N>
    void clamp(Texture<float, N>&);

    /// @brief measure maximum and minimum value, the linearly project all pixel values into [0, 1]
    /// @param texture: texture to be modified
    /// @note for measuring the texture data needs to be examined in ram, causing overhead
    template<size_t N>
    void normalize(Texture<float, N>&);

    /// @brief sigmoid interpolate into [0, 1]
    /// @param texture: texture to be modified
    template<size_t N>
    void smoothstep(Texture<float, N>&);
}

#include ".src/whole_texture_transform.inl"