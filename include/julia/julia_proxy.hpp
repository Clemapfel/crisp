// 
// Copyright 2021 Clemens Cords
// Created on 15.12.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <julia.h>
#include <julia/julia_native_type.hpp>

namespace crisp::julia
{
    class Proxy
    {
        public:
            Proxy(jl_value_t*);
            ~Proxy();

            operator bool();
            operator char();

            operator float();
            operator double();

            operator uint8_t();
            operator uint16_t();
            operator uint32_t();
            operator uint64_t();

            operator int8_t();
            operator int16_t();
            operator int32_t();
            operator int64_t();

            operator nullptr_t();

            template<typename T>
            T cast_to() const;

            NativeType _type;
            jl_value_t* _value;
    };
}

#include ".src/julia_proxy.inl"