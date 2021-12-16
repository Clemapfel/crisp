// 
// Copyright 2021 Clemens Cords
// Created on 15.12.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <julia.h>

namespace crisp::julia
{
    enum class NativeType
    {
        BOOL,
        CHAR,
        INT_8,
        UINT_8,
        INT_16,
        UINT_16,
        INT_32,
        UINT_32,
        INT_64,
        UINT_64,
        FLOAT_16,
        FLOAT_32,
        FLOAT_64,
        VOID,
        VOID_PTR,
        NOTHING,
        UNKNOWN
    };

    NativeType get_type(jl_value_t*);
}

#include ".src/julia_native_type.inl"