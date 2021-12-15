// 
// Copyright 2021 Clemens Cords
// Created on 15.12.21 by clem (mail@clemens-cords.com)
//

#include <julia/julia_native_type.hpp>
#include <iostream>

namespace crisp::julia 
{
    NativeType get_type(jl_value_t* v)
    {
        if (jl_typeis(v, jl_bool_type))
            return NativeType::BOOL;
        else if (jl_typeis(v, jl_char_type))
            return NativeType::CHAR;
        else if (jl_typeis(v, jl_int8_type))
            return NativeType::INT_8;
        else if (jl_typeis(v, jl_int16_type))
            return NativeType::INT_16;
        else if (jl_typeis(v, jl_int32_type))
            return NativeType::INT_32;
        else if (jl_typeis(v, jl_int64_type))
            return NativeType::INT_64;
        else if (jl_typeis(v, jl_uint8_type))
            return NativeType::UINT_8;
        else if (jl_typeis(v, jl_uint16_type))
            return NativeType::UINT_16;
        else if (jl_typeis(v, jl_uint32_type))
            return NativeType::UINT_32;
        else if (jl_typeis(v, jl_uint64_type))
            return NativeType::UINT_64;
        else if (jl_typeis(v, jl_float16_type))
            return NativeType::FLOAT_16;
        else if (jl_typeis(v, jl_float32_type))
            return NativeType::FLOAT_32;
        else if (jl_typeis(v, jl_float64_type))
            return NativeType::FLOAT_64;
        else if (jl_typeis(v, jl_void_type))
            return NativeType::VOID;
        else if (jl_typeis(v, jl_voidpointer_type))
            return NativeType::VOID_PTR;
        else if (jl_typeis(v, jl_nothing_type))
            return NativeType::NOTHING;
        else 
        {
            std::cerr << "[WARNING] trying to classify julia value type pointer " << v << " of unknown type" << std::endl;
            return NativeType::UNKNOWN;
        }
    }
}