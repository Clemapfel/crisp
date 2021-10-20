// 
// Copyright 2021 Clemens Cords
// Created on 20.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gpu_side/native_handle.hpp>

namespace crisp
{
    struct Texture
    {
        Texture(GLNativeHandler id) : _id(id) {}

        GLNativeHandler _id;
    };
}