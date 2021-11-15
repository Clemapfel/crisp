// 
// Copyright 2021 Clemens Cords
// Created on 15.11.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gpu_side/texture.hpp>

namespace crisp
{
    template<size_t N>
    void compute_gradient_magnitude(Texture<float, N>&);
}

#include ".src/whole_texture_transform.inl"